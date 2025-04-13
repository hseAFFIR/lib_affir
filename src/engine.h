//
// Created by Shon on 13.04.2025.
//

#ifndef LIB_AFFIR_ENGINE_H
#define LIB_AFFIR_ENGINE_H

#include "processing/dataHandler.h"
#include "searcher/search.h"
#include "storages/files/fileStorage.h"
#include <filesystem>

class Engine {
public:
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
        STOPWORDS = 1 << 4
    };

    explicit Engine(FilterType filterFlags, IndexStorageType indexStorageType, size_t buffer = DEFAULT_BUFFER_SIZE);

    explicit Engine(IndexStorageType indexStorageType, size_t buffer = DEFAULT_BUFFER_SIZE)
        : Engine(FilterType::NONE, indexStorageType, buffer) { };

    virtual ~Engine();

    void flush();

    void proceed(std::string text, FileStorage &storage);
    /**
     * @brief Reads a text file and processes its contents
     * @param filename Path to the input text file
     * @throws std::runtime_error if file cannot be opened
     */
    void proceed(const std::string& filepath, size_t CHUNK_SIZE = DEFAULT_CHUNK_SIZE);
    /**
     * @brief Processes input text by storing, tokenizing, and indexing it
     *
     * @param text Input text
     * @param filename Target file name
     */
    void proceed(std::string text, const std::string& filename);
    /**
     * @brief Searches for a query in the indexed data
     *
     * @param query Search query
     * @return Search::SearchResult structure
     */
    Search::SearchResult find(std::string query) const;
    /**
     * @brief Utility method to print search result
     */
    static void displayResult(const Search::SearchResult& result);

    static void displayResultInContext(const Search::SearchResult &result, size_t contextWords = 0);

private:
    std::unique_ptr<DataHandler> dataHandler;
    std::unique_ptr<Search> searcher;
    std::unique_ptr<IIndexStorage> indexStorage;

    const std::vector<Base*> filters;

    static constexpr size_t DEFAULT_CHUNK_SIZE = 512;
    static constexpr size_t DEFAULT_BUFFER_SIZE = 1024 * 1024 * 16;

    std::string getFilename(const std::string& path);
    static std::vector<Base*> createFilters(FilterType filterFlags);

    static unsigned short instancesNumber;
};


#endif //LIB_AFFIR_ENGINE_H
