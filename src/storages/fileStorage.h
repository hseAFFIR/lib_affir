//
// Created by Shon on 10.03.2025.
//

#ifndef LIB_AFFIR_FILESTORAGE_H
#define LIB_AFFIR_FILESTORAGE_H

#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

struct DataStruct {
    unsigned long startPos;
    unsigned long endPos;
    std::string filename;
    size_t filesize;
};

typedef unsigned long FileId;

class FileStorage {
public:
    explicit FileStorage(FileId id);
    explicit FileStorage(const std::string& filename, size_t filesize);

    size_t read();
    void write(std::string_view data);

    void close();

    ~FileStorage();
private:
    static unsigned short instancesNumber;
    static unsigned long cursor;
    static std::unordered_map<FileId, DataStruct> dataMap;

    void open();

    DataStruct dataStruct;
    FileId id;
public:
    FileId getId() const;

    size_t getFilesize() const;

    const std::string &getFilename() const;

private:

    const std::string DATA_FILENAME_PATH = "data_file.txt";
    std::fstream dataFile;

    void createFile();
};


#endif //LIB_AFFIR_FILESTORAGE_H
