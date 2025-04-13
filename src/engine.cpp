//
// Created by Shon on 13.04.2025.
//

#include <fstream>
#include "engine.h"
#include "storages/files/fileStorage.h"

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

    dataHandler->flush();
    FileStorage::saveStorageMeta();
}

void Engine::proceed(std::string text, const std::string &filename) {
    FileStorage storage(filename, text.size());
    proceed(std::move(text), storage);

    dataHandler->flush();
    FileStorage::saveStorageMeta();
}

void Engine::proceed(std::string text, FileStorage &storage) {
    storage.write(text);
    dataHandler->processText(text, storage.getId());
}

Search::SearchResult Engine::find(std::string query) const {
    return searcher->search(query);
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
