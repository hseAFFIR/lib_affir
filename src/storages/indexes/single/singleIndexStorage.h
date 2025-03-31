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
    uint32_t blockStart{};
    uint32_t bytesSize{};
};

class SingleIndexStorage : public IIndexStorage {

public:
    explicit SingleIndexStorage();

    void createIndex(std::unordered_map<std::string, BigToken> &data) override;

    void getRawIndex(const std::string& body, std::vector<PosMap &> &vector) override;

    void close() override;

private:
    static std::unordered_map<std::string, IndexPos> indexMap;
    const static std::string STORAGE_FILENAME_PATH;
    const static std::string META_FILENAME_PATH; ///< Path to the metadata file.
    std::fstream indexStream;
    static bool isStorageLoaded;
    static const int MASK_MULTIPLE = 16;
    static const int ROW_SIZE = sizeof(TokenInfo) + sizeof(FileId);

    // Block number, block count (available space)
    static std::map<uint32_t, uint32_t> freeBlockPoses;

    static void markBlockAvailable(uint32_t blockStart, uint32_t blockCount);

    void updateStorageFile(IndexPos &indexPos, const PosMap& positions);

    void copyBytes(IndexPos from, IndexPos& to);
    const size_t COPY_BLOCK_SIZE = 4096;

    static IndexPos getNewBlock(uint32_t indexSize);

    static uint32_t currentBlockPos;

    static BlockMask getMask(size_t size);

    static uint32_t toBaseBlocks(BlockMask blockMask);

    std::streampos blockToPos(const IndexPos &indexPos) const;

    void open();

    void createIndexFile();

    /**
 * @brief Loads metadata for the storage system.
 */
    static void loadStorageMeta();
    /**
     * @brief Saves metadata for the storage system.
     * @note It has to be called before exit!
     */
    static void saveStorageMeta();
};


#endif //LIB_AFFIR_SINGLEINDEXSTORAGE_H
