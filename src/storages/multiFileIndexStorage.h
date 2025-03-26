#ifndef LIB_AFFIR_MULTIFILEINDEXSTORAGE_H
#define LIB_AFFIR_MULTIFILEINDEXSTORAGE_H

#include "iIndexStorage.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <filesystem>

/**
 * @brief Multi-file storage implementation for indexing.
 *
 * This class manages token indexes across multiple JSON files, providing
 * efficient storage and retrieval of positional data.
 */
class MultiFileIndexStorage : public IIndexStorage {
private:
    std::string storageDir = "index_files"; ///< Directory where index files are stored.
    std::string metadataFile = storageDir + "/index_metadata.json"; ///< File tracking token locations.
    unsigned int fileCounter = 0; ///< Counter for unique file naming.

    /**
     * @brief Loads metadata mapping tokens to index files.
     *
     * @return An unordered_map where keys are tokens and values are lists of file names.
     */
    std::unordered_map<std::string, std::vector<std::string>> loadMetadata();

    /**
     * @brief Saves metadata to JSON format.
     *
     * @param metadata The mapping of tokens to index files.
     */
    void saveMetadata(const std::unordered_map<std::string, std::vector<std::string>>& metadata);

    /**
     * @brief Converts PosMap to a JSON string.
     *
     * @param posMap The positional map to convert.
     * @return A string in JSON format.
     */
    std::string posMapToJson(const PosMap& posMap);

    /**
     * @brief Parses a JSON string into a PosMap.
     *
     * @param jsonStr The JSON string containing position data.
     * @return Parsed PosMap.
     */
    PosMap jsonToPosMap(const std::string& jsonStr);

public:
    MultiFileIndexStorage();

    ~MultiFileIndexStorage();

    void createIndex(std::unordered_map<std::string, BigToken>& data) override;
    void getRawIndex(const std::string& body, std::vector<PosMap>& output) override;
};

#endif //LIB_AFFIR_MULTIFILEINDEXSTORAGE_H
