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
#include <sstream>

// Mask indicating the block size in tokens
enum BlockMask : uint8_t {
    P_512 = 1 << 0,
    P_4k = 1 << 1,
    P_32k = 1 << 2,
    P_262k = 1 << 3,
    P_2b = 1 << 4,
    P_16b = 1 << 5,
    P_134b = 1 << 6,
    P_1t = 1 << 7
};

constexpr BlockMask allBlockMasks[] = {
        BlockMask::P_512,
        BlockMask::P_4k,
        BlockMask::P_32k,
        BlockMask::P_262k,
        BlockMask::P_2b,
        BlockMask::P_16b,
        BlockMask::P_134b,
        BlockMask::P_1t
};
// Information for defining index position in file
struct IndexPos {
    BlockMask blockMask;
    uint32_t blockStart{};
    uint32_t bytesSize{};
};

class SingleIndexStorage : public IIndexStorage {

public:
    explicit SingleIndexStorage();

    void createIndex(std::unordered_map<std::string, BigToken> &data) override;

    void getRawIndex(const std::string& body, std::vector<PosMap> &vector) override;

    void close() override;

    /**
     * @brief Loads metadata for the storage system.
     */
    void loadStorageMeta() override;
    /**
     * @brief Saves metadata for the storage system.
     * @note It has to be called before exit!
     */
    void saveStorageMeta() override;

private:
    static std::unordered_map<std::string, IndexPos> indexMap; ///< Path to the reverse index file.
    const static std::string STORAGE_FILENAME_PATH; ///< Path to the single index file.
    const static std::string META_FILENAME_PATH; ///< Path to the metadata file.
    std::fstream indexStream;
    static bool isStorageLoaded;
    static const int MASK_MULTIPLE = 512;
    static const int ROW_SIZE = sizeof(TokenInfo) + sizeof(FileId);

    std::string to_str_indexpos(const IndexPos& indexPos) const {
        return "(mask: " + std::to_string(indexPos.blockMask)
                + ", start: " + std::to_string(indexPos.blockStart)
                + ", size: " + std::to_string(indexPos.bytesSize) + ")"; }

    static std::string to_str_map(const std::map<uint32_t, uint32_t> &map) {
        std::ostringstream oss;
        oss << "[";
        for (const auto &[blockNum, blockCount]:map) {
            oss << "(" << blockNum << ", " << blockCount << "), ";
        }
        oss << "]";
        return oss.str();
    }

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

    std::streampos blockToPos(const IndexPos &indexPos, bool withSize = false) const;

    void open();

    void createIndexFile();
};


#endif //LIB_AFFIR_SINGLEINDEXSTORAGE_H
