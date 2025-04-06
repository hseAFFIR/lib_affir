#include "singleIndexStorage.h"
#include "../../../logger/logger.h"

const std::string SingleIndexStorage::STORAGE_FILENAME_PATH = "index_storage.bin";
const std::string SingleIndexStorage::META_FILENAME_PATH = "index_storage_metadata.bin";
bool SingleIndexStorage::isStorageLoaded = false;
std::unordered_map<std::string, IndexPos> SingleIndexStorage::indexMap;
std::map<uint32_t, uint32_t> SingleIndexStorage::freeBlockPoses;
uint32_t SingleIndexStorage::currentBlockPos = 0;

SingleIndexStorage::SingleIndexStorage() {
    Logger::info("SingleFileStorage", "Init storage");
    loadStorageMeta();
    open();
}

void SingleIndexStorage::createIndex(std::unordered_map<std::string, BigToken> &data) {
    for (const auto& [key, value] : data) {
        uint32_t incomingIndexSize = value.getPosesSize();
        Logger::debug("SingleFileStorage", "Key: {}, Value count: {}, incomingIndexSize: {}",
                      key, value.getFilePositions().size(), incomingIndexSize);
        // Already exists
        if (indexMap.find(key) != indexMap.end()) {
            Logger::debug("SingleFileStorage", "Token body already exist in map. Appending...");
            IndexPos curIndexPos = indexMap[key];
            BlockMask neededMask = getMask(curIndexPos.bytesSize + incomingIndexSize);
            Logger::debug("SingleFileStorage", "Current index pos: {},\nNeeded mask: {}",
                          to_str_indexpos(curIndexPos), (int)neededMask);
            // New mask is short, we need larger
            if(curIndexPos.blockMask != neededMask) {
                Logger::debug("SingleFileStorage", "Mask is short. Getting the bigger one...");
                // Save old position
                IndexPos oldIndexPos = indexMap[key];
                Logger::debug("SingleFileStorage", "Saved old IndexPos: {}", to_str_indexpos(oldIndexPos));
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
        Logger::debug("SingleFileStorage", "IndexPos for {} is {}", key, to_str_indexpos(indexMap[key]));
        updateStorageFile(indexMap[key], value.getFilePositions());
    }
}

void SingleIndexStorage::copyBytes(const IndexPos from, IndexPos& to) {
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

void SingleIndexStorage::updateStorageFile(IndexPos &indexPos, const PosMap& positions) {
    indexStream.seekp(blockToPos(indexPos));
    for (const auto& [fileId, tokens] : positions) {
        for (const auto& token : tokens) {
            indexStream.write(reinterpret_cast<const char*>(&fileId), sizeof(fileId));
            indexStream.write(reinterpret_cast<const char*>(&token.pos), sizeof(token.pos));
            indexStream.write(reinterpret_cast<const char*>(&token.wordPos), sizeof(token.wordPos));
            indexPos.bytesSize += ROW_SIZE;
        }
    }
}

void SingleIndexStorage::getRawIndex(const std::string& body, std::vector<PosMap &> &vector) {

}

void SingleIndexStorage::open() {
    if (indexStream.is_open()) return;
    createIndexFile();
    indexStream.open(STORAGE_FILENAME_PATH, std::ios::out | std::ios::in);
}

void SingleIndexStorage::createIndexFile() {
    indexStream.open(STORAGE_FILENAME_PATH, std::ios::app);
    if (!indexStream.is_open())
        throw std::runtime_error("Cannot open file: " + std::string(strerror(errno)));
    indexStream.close();
}

BlockMask SingleIndexStorage::getMask(size_t size) {
    for(auto mask : allBlockMasks)
        if(size <= mask * mask * MASK_MULTIPLE * ROW_SIZE)
            return mask;
    throw std::runtime_error("There is no suitable mask for the size = " + std::to_string(size));
}

void SingleIndexStorage::markBlockAvailable(const uint32_t blockStart, const uint32_t blockCount) {
    Logger::debug("SingleFileStorage (markBlockAvailable)", "freeBlockPoses (before): {}", to_str_map(freeBlockPoses));
    auto lower_it = freeBlockPoses.lower_bound(blockStart);
    if(lower_it->first == blockStart)
        throw std::runtime_error("Block pos already exists in treeBlockPoses.");

    uint32_t newBlockPos = blockStart;
    uint32_t newBlockCount = blockCount;

    // Merged with prev block
    if(lower_it->first + lower_it->second == blockStart) {
        newBlockPos = lower_it->first;
        newBlockCount = lower_it->second + blockCount;
        freeBlockPoses.erase(lower_it);
    }
    auto upper_it = freeBlockPoses.upper_bound(blockStart);
    // Merged with next block
    if(upper_it->first == blockStart + blockCount) {
        newBlockCount += upper_it->second;
        freeBlockPoses.erase(upper_it);
    }

    freeBlockPoses[newBlockPos] = newBlockCount;
    Logger::debug("SingleFileStorage (markBlockAvailable)", "freeBlockPoses (after): {}", to_str_map(freeBlockPoses));
}

IndexPos SingleIndexStorage::getNewBlock(uint32_t indexSize) {
    BlockMask requiredBlockMask = getMask(indexSize);
    uint32_t requiredBlocks = toBaseBlocks(requiredBlockMask);

    IndexPos indexPos;
    uint32_t availableBlocks = 0;
    // Finding first available pos, which free bytes are greater or equal our indexSize
    for (const auto& pair : freeBlockPoses) {
        if(pair.second <= requiredBlocks) {
            indexPos = IndexPos(requiredBlockMask, pair.first, 0);
            availableBlocks = pair.second;
            break;
        }
    }
    // We should remove reserved space from map
    if(availableBlocks) {
        Logger::debug("SingleFileStorage", "There are availableBlocks: start {}, count {}", indexPos.blockStart, availableBlocks);
        // Remove from freeBlocks == reserve it
        freeBlockPoses.erase(indexPos.blockStart);
        uint32_t newFreeIndexPos = indexPos.blockStart + requiredBlocks;
        markBlockAvailable(newFreeIndexPos, availableBlocks - requiredBlocks);
    }
    // Otherwise there is no such pos, then we must allocate new one
    else {
        Logger::debug("SingleFileStorage", "There are no availableBlocks, create the new one at: {}", currentBlockPos);
        indexPos = IndexPos(requiredBlockMask, currentBlockPos, 0);
        currentBlockPos++;
    }
    return indexPos;
}

void SingleIndexStorage::close() {
    indexStream.close();
}

// Return startPos for startBlock
std::streampos SingleIndexStorage::blockToPos(const IndexPos &indexPos) const {
    return MASK_MULTIPLE * ROW_SIZE * indexPos.blockStart;
}

uint32_t SingleIndexStorage::toBaseBlocks(BlockMask blockMask) {
    return (blockMask * blockMask) / (BlockMask::P_16 * BlockMask::P_16);
}

void SingleIndexStorage::loadStorageMeta() {
    if (isStorageLoaded) return;

    std::ifstream metaFileIn(SingleIndexStorage::META_FILENAME_PATH, std::ios::binary);
    if (!metaFileIn.is_open()) {
        std::cerr << "Cannot open file: " + std::string(strerror(errno)) << std::endl;
        return;
    }

    freeBlockPoses.clear();
    indexMap.clear();

    // Load freeBlockPoses
    size_t freeBlockSize;
    metaFileIn.read(reinterpret_cast<char*>(&freeBlockSize), sizeof(freeBlockSize));
    for (uint32_t i = 0; i < freeBlockSize; ++i) {
        uint32_t key, value;
        metaFileIn.read(reinterpret_cast<char*>(&key), sizeof(key));
        metaFileIn.read(reinterpret_cast<char*>(&value), sizeof(value));
        freeBlockPoses[key] = value;
    }

    // Load indexMap
    size_t indexMapSize;
    metaFileIn.read(reinterpret_cast<char*>(&indexMapSize), sizeof(indexMapSize));
    for (uint32_t i = 0; i < indexMapSize; ++i) {
        size_t keySize;
        metaFileIn.read(reinterpret_cast<char*>(&keySize), sizeof(keySize));
        std::string key(keySize, '\0');
        metaFileIn.read(key.data(), keySize);

        IndexPos value;
        metaFileIn.read(reinterpret_cast<char*>(&value.blockMask), sizeof(value.blockMask));
        metaFileIn.read(reinterpret_cast<char*>(&value.blockStart), sizeof(value.blockStart));
        metaFileIn.read(reinterpret_cast<char*>(&value.bytesSize), sizeof(value.bytesSize));

        indexMap[key] = value;
    }

    metaFileIn.close();
    isStorageLoaded = true;
}

void SingleIndexStorage::saveStorageMeta() {
    std::ofstream metaFileOut(SingleIndexStorage::META_FILENAME_PATH, std::ios::binary);

    size_t freeBlockCount = freeBlockPoses.size();
    // Save free block map
    metaFileOut.write(reinterpret_cast<const char*>(&freeBlockCount), sizeof(freeBlockCount));
    for (const auto& [key, val] : freeBlockPoses) {
        metaFileOut.write(reinterpret_cast<const char*>(&key), sizeof(key));
        metaFileOut.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }

    // Save indexMap
    size_t indexMapSize = indexMap.size();
    metaFileOut.write(reinterpret_cast<char*>(&indexMapSize), sizeof(indexMapSize));
    for (const auto& [key, value] : indexMap) {
        size_t keySize = key.size();
        metaFileOut.write(reinterpret_cast<char*>(&keySize), sizeof(keySize));
        metaFileOut.write(key.data(), keySize);

        metaFileOut.write(reinterpret_cast<const char*>(&value.blockMask), sizeof(value.blockMask));
        metaFileOut.write(reinterpret_cast<const char*>(&value.blockStart), sizeof(value.blockStart));
        metaFileOut.write(reinterpret_cast<const char*>(&value.bytesSize), sizeof(value.bytesSize));
    }

    metaFileOut.close();
}
