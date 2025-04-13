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

    explicit Engine(const std::vector<Base*>& filters, IndexStorageType indexStorageType, size_t buffer = DEFAULT_BUFFER_SIZE);

    virtual ~Engine();

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
    /**
     * @brief Utility method to print vector of search results
     */
    static void displayResult(const std::vector<Search::SearchResult>& results);

private:
    std::unique_ptr<DataHandler> dataHandler;
    std::unique_ptr<Search> searcher;
    std::unique_ptr<IIndexStorage> indexStorage;

    static constexpr size_t DEFAULT_CHUNK_SIZE = 512;
    static constexpr size_t DEFAULT_BUFFER_SIZE = 1024 * 1024 * 16;

    std::string getFilename(const std::string& path);

    static unsigned short instancesNumber;
};


#endif //LIB_AFFIR_ENGINE_H
