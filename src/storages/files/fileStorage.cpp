#include <stdexcept>
#include <iostream>
#include <cstring>
#include "fileStorage.h"
#include "../../logger/logger.h"

unsigned short FileStorage::instancesNumber = 0;
Pos FileStorage::g_cursor = 0;
std::unordered_map<FileId, DataStruct> FileStorage::dataMap;

bool FileStorage::isStorageLoaded = false;
const std::string FileStorage::DATA_FILENAME_PATH = "file_storage_raw";
const std::string FileStorage::META_FILENAME_PATH = "file_storage_metadata.bin";

FileStorage::FileStorage(const std::string &filename, const size_t filesize) {

    loadStorageMeta();

    instancesNumber++;
    id = dataMap.size() + 1;
    dataStruct = DataStruct(g_cursor, filename, filesize);
    dataMap[id] = dataStruct;
    open();
    Logger::debug("FileStorage", "Created file block with: filename = {}, startPos = {}, endPos = {}, id = {}",
                  dataStruct.filename, dataStruct.startPos, dataStruct.endPos(), id);

    dataStream.seekp((long)dataStruct.startPos);
    g_cursor = dataStruct.endPos();
}

FileStorage::FileStorage(const FileId fileId) : id(fileId) {
    loadStorageMeta();

    open();
    dataStruct = dataMap[id];
    currentPosition = 0;
    Logger::debug("FileStorage", "Created file block with: filename = {}, startPos = {}, endPos = {}, id = {}",
                  dataStruct.filename, dataStruct.startPos, dataStruct.endPos(), id);
    }

void FileStorage::open() {
    if (dataStream.is_open()) return;
    createDataFile();
    dataStream.open(DATA_FILENAME_PATH, std::ios::out | std::ios::in);
}

FileStorage::~FileStorage() {
    instancesNumber--;
}

void FileStorage::write(std::string_view data) {
    dataStream.write(data.data(), (long)data.size());
    std::streampos position = dataStream.tellp();

    Logger::debug("FileStorage::write", "New pos (file {}) = {}", dataStruct.filename, (long)position);

    if (position > dataStruct.endPos())
        Logger::debug("FileStorage::write","position > dataStruct.endPos, {} > {}", (long)position, dataStruct.endPos());
}

size_t FileStorage::read(std::vector<char> &buffer, size_t bytesToRead, size_t startPos) {
    if (startPos >= dataStruct.filesize)
        throw std::out_of_range("Starting position exceeds file's end position.");

    dataStream.seekg((long)(startPos + dataStruct.startPos));

    buffer.resize(bytesToRead + 1);
    size_t bytesToEnd = dataStruct.endPos() - dataStream.tellg();
    dataStream.read(buffer.data(), (long)std::min(bytesToRead, bytesToEnd));

    size_t bytesRead = dataStream.gcount();
    Logger::debug("FileStorage::read", "Read {} bytes from position {}", bytesRead, startPos);
    return bytesRead;
}

size_t FileStorage::read(std::vector<char> &buffer, size_t bytesToRead) {
    size_t bytesRead = read(buffer, bytesToRead, currentPosition);
    currentPosition += bytesToRead;
    return bytesRead;
}

FileId FileStorage::getId() const {
    return id;
}

size_t FileStorage::getFilesize() const {
    return dataStruct.filesize;
}

std::string FileStorage::getFilename() const {
    return dataStruct.filename;
}

void FileStorage::createDataFile() {
    dataStream.open(DATA_FILENAME_PATH, std::ios::app);
    if (!dataStream.is_open())
        throw std::runtime_error("Cannot open file: " + std::string(strerror(errno)));
    dataStream.close();
}

bool FileStorage::isEnd() {
    return dataStream.tellg() == dataStruct.endPos();
}

void FileStorage::loadStorageMeta() {
    if (isStorageLoaded) return;
    isStorageLoaded = true;

    dataMap.clear();

    std::ifstream metaFileIn(FileStorage::META_FILENAME_PATH, std::ios::binary);
    if (!metaFileIn.is_open()) {
        Logger::warn("FileStorage", "Cannot open file: {}", std::string(strerror(errno)));
        return;
    }

    // Load cursor
    metaFileIn.read(reinterpret_cast<char *>(&g_cursor), sizeof(g_cursor));

    // Load size of dataMap
    size_t mapSize;
    metaFileIn.read(reinterpret_cast<char *>(&mapSize), sizeof(mapSize));

    for (size_t i = 0; i < mapSize; ++i) {
        // Read FileId
        FileId fileId;
        metaFileIn.read(reinterpret_cast<char *>(&fileId), sizeof(fileId));

        // Read filesize
        DataStruct data;
        metaFileIn.read(reinterpret_cast<char *>(&data.startPos), sizeof(data.startPos));

        // Read filename length
        size_t filenameSize;
        metaFileIn.read(reinterpret_cast<char *>(&filenameSize), sizeof(filenameSize));
        // Read filename
        data.filename.resize(filenameSize);
        metaFileIn.read(&data.filename[0], (long)filenameSize);

        // Read filesize
        metaFileIn.read(reinterpret_cast<char *>(&data.filesize), sizeof(data.filesize));

        dataMap[fileId] = data;
    }
    metaFileIn.close();
    isStorageLoaded = true;
}

void FileStorage::saveStorageMeta() {
    std::ofstream metaFileOut(FileStorage::META_FILENAME_PATH, std::ios::binary);

    // Save cursor
    metaFileOut.write(reinterpret_cast<char *>(&g_cursor), sizeof(g_cursor));

    // Save size of dataMap
    size_t mapSize = dataMap.size();
    metaFileOut.write(reinterpret_cast<char *>(&mapSize), sizeof(mapSize));

    // Save each element of the unordered_map
    for (const auto &pair: dataMap) {
        // Write FileId
        metaFileOut.write(reinterpret_cast<const char *>(&pair.first), sizeof(pair.first));

        // Save DataStruct members
        metaFileOut.write(reinterpret_cast<const char *>(&pair.second.startPos), sizeof(pair.second.startPos));

        size_t filenameSize = pair.second.filename.size();
        // Write length of filename
        metaFileOut.write(reinterpret_cast<char *>(&filenameSize), sizeof(filenameSize));
        // Write filename
        metaFileOut.write(pair.second.filename.c_str(), (long)filenameSize);

        // Write filesize
        metaFileOut.write(reinterpret_cast<const char *>(&pair.second.filesize), sizeof(pair.second.filesize));
    }
    metaFileOut.close();
}

