#ifndef LIB_AFFIR_MULTIFILEINDEXSTORAGE_H
#define LIB_AFFIR_MULTIFILEINDEXSTORAGE_H

#include "../iIndexStorage.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <filesystem>

/**
 * @brief Multi-file storage implementation for indexing.
 *
 * This class manages token indexes across multiple JSON files, providing
 * efficient storage and retrieval of positional data with in-memory metadata.
 */
class MultiFileIndexStorage : public IIndexStorage {
private:
    const char metaChar = ':'; /// Char indicates meta data in file such as file number.

    std::string storageDir = "index_files";
    std::string metadataFile = storageDir + "/metadata.idx";
    unsigned int fileCounter = 0;
    static bool isMetadataLoaded;

    /// In-memory metadata storage: token -> vector of (filename, offset)
    std::unordered_map<std::string, std::vector<std::pair<std::string, size_t>>> metadata;

    /**
     * @brief Converts PosMap to compact JSON format
     */
    std::string posMapToJson(const PosMap& posMap);

    /**
     * @brief Parses compact JSON to PosMap
     */
    PosMap jsonToPosMap(const std::string& jsonStr);

public:
    MultiFileIndexStorage();

    /**
     * @brief Flushes in-memory metadata to disk
     */
    void saveStorageMeta() override;

    /**
     * @brief Loads metadata from disk to memory
     */
    void loadStorageMeta() override;


    void createIndex(std::unordered_map<std::string, BigToken>& data) override;
    void getRawIndex(const std::string& body, std::vector<PosMap>& output) override;

    void close() override;
};

#endif //LIB_AFFIR_MULTIFILEINDEXSTORAGE_H