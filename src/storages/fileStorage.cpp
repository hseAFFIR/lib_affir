//
// Created by Shon on 10.03.2025.
//

#include <stdexcept>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <format>
#include "fileStorage.h"

unsigned short FileStorage::instancesNumber = 0;
unsigned long FileStorage::cursor = 0;

FileStorage::FileStorage(const std::string &filename, const size_t filesize) {
    instancesNumber++;
    id = dataMap.size() + 1;
    dataStruct = DataStruct(cursor, cursor + filesize, filename, filesize);
    dataMap[id] = dataStruct;
    open();

    std::cout << std::format("Created file block with: filename = {}, startPos = {}, endPos = {}, id = {}",
                             dataStruct.filename, dataStruct.startPos, dataStruct.endPos, id) << std::endl;

    dataFile.seekp((long) dataStruct.startPos);
    cursor += dataStruct.endPos + 1;
}

FileStorage::FileStorage(const FileId id) : id(id) {
    open();
    dataStruct = dataMap[id];
    dataFile.seekg((long) dataStruct.startPos);

    std::cout << std::format("Created file block with: filename = {}, startPos = {}, endPos = {}, id = {}",
                             dataStruct.filename, dataStruct.startPos, dataStruct.endPos, id) << std::endl;
}

void FileStorage::open() {
    dataFile.open(DATA_FILENAME_PATH, std::ios::out | std::ios::in);
    if (!dataFile.is_open())
        throw std::runtime_error("Cannot open file: " + std::string(strerror(errno)));
}

FileStorage::~FileStorage() {
    instancesNumber--;
}

size_t FileStorage::read() {
    dataFile.seekg((long) 1024 * 1024 * 1024 * 5);
    char buffer[2];
    dataFile.read(buffer, 1);
    buffer[1] = '\0';
    std::cout << buffer << std::endl;

    return 0;
}

void FileStorage::write(std::string_view data) {
    dataFile.write(data.data(), (long) data.size());
    std::streampos position = dataFile.tellp();
    std::cout << std::format("New pos (file {}) = {}", dataStruct.filename, (long) position) << std::endl;
    if (position > dataStruct.endPos)
        throw std::runtime_error(std::format("position > dataStruct.endPos, {} > {}", (long) position, dataStruct.endPos));
}

void FileStorage::close() {
    dataFile.close();
    delete this;
}

FileId FileStorage::getId() const {
    return id;
}

size_t FileStorage::getFilesize() const {
    return dataStruct.filesize;
}

const std::string &FileStorage::getFilename() const {
    return dataStruct.filename;
}
