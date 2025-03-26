//
// Created by Shon on 26.03.2025.
//

#ifndef LIB_AFFIR_SINGLEINDEXSTORAGE_H
#define LIB_AFFIR_SINGLEINDEXSTORAGE_H

#include <fstream>
#include "../iIndexStorage.h"
#include <unordered_map>
#include "../../../common.h"

enum BlockMask {
    P_16 = 1 << 0,
    P_64 = 1 << 1,
    P_256 = 1 << 2,
    P_1k = 1 << 3,
    P_4k = 1 << 4,
    P_16k = 1 << 5,
    P_65k = 1 << 6,
    P_262k = 1 << 7
};

struct IndexPos {
    BlockMask blockMask;
    size_t blockNumber{};
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

    const int MASK_MULTIPLE = 16;
    const int ROW_SIZE = sizeof(TokenInfo) + sizeof(FileId);

    void open();

    void createIndexFile();
};


#endif //LIB_AFFIR_SINGLEINDEXSTORAGE_H
