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

    unsigned long fileEnd() const {
        return endPos - startPos;
    }
};

typedef unsigned long FileId;

class FileStorage {
public:
    explicit FileStorage(FileId id);
    explicit FileStorage(const std::string& filename, size_t filesize);

    void write(std::string_view data);
    size_t read(std::vector<char>& buffer, size_t bytesToRead, size_t startPos);
    size_t read(std::vector<char>& buffer, size_t bytesToRead);
    size_t read(std::vector<char>& buffer) { return read(buffer, BYTES_BLOCK); }

    bool isEnd();

    void close();

    static const unsigned short BYTES_BLOCK = 512;
    FileId getId() const;

    size_t getFilesize() const;

    std::string getFilename() const;

    static void loadStorageMeta();
    static void saveStorageMeta();

    ~FileStorage();
private:
    static unsigned short instancesNumber;
    static unsigned long g_cursor;
    static std::unordered_map<FileId, DataStruct> dataMap;
    static bool isStorageLoaded;

    void open();
    void createDataFile();

    DataStruct dataStruct;
    FileId id;

    const static std::string DATA_FILENAME_PATH;
    const static std::string META_FILENAME_PATH;
    std::fstream dataFile;

    size_t currentPosition{};
};


#endif //LIB_AFFIR_FILESTORAGE_H
