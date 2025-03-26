//
// Created by Shon on 26.03.2025.
//

#ifndef LIB_AFFIR_SINGLEINDEXSTORAGE_H
#define LIB_AFFIR_SINGLEINDEXSTORAGE_H

#include <fstream>
#include "iIndexStorage.h"

class SingleIndexStorage : public IIndexStorage {
public:
    explicit SingleIndexStorage(unsigned short blockSize);

    explicit SingleIndexStorage() : SingleIndexStorage(BYTES_BLOCK) { };

    void createIndex(std::unordered_map<std::string, BigToken> &data) override;

    void getRawIndex(const std::string& body, std::vector<PosMap &> &vector) override;

    static const unsigned short BYTES_BLOCK = 512;

private:
    const static std::string STORAGE_FILENAME_PATH;
    std::fstream indexStream;
    
    const unsigned short blockSize;

    void open();

    void createIndexFile();
};


#endif //LIB_AFFIR_SINGLEINDEXSTORAGE_H
