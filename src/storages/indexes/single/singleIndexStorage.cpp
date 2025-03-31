#include "singleIndexStorage.h"
#include "../../../logger/logger.h"

const std::string SingleIndexStorage::STORAGE_FILENAME_PATH = "index";
uint32_t currentBlockPos = 0;

SingleIndexStorage::SingleIndexStorage() {
    Logger::info("SingleFileStorage", "Init storage");
    open();
}

void SingleIndexStorage::createIndex(std::unordered_map<std::string, BigToken> &data) {
    for (const auto& [key, value] : data) {
        uint32_t incomingIndexSize = value.getPosesSize();
        // Already exists
        if (indexMap.find(key) != indexMap.end()) {
            IndexPos curIndexPos = indexMap[key];
            BlockMask neededMask = getMask(curIndexPos.bytesSize + incomingIndexSize);
            // New mask is short, we need larger
            if(curIndexPos.blockMask != neededMask) {
                // Save old position
                IndexPos oldIndexPos = indexMap[key];
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
        updateStorageFile(indexMap[key], value.getFilePositions(), incomingIndexSize);
    }
}

void SingleIndexStorage::copyBytes(const IndexPos from, IndexPos& to) {
    std::vector<char> buffer(COPY_BLOCK_SIZE);
    // Move the read position to 'from'
    indexStream.seekg(blockToPos(from));
    // Move the write position to 'to'
    indexStream.seekp(blockToPos(to));

    size_t bytesToCopy = from.bytesSize;
    while (bytesToCopy > 0) {
        size_t bytesToRead = std::min(bytesToCopy, COPY_BLOCK_SIZE);
        // Read data into buffer
        indexStream.read(buffer.data(), (long)bytesToRead);
        // Write the data from buffer
        indexStream.write(buffer.data(), (long)bytesToRead);
        // Decrease the remaining byte count and increase 'to' struct
        bytesToCopy -= bytesToRead;
        to.bytesSize += bytesToRead;
    }
}

void SingleIndexStorage::updateStorageFile(IndexPos &indexPos, const PosMap& positions) {
    indexStream.seekp(blockToPos(indexPos));
    for (const auto& [fileId, tokens] : positions) {
        for (const auto& token : tokens) {
            indexStream.write(reinterpret_cast<const char*>(fileId), sizeof(fileId));
            indexStream.write(reinterpret_cast<const char*>(&token.pos), sizeof(token.pos));
            indexStream.write(reinterpret_cast<const char*>(&token.wordPos), sizeof(token.wordPos));
        }
        indexPos.bytesSize += ROW_SIZE;
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
        // Remove from freeBlocks == reserve it
        freeBlockPoses.erase(indexPos.blockStart);
        uint32_t newFreeIndexPos = indexPos.blockStart + requiredBlocks;
        markBlockAvailable(newFreeIndexPos, availableBlocks - requiredBlocks);
    }
    // Otherwise there is no such pos, then we must allocate new one
    else {
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
