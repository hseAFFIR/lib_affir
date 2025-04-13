//
// Created by Shon on 13.04.2025.
//

#include <fstream>
#include "engine.h"
#include "storages/files/fileStorage.h"
#include "storages/indexes/single/singleIndexStorage.h"
#include "storages/indexes/multi/multiFileIndexStorage.h"
#include "tokenizer/filters/filters.h"
#include "logger/logger.h"

unsigned short Engine::instancesNumber = 0;

Engine::Engine(FilterType filterFlags, IndexStorageType indexStorageType, const size_t buffer) : filters(createFilters(filterFlags)) {
    if(instancesNumber > 0)
        throw std::logic_error("Engine instance is already created.");

    instancesNumber++;

    if (indexStorageType == IndexStorageType::SINGLE)
        indexStorage = std::make_unique<SingleIndexStorage>();
    else if (indexStorageType == IndexStorageType::MULTI)
        indexStorage = std::make_unique<MultiFileIndexStorage>();
    else
        throw std::invalid_argument("Unknown type of IndexStorage");

    dataHandler = std::make_unique<DataHandler>(filters, buffer, *indexStorage);
    searcher = std::make_unique<Search>(filters, *indexStorage);
}

std::vector<Base*> Engine::createFilters(Engine::FilterType filterFlags) {
    std::vector<Base*> filters;

    if (static_cast<int>(filterFlags) & static_cast<int>(FilterType::HTMLER))
        filters.push_back(new Htmler());
    if (static_cast<int>(filterFlags) & static_cast<int>(FilterType::LOWERCASER))
        filters.push_back(new Lowercaser());
    if (static_cast<int>(filterFlags) & static_cast<int>(FilterType::PUNCTUATOR))
        filters.push_back(new Punctuator());
    if (static_cast<int>(filterFlags) & static_cast<int>(FilterType::STEMMER))
        filters.push_back(new StemFilter());
    if (static_cast<int>(filterFlags) & static_cast<int>(FilterType::STOPWORDS))
        filters.push_back(new StopWords());

    Logger::debug("Engine", "Filters created: {}", filters.size());

    return std::move(filters);
}

void Engine::proceed(const std::string &filepath, size_t CHUNK_SIZE) {
    std::ifstream file(filepath);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    std::vector<char> buffer(CHUNK_SIZE);

    // Initialise FileStorage in input mode
    const std::string filename = getFilename(filepath);
    const size_t filesize = std::filesystem::file_size(filepath);
    FileStorage storage(filename, filesize);

    // Read unless reached the end of file.
    while (file.read(buffer.data(), (long) CHUNK_SIZE) || file.gcount() > 0) {
        std::string data(buffer.data(), file.gcount());
        proceed(std::move(data), storage);
    }

    flush();
}

void Engine::proceed(std::string text, const std::string &filename) {
    FileStorage storage(filename, text.size());
    proceed(std::move(text), storage);

    flush();
}

void Engine::proceed(std::string text, FileStorage &storage) {
    storage.write(text);
    dataHandler->processText(text, storage.getId());
}

Search::SearchResult Engine::find(std::string query) const {
    return std::move(searcher->search(query));
}

void Engine::displayResult(const Search::SearchResult &result) {
    Search::printSearchResults(result);
}

void Engine::displayResult(const std::vector<Search::SearchResult> &results) {
    Search::printSearchResults(results);
}

std::string Engine::getFilename(const std::string &path) {
    return std::move(std::filesystem::path(path).filename().string());
}

Engine::~Engine() {
    instancesNumber--;
    for (auto f : filters) delete f;
    Logger::debug("Engine", "Destructed (number of instances = {})", instancesNumber);
}

void Engine::flush() {
    dataHandler->flush();
    FileStorage::saveStorageMeta();
}
