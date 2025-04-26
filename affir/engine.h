#ifndef LIB_AFFIR_ENGINE_H
#define LIB_AFFIR_ENGINE_H

#include <filesystem>
#include <map>
#include "processing/dataHandler.h"
#include "searcher/search.h"
#include "searcher/searchResult.h"
#include "storages/files/fileStorage.h"

namespace affir {
    enum class EngineFocus {
        POSES,
        CONTEXT
    };

    enum class IndexStorageType {
        SINGLE,
        MULTI
    };

    enum class FilterType {
        NONE = 0,
        HTMLER = 1 << 0,
        LOWERCASER = 1 << 1,
        PUNCTUATOR = 1 << 2,
        STEMMER = 1 << 3,
        STOPWORDS = 1 << 4,
        STEMMER_RU = 1 << 5,
        STEMMER_EN = 1 << 6
    };

    inline FilterType operator|(FilterType a, FilterType b) {
        using T = std::underlying_type_t<FilterType>;
        return static_cast<FilterType>(static_cast<T>(a) | static_cast<T>(b));
    }

    inline FilterType operator&(FilterType a, FilterType b) {
        using T = std::underlying_type_t<FilterType>;
        return static_cast<FilterType>(static_cast<T>(a) & static_cast<T>(b));
    }

    class Engine {
    public:
        explicit Engine(EngineFocus engineFocus, FilterType filterFlags, IndexStorageType indexStorageType,
                        size_t buffer = DEFAULT_BUFFER_SIZE);

        explicit Engine(EngineFocus engineFocus, IndexStorageType indexStorageType, size_t buffer = DEFAULT_BUFFER_SIZE)
                : Engine(engineFocus, FilterType::NONE, indexStorageType, buffer) { };

        virtual ~Engine();

        /**
         * @brief Forces saving meta data changes
         */
        void flush();

        /**
         * @brief Saves text data into the given storage
         * @details Be careful, you also have to saveStorageMeta by yourself
         */
        void proceed(std::string text, FileStorage &storage);

        /**
         * @brief Reads a text file and processes its contents
         * @param filename Path to the input text file
         * @throws std::runtime_error if file cannot be opened
         */
        void proceed(const std::string &filepath, size_t CHUNK_SIZE = DEFAULT_CHUNK_SIZE);

        /**
         * @brief Processes input text by storing, tokenizing, and indexing it
         *
         * @param text Input text
         * @param filename Target file name
         */
        void proceed(std::string text, const std::string &filename);

        /**
         * @brief Searches for a query in the indexed data
         *
         * @param query Search query
         * @return SearchResult structure
         */
        SearchResult find(std::string query) const;

        /**
         * @brief Utility method to print search result in context
         */
        static std::map<FileId, std::vector<std::string>>
        returnResultInContext(const SearchResult &result, size_t contextSymbols = DEFAULT_CONTEXT_SYMBOLS);

        void openFS(FileId fileId) { readableFileStorage = std::make_unique<FileStorage>(fileId); };

        /**
         * @brief Reads data from the file storage into a buffer, with a specified number of bytes and start position.
         *
         * @param buffer The buffer to store read data.
         * @param bytesToRead The number of bytes to read.
         * @param startPos The starting position in the file to read from.
         * @return The number of bytes successfully read.
         */
        size_t read(std::vector<char> &buffer, size_t bytesToRead, size_t startPos);

        /**
         * @brief Reads data from the last position in file storage into a buffer, with a specified number of bytes.
         *
         * @param buffer The buffer to store read data.
         * @param bytesToRead The number of bytes to read.
         * @return The number of bytes successfully read.
         */
        size_t read(std::vector<char> &buffer, size_t bytesToRead);

        /**
         * @brief Reads a default-sized block of data from the file storage into a buffer.
         *
         * @param buffer The buffer to store read data.
         * @return The number of bytes successfully read.
         */
        size_t read(std::vector<char> &buffer) { return read(buffer, FileStorage::BYTES_BLOCK); }

        /**
         * @brief Checks if the file has reached the end of the data.
         *
         * @return True if the end of the data is reached, false otherwise.
         */
        bool isEnd();

        /**
         * @brief Gets the size of the file.
         *
         * @return The size of the file in bytes.
         */
        size_t getFilesize();

        static size_t getFilesize(FileId fileId) { return FileStorage(fileId).getFilesize(); };

        /**
         * @brief Gets the filename associated with the data in storage.
         *
         * @return The filename.
         */
        std::string getFilename();

        static std::string getFilename(FileId fileId) { return FileStorage(fileId).getFilename(); };
    private:
        DataHandler *dataHandler;
        Search *searcher;
        IIndexStorage *indexStorage;

        std::unique_ptr<FileStorage> readableFileStorage;

        const std::vector<Base *> filters;

        static constexpr size_t DEFAULT_CHUNK_SIZE = 512;
        static constexpr size_t DEFAULT_BUFFER_SIZE = 1024 * 1024 * 16;
        static constexpr size_t DEFAULT_CONTEXT_SYMBOLS = 20;

        std::string getFilename(const std::string &path);

        static std::vector<Base *> createFilters(FilterType filterFlags);

        static unsigned short instancesNumber;

        /**
         * @brief Checks if readableFileStorage is initialisated.
         */
        void checkRFSReadiness() {
            if (!readableFileStorage)
                throw std::logic_error("FileStorage module is not initialized. Run Engine::open() first!");
        }

        static std::string trimContext(const std::string &context);
    };
}

#endif //LIB_AFFIR_ENGINE_H
