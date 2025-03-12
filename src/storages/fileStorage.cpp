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
std::unordered_map<FileId, DataStruct> FileStorage::dataMap;

FileStorage::FileStorage(const std::string &filename, const size_t filesize) {
    instancesNumber++;
    id = dataMap.size() + 1;
    dataStruct = DataStruct(cursor, cursor + filesize, filename, filesize);
    dataMap[id] = dataStruct;
    open();

    std::cout << std::format("Created file block with: filename = {}, startPos = {}, endPos = {}, id = {}",
                             dataStruct.filename, dataStruct.startPos, dataStruct.endPos, id) << std::endl;

    dataFile.seekp((long) dataStruct.startPos);
    cursor = dataStruct.endPos;
}

FileStorage::FileStorage(const FileId id) : id(id) {
    open();
    dataStruct = dataMap[id];
    std::cout << std::format("Created file block with: filename = {}, startPos = {}, endPos = {}, id = {}",
                             dataStruct.filename, dataStruct.startPos, dataStruct.endPos, id) << std::endl;
}

void FileStorage::open() {
    if (dataFile.is_open()) return;
    createFile();
    dataFile.open(DATA_FILENAME_PATH, std::ios::out | std::ios::in);
}

FileStorage::~FileStorage() {
    instancesNumber--;
}

void FileStorage::write(std::string_view data) {
    dataFile.write(data.data(), (long)data.size());
    std::streampos position = dataFile.tellp();
    std::cout << std::format("New pos (file {}) = {}", dataStruct.filename, (long)position) << std::endl;
    if (position > dataStruct.endPos)
        throw std::out_of_range(std::format("position > dataStruct.endPos, {} > {}", (long)position, dataStruct.endPos));
}

size_t FileStorage::read(std::vector<char> &buffer, size_t bytesToRead, size_t startPos) {
    if (startPos >= dataStruct.fileEnd())
        throw std::out_of_range("Starting position exceeds file's end position.");

    dataFile.seekg((long) startPos);

    buffer.resize(bytesToRead + 1);
    size_t bytesToEnd = dataStruct.endPos - dataFile.tellg();
    dataFile.read(buffer.data(), (long)std::min(bytesToRead, bytesToEnd));

    size_t bytesRead = dataFile.gcount();
    std::cout << std::format("Read {} bytes from position {}", bytesRead, startPos) << std::endl;
    return bytesRead;
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

std::string FileStorage::getFilename() const {
    return dataStruct.filename;
}

void FileStorage::createFile() {
    dataFile.open(DATA_FILENAME_PATH, std::ios::app);
    if (!dataFile.is_open())
        throw std::runtime_error("Cannot open file: " + std::string(strerror(errno)));
    dataFile.close();
}

bool FileStorage::isEnd() {
    return dataFile.tellg() == dataStruct.endPos;
}

