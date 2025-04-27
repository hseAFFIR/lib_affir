#ifndef LIB_AFFIR_MULTIFILEINDEXSTORAGE_H
#define LIB_AFFIR_MULTIFILEINDEXSTORAGE_H

#include "../iIndexStorage.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <filesystem>

namespace affir {

/**
 * @brief Multi-file storage implementation for indexing.
 *
 * This class manages token indexes across multiple JSON files, providing
 * efficient storage and retrieval of positional data with in-memory metadata.
 */
    class MultiFileIndexStorage : public IIndexStorage {
    private:
        const char metaChar = ':'; /// Char indicates meta data in file such as file number.
        const std::string metadataFile = STORAGE_DIR + "/metadata.idx";
        unsigned int fileCounter = 0;
        static bool isMetadataLoaded;

        /// In-memory metadata storage: token -> vector of (filename, offset)
        std::unordered_map<std::string, std::vector<std::pair<std::string, size_t>>> metadata;

        /**
         * @brief Converts PosMap to compact JSON format
         */
        std::string posMapToJson(const PosMap &posMap);

        /**
         * @brief Parses compact JSON to PosMap
         */
        void jsonToPosMap(const std::string &jsonStr, PosMap &posMap);

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


        void createIndex(const std::unordered_map<std::string, BigToken> &data) override;

        PosMap getRawIndex(const std::string &body) override;

        void close() override;
    };
}

#endif //LIB_AFFIR_MULTIFILEINDEXSTORAGE_H