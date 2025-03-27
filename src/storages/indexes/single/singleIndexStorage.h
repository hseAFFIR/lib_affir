//
// Created by Shon on 26.03.2025.
//

#ifndef LIB_AFFIR_SINGLEINDEXSTORAGE_H
#define LIB_AFFIR_SINGLEINDEXSTORAGE_H

#include <fstream>
#include "../iIndexStorage.h"
#include <unordered_map>
#include "../../../common.h"
#include <map>

enum BlockMask : uint8_t {
    P_16 = 1 << 0,
    P_64 = 1 << 1,
    P_256 = 1 << 2,
    P_1k = 1 << 3,
    P_4k = 1 << 4,
    P_16k = 1 << 5,
    P_65k = 1 << 6,
    P_262k = 1 << 7,
    P_inf = 0
};

constexpr BlockMask allBlockMasks[] = {
        BlockMask::P_16,
        BlockMask::P_64,
        BlockMask::P_256,
        BlockMask::P_1k,
        BlockMask::P_4k,
        BlockMask::P_16k,
        BlockMask::P_65k,
        BlockMask::P_262k
};

struct IndexPos {
    BlockMask blockMask;
    uint32_t blockNumber{};
    uint32_t size{};
};

class SingleIndexStorage : public IIndexStorage {

public:
    explicit SingleIndexStorage();

    void createIndex(std::unordered_map<std::string, BigToken> &data) override;

    void getRawIndex(const std::string& body, std::vector<PosMap &> &vector) override;


private:
    std::unordered_map<std::string, IndexPos> indexMap;
    const static std::string STORAGE_FILENAME_PATH;
    std::fstream indexStream;

    static const int MASK_MULTIPLE = 16;
    static const int ROW_SIZE = sizeof(TokenInfo) + sizeof(FileId);

    static std::map<uint32_t, BlockMask> treeBlockPoses;

    static void markBlockFree(uint32_t blockPos, BlockMask blockMask);

    static IndexPos getNewBlock(uint32_t indexSize);

    static uint32_t currentBlockPos;

    static BlockMask getMask(size_t size);

    void open();

    void createIndexFile();
};


#endif //LIB_AFFIR_SINGLEINDEXSTORAGE_H
