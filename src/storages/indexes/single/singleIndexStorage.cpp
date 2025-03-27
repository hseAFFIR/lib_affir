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
        // Already exists
        if (indexMap.find(key) != indexMap.end()) {
            IndexPos curIndexPos = indexMap[key];
            uint32_t newIndexSize = value.getPosesSize();
            BlockMask neededMask = getMask(curIndexPos.size + newIndexSize);
            if(curIndexPos.blockMask != neededMask) {
                // Save old position
                IndexPos oldIndexPos = indexMap[key];
                // Save the new one
                markBlockFree(curIndexPos.blockNumber, curIndexPos.blockMask);
                mergeFreeBlock();
                indexMap[key] = getNewBlock(curIndexPos.size + newIndexSize);
                // Copy old bytes
                copyBytes(oldIndexPos, indexMap[key]);

            }
            updateStorageFile(indexMap[key], value.getFilePositions(), newIndexSize);
        }
        // Find available pos for the completely new item
        else {
            uint32_t newIndexSize = value.getPosesSize();
            indexMap[key] = getNewBlock(newIndexSize);
            updateStorageFile(indexMap[key], value.getFilePositions(), newIndexSize);
        }
    }
}

void SingleIndexStorage::copyBytes(const IndexPos from, IndexPos& to) {
    std::vector<char> buffer(COPY_BLOCK_SIZE);
    // Move the read position to 'from'
    indexStream.seekg(blockToPos(from));
    // Move the write position to 'to'
    indexStream.seekp(blockToPos(to));

    size_t bytesToCopy = from.size;
    while (bytesToCopy > 0) {
        size_t bytesToRead = std::min(bytesToCopy, COPY_BLOCK_SIZE);
        // Read data into buffer
        indexStream.read(buffer.data(), (long)bytesToRead);
        // Write the data from buffer
        indexStream.write(buffer.data(), (long)bytesToRead);
        // Decrease the remaining byte count and increase 'to' struct
        bytesToCopy -= bytesToRead;
        to.size += bytesToRead;
    }
}

void SingleIndexStorage::updateStorageFile(IndexPos &indexPos, const PosMap& positions, uint32_t size) {
    // Записать начиная с indexPos.blockPos + indexPos.size
    indexPos.size += size;
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

void SingleIndexStorage::markBlockFree(uint32_t blockPos, BlockMask blockMask) {
    auto lower_it = treeBlockPoses.lower_bound(blockPos);
    if(lower_it->first == blockPos)
        throw std::runtime_error("Block pos already exists in treeBlockPoses.");

    treeBlockPoses[blockPos] = blockMask;
}

void SingleIndexStorage::mergeFreeBlock() {

}


IndexPos SingleIndexStorage::getNewBlock(uint32_t indexSize) {
    BlockMask requiredBlockMask = getMask(indexSize);
    IndexPos indexPos;
    BlockMask foundMask;
    bool isFound = false;
    // Finding first available pos, which mask is greater or equal ours
    for (const auto& pair : treeBlockPoses) {
        if(pair.second <= requiredBlockMask) {
            foundMask = pair.second;
            indexPos = IndexPos(requiredBlockMask, pair.first, 0);
            isFound = true;
            break;
        }
    }
    // We should mark left space as free in tree, since our required space might be less than available
    if(isFound) {
        treeBlockPoses.erase(indexPos.blockNumber);
        const int times = (foundMask * foundMask) / (requiredBlockMask * requiredBlockMask) - 1;
        const uint32_t step = inBaseBlock(requiredBlockMask);
        for (int i = 1; i <= times; ++i)
            markBlockFree(indexPos.blockNumber + step * i, requiredBlockMask);
        mergeFreeBlock();
    }
    // Otherwise there is no such pos, then we must allocate new one
    else {
        indexPos = IndexPos(requiredBlockMask, currentBlockPos, 0);
        currentBlockPos++;
    }
    return indexPos;
}

uint32_t SingleIndexStorage::inBaseBlock(BlockMask blockMask) {
    return (blockMask * blockMask) / (BlockMask::P_16 * BlockMask::P_16);
}

void SingleIndexStorage::close() {
    indexStream.close();
}

std::streampos SingleIndexStorage::blockToPos(IndexPos indexPos) const {
    return indexPos.blockMask * indexPos.blockMask * MASK_MULTIPLE * ROW_SIZE * indexPos.blockNumber;
}
