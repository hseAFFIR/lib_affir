#include "singleIndexStorage.h"
#include "../../../logger/logger.h"

using namespace affir;

const std::string SingleIndexStorage::STORAGE_FILENAME_PATH = STORAGE_DIR + "/index_storage.bin";
const std::string SingleIndexStorage::META_FILENAME_PATH = STORAGE_DIR + "/index_storage_metadata.bin";
bool SingleIndexStorage::isStorageLoaded = false;
std::unordered_map<std::string, IndexPos> SingleIndexStorage::indexMap;
std::map<uint32_t, uint32_t> SingleIndexStorage::freeBlockPoses;
uint32_t SingleIndexStorage::currentBlockPos = 0;

SingleIndexStorage::SingleIndexStorage() {
    LOG_INFO(Logger::logger, "SingleIndexStorage module initialized");
    loadStorageMeta();
    open();
}

void SingleIndexStorage::createIndex(const std::unordered_map<std::string, BigToken> &data) {
    for (const auto &[key, value]: data) {
        uint32_t incomingIndexSize = value.getFullPosesSize();
        LOG_DEBUG(Logger::logger, "Key: {}, Value count: {}, incomingIndexSize: {}",
                  key, value.getFilePositions().size(), incomingIndexSize);
        // Already exists
        if (indexMap.find(key) != indexMap.end()) {
            LOG_DEBUG(Logger::logger, "Token body already exists in map. Appending...");
            IndexPos curIndexPos = indexMap[key];
            BlockMask neededMask = getMask(curIndexPos.bytesSize + incomingIndexSize);
            LOG_DEBUG(Logger::logger, "Current index pos: {},\n\tNeeded mask: {}", to_str_indexpos(curIndexPos),
                      (int)neededMask);
            // New mask is short, we need larger
            if (curIndexPos.blockMask != neededMask) {
                LOG_DEBUG(Logger::logger, "Mask is short. Getting the bigger one...");
                // Save old position
                IndexPos oldIndexPos = indexMap[key];
                LOG_DEBUG(Logger::logger, "Saved old IndexPos: {}", to_str_indexpos(oldIndexPos));
                // Save the new one
                markBlockAvailable(curIndexPos.blockStart, toBaseBlocks(curIndexPos.blockMask));
                indexMap[key] = getNewBlock(curIndexPos.bytesSize + incomingIndexSize);
                // Copy old bytes
                copyBytes(oldIndexPos, indexMap[key]);
            }
        }
            // Find available pos for the completely new item
        else
            indexMap[key] = getNewBlock(incomingIndexSize);
        updateStorageFile(indexMap[key], value.getFilePositions());
        LOG_DEBUG(Logger::logger, "IndexPos for {} is {}", key, to_str_indexpos(indexMap[key]));
    }
}

void SingleIndexStorage::copyBytes(const IndexPos from, IndexPos &to) {
    std::vector<char> buffer(COPY_BLOCK_SIZE);
    std::streampos readPos = blockToPos(from);
    std::streampos writePos = blockToPos(to);

    size_t bytesToCopy = from.bytesSize;
    while (bytesToCopy > 0) {
        size_t bytesToRead = std::min(bytesToCopy, COPY_BLOCK_SIZE);
        // Read data into buffer
        indexStream.seekg(readPos);
        readPos = indexStream.read(buffer.data(), (long)bytesToRead).tellg();
        // Write the data from buffer
        indexStream.seekp(writePos);
        writePos = indexStream.write(buffer.data(), (long)bytesToRead).tellp();
        // Decrease the remaining byte count and increase 'to' struct
        bytesToCopy -= bytesToRead;
        to.bytesSize += bytesToRead;
    }
}

void SingleIndexStorage::updateStorageFile(IndexPos &indexPos, const PosMap &positions) {
    indexStream.seekp(blockToPos(indexPos, true));
    for (const auto &[fileId, tokens]: positions) {
        for (const auto &token: tokens) {
            indexStream.write(reinterpret_cast<const char *>(&fileId), sizeof(fileId));
            indexStream.write(reinterpret_cast<const char *>(&token.pos), sizeof(token.pos));
            indexStream.write(reinterpret_cast<const char *>(&token.wordPos), sizeof(token.wordPos));
            indexPos.bytesSize += ROW_SIZE;
        }
    }
}

PosMap SingleIndexStorage::getRawIndex(const std::string &body) {
    PosMap output;
    IndexPos indexPos = indexMap[body];
    indexStream.seekg(blockToPos(indexPos));
    for (size_t i = 0; i < indexPos.bytesSize; i += ROW_SIZE) {
        FileId fileId;
        Pos pos;
        Pos wordPos;

        indexStream.read(reinterpret_cast<char *>(&fileId), sizeof(fileId));
        indexStream.read(reinterpret_cast<char *>(&pos), sizeof(pos));
        indexStream.read(reinterpret_cast<char *>(&wordPos), sizeof(wordPos));

        output[fileId].emplace_back(pos, wordPos);
    }
    return std::move(output);
}

void SingleIndexStorage::open() {
    if (indexStream.is_open()) return;
    createIndexFile();
    indexStream.open(STORAGE_FILENAME_PATH, std::ios::out | std::ios::in);
}

void SingleIndexStorage::createIndexFile() {
    indexStream.open(STORAGE_FILENAME_PATH, std::ios::app);
    if (!indexStream.is_open())
//        throw std::runtime_error("Cannot open file: " + std::string(strerror(errno)));
        LOG_ERROR(Logger::logger, "Cannot open file: {}", std::string(strerror(errno)));
    indexStream.close();
}

BlockMask SingleIndexStorage::getMask(size_t size) {
    for (auto mask: allBlockMasks)
        if (size <= mask * mask * mask * MASK_MULTIPLE * ROW_SIZE)
            return mask;
    throw std::runtime_error("There is no suitable mask for the size = " + std::to_string(size));
}

void SingleIndexStorage::markBlockAvailable(const uint32_t blockStart, const uint32_t blockCount) {
    if (blockCount <= 0) return;
    LOG_DEBUG(Logger::logger, "freeBlockPoses (before): {}", to_str_map(freeBlockPoses));
    // -1 to avoid self finding
    auto lower_it = freeBlockPoses.lower_bound(blockStart - 1);

    uint32_t newBlockPos = blockStart;
    uint32_t newBlockCount = blockCount;

    // Merged with prev block
    if (lower_it->second > 0 && lower_it->first + lower_it->second == blockStart) {
        newBlockPos = lower_it->first;
        newBlockCount = lower_it->second + blockCount;
        freeBlockPoses.erase(lower_it);
    }
    auto upper_it = freeBlockPoses.upper_bound(blockStart);
    // Merged with next block
    if (upper_it->second > 0 && upper_it->first == blockStart + blockCount) {
        newBlockCount += upper_it->second;
        freeBlockPoses.erase(upper_it);
    }

    freeBlockPoses[newBlockPos] = newBlockCount;
    LOG_DEBUG(Logger::logger, "freeBlockPoses (after): {}", to_str_map(freeBlockPoses));
}

IndexPos SingleIndexStorage::getNewBlock(uint32_t indexSize) {
    BlockMask requiredBlockMask = getMask(indexSize);
    uint32_t requiredBlocks = toBaseBlocks(requiredBlockMask);

    IndexPos indexPos;
    uint32_t availableBlocks = 0;
    // Finding first available pos, which free bytes are greater or equal our indexSize
    for (const auto &pair: freeBlockPoses) {
        if (pair.second >= requiredBlocks) {
            indexPos = IndexPos(requiredBlockMask, pair.first, 0);
            availableBlocks = pair.second;
            break;
        }
    }
    // We should remove reserved space from map
    if (availableBlocks) {
        LOG_DEBUG(Logger::logger, "There are availableBlocks: start {}, count {}", indexPos.blockStart,
                  availableBlocks);
        // Remove from freeBlocks == reserve it
        LOG_DEBUG(Logger::logger, "Delete this block sequence.");
        freeBlockPoses.erase(indexPos.blockStart);
        uint32_t newFreeIndexPos = indexPos.blockStart + requiredBlocks;
        LOG_DEBUG(Logger::logger, "Adding left sequence ({}, {})...", newFreeIndexPos,
                  availableBlocks - requiredBlocks);
        markBlockAvailable(newFreeIndexPos, availableBlocks - requiredBlocks);
    }
        // Otherwise there is no such pos, then we must allocate new one
    else {
        LOG_DEBUG(Logger::logger, "There are NO availableBlocks, create the new one at: {}", currentBlockPos);
        indexPos = IndexPos(requiredBlockMask, currentBlockPos, 0);
        currentBlockPos += requiredBlocks;
    }
    return indexPos;
}

void SingleIndexStorage::close() {
    indexStream.close();
}

// Return startPos for startBlock
std::streampos SingleIndexStorage::blockToPos(const IndexPos &indexPos, bool withSize) const {
    uint32_t size = 0;
    if (withSize) size = indexPos.bytesSize;
    return MASK_MULTIPLE * ROW_SIZE * indexPos.blockStart + size;
}

uint32_t SingleIndexStorage::toBaseBlocks(BlockMask blockMask) {
    return (blockMask * blockMask * blockMask);
}

void SingleIndexStorage::loadStorageMeta() {
    if (isStorageLoaded) return;
    isStorageLoaded = true;

    freeBlockPoses.clear();
    indexMap.clear();

    std::ifstream metaFileIn(SingleIndexStorage::META_FILENAME_PATH, std::ios::binary);

    if (!metaFileIn.is_open()) {
        LOG_ERROR(Logger::logger, "Cannot open file: {}", std::string(strerror(errno)));
        return;
    }

    // Load current block pos
    metaFileIn.read(reinterpret_cast<char *>(&currentBlockPos), sizeof(currentBlockPos));

    // Load freeBlockPoses
    size_t freeBlockSize;
    metaFileIn.read(reinterpret_cast<char *>(&freeBlockSize), sizeof(freeBlockSize));
    for (uint32_t i = 0; i < freeBlockSize; ++i) {
        uint32_t key, value;
        metaFileIn.read(reinterpret_cast<char *>(&key), sizeof(key));
        metaFileIn.read(reinterpret_cast<char *>(&value), sizeof(value));
        freeBlockPoses[key] = value;
    }

    // Load indexMap
    size_t indexMapSize;
    metaFileIn.read(reinterpret_cast<char *>(&indexMapSize), sizeof(indexMapSize));
    for (uint32_t i = 0; i < indexMapSize; ++i) {
        size_t keySize;
        metaFileIn.read(reinterpret_cast<char *>(&keySize), sizeof(keySize));
        std::string key(keySize, '\0');
        metaFileIn.read(key.data(), keySize);

        IndexPos value;
        metaFileIn.read(reinterpret_cast<char *>(&value.blockMask), sizeof(value.blockMask));
        metaFileIn.read(reinterpret_cast<char *>(&value.blockStart), sizeof(value.blockStart));
        metaFileIn.read(reinterpret_cast<char *>(&value.bytesSize), sizeof(value.bytesSize));

        indexMap[key] = value;
    }

    metaFileIn.close();
}

void SingleIndexStorage::saveStorageMeta() {
    std::ofstream metaFileOut(SingleIndexStorage::META_FILENAME_PATH, std::ios::binary);

    // Save current block pos
    metaFileOut.write(reinterpret_cast<const char *>(&currentBlockPos), sizeof(currentBlockPos));
    // Save free block map
    size_t freeBlockCount = freeBlockPoses.size();
    metaFileOut.write(reinterpret_cast<const char *>(&freeBlockCount), sizeof(freeBlockCount));
    for (const auto &[key, val]: freeBlockPoses) {
        metaFileOut.write(reinterpret_cast<const char *>(&key), sizeof(key));
        metaFileOut.write(reinterpret_cast<const char *>(&val), sizeof(val));
    }

    // Save indexMap
    size_t indexMapSize = indexMap.size();
    metaFileOut.write(reinterpret_cast<char *>(&indexMapSize), sizeof(indexMapSize));
    for (const auto &[key, value]: indexMap) {
        size_t keySize = key.size();
        metaFileOut.write(reinterpret_cast<char *>(&keySize), sizeof(keySize));
        metaFileOut.write(key.data(), keySize);

        metaFileOut.write(reinterpret_cast<const char *>(&value.blockMask), sizeof(value.blockMask));
        metaFileOut.write(reinterpret_cast<const char *>(&value.blockStart), sizeof(value.blockStart));
        metaFileOut.write(reinterpret_cast<const char *>(&value.bytesSize), sizeof(value.bytesSize));
    }

    metaFileOut.close();
}
