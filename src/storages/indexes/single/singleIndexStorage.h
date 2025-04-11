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

    void getRawIndex(const std::string& body, PosMap &output) override;

    void close() override;

    void loadStorageMeta() override;

    void saveStorageMeta() override;

private:
    static std::unordered_map<std::string, IndexPos> indexMap; ///< Path to the reverse index file.
    const static std::string STORAGE_FILENAME_PATH; ///< Path to the single index file.
    const static std::string META_FILENAME_PATH; ///< Path to the metadata file.
    std::fstream indexStream; ///< File descriptor for index file.
    static bool isStorageLoaded; ///< Flag for preventing loading meta storage twice.
    static const int MASK_MULTIPLE = 512; ///< Size of basic mask block.
    static const int ROW_SIZE = sizeof(TokenInfo) + sizeof(FileId); ///< Size of one position for token in index file.

    /**
     * @brief Prints IndexPos.
     *
     * @param IndexPos Struct that has to be printed.
     * @return Output string.
     */
    std::string to_str_indexpos(const IndexPos& indexPos) const {
        return "(mask: " + std::to_string(indexPos.blockMask)
                + ", start: " + std::to_string(indexPos.blockStart)
                + ", size: " + std::to_string(indexPos.bytesSize) + ")"; }

    /**
    * @brief Prints free blocks in index file.
    *
    * @param map Map that has to be printed.
     * @return Output string.
    */
    static std::string to_str_map(const std::map<uint32_t, uint32_t> &map) {
        std::ostringstream oss;
        oss << "[";
        for (const auto &[blockNum, blockCount]:map) {
            oss << "(" << blockNum << ", " << blockCount << "), ";
        }
        oss << "]";
        return oss.str();
    }

    static std::map<uint32_t, uint32_t> freeBlockPoses;  ///< Block number, block count (available space).

    /**
    * @brief Mark block of file index in freeBlockPoses map as available for writing.
    *
    * @param blockStart Start of available block sequence.
    * @param blockCount Size of block sequence.
    */
    static void markBlockAvailable(uint32_t blockStart, uint32_t blockCount);

    /**
    * @brief Write token positions ib byte form to index file.
    *
    * @param indexPos Position in index file to write down token info.
    * @param positions Positions of token in somewhere.
    */
    void updateStorageFile(IndexPos &indexPos, const PosMap& positions);

    /**
    * @brief Copies bytes from one index pos to another.
    *
    * @param from Index pos from.
    * @param to Index pos to.
    */
    void copyBytes(IndexPos from, IndexPos& to);
    const size_t COPY_BLOCK_SIZE = 4096; ///< Size of copy block.
    /**
    * @brief Provide block for the token positions.
    *
    * @param indexSize Size of token info (its positions).
     * @return IndexPos for the given positions.
    */
    static IndexPos getNewBlock(uint32_t indexSize);

    static uint32_t currentBlockPos; ///< Head to the new index block.
    /**
    * @brief Gets appropriate mask for the given size.
    *
    * @param size Size that has to be placed in block.
     * @return BlockMask type.
    */
    static BlockMask getMask(size_t size);
    /**
     * @brief Convert block mask to the base one.
     *
     * @param blockMask Given block mask.
     * @return Count of base blocks.
    */
    static uint32_t toBaseBlocks(BlockMask blockMask);
    /**
     *
     * @brief Convert IndexPos mask to file position.
     *
     * @param indexPos IndexPos to convert to file positions.
     * @param withSize Return the end of IndexPos position in file (default is false)
     * @return Pos in file.
    */
    std::streampos blockToPos(const IndexPos &indexPos, bool withSize = false) const;
    /**
    * @brief Opens index file.
    */
    void open();

    /**
    * @brief Creates index file if not exists.
    */
    void createIndexFile();
};


#endif //LIB_AFFIR_SINGLEINDEXSTORAGE_H
