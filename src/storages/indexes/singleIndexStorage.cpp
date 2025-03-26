#include "singleIndexStorage.h"
#include "../../logger/logger.h"

const std::string SingleIndexStorage::STORAGE_FILENAME_PATH = "index";

SingleIndexStorage::SingleIndexStorage(unsigned short blockSize) : blockSize(blockSize) {
    Logger::info("SingleFileStorage", "Init storage with block size = {}", blockSize);
    open();
}

void SingleIndexStorage::createIndex(std::unordered_map<std::string, BigToken> &data) {

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