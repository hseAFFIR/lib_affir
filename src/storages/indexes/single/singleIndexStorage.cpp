#include "singleIndexStorage.h"
#include "../../../logger/logger.h"

const std::string SingleIndexStorage::STORAGE_FILENAME_PATH = "index";
uint32_t currentBlockPos = 0;

SingleIndexStorage::SingleIndexStorage() {
    Logger::info("SingleFileStorage", "Init storage");
//    open();
}

void SingleIndexStorage::createIndex(std::unordered_map<std::string, BigToken> &data) {
    for (const auto& [key, value] : data) {
        if (indexMap.find(key) == indexMap.end()) {
            BlockMask mask = getMask(value.getPosesSize());
            indexMap[key] = IndexPos(mask, 1, value.getPosesSize());
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
    return P_inf;
}

void SingleIndexStorage::markBlockFree(uint32_t blockPos, BlockMask blockMask) {

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
            indexPos = IndexPos(requiredBlockMask, pair.first, indexSize);
            isFound = true;
            break;
        }
    }
    // We should mark left space as free in tree, since our required space might be less than available
    if(isFound) {
        treeBlockPoses.erase(indexPos.blockNumber);
        const int times = (foundMask * foundMask) / (requiredBlockMask * requiredBlockMask) - 1;
        const uint32_t step = (foundMask * foundMask) / (BlockMask::P_16 * BlockMask::P_16);
        for (int i = 1; i <= times; ++i)
            markBlockFree(indexPos.blockNumber + step * 1, foundMask);
    }
    // Otherwise there is no such pos, then we must allocate new one
    else {
        indexPos = IndexPos(requiredBlockMask, currentBlockPos, indexSize);
        currentBlockPos++;
    }
    return indexPos;
}
