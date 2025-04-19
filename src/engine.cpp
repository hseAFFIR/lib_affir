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
        indexStorage = new SingleIndexStorage();
    else if (indexStorageType == IndexStorageType::MULTI)
        indexStorage = new MultiFileIndexStorage();
    else
        throw std::invalid_argument("Unknown type of IndexStorage");

    dataHandler = new DataHandler(filters, buffer, *indexStorage);
    searcher = new Search(filters, *indexStorage);
}

std::vector<Base*> Engine::createFilters(FilterType filterFlags) {
    std::vector<Base*> filters;

    if ((filterFlags & FilterType::HTMLER) == FilterType::HTMLER)
        filters.push_back(new Htmler());
    if ((filterFlags & FilterType::LOWERCASER) == FilterType::LOWERCASER)
        filters.push_back(new Lowercaser());
    if ((filterFlags & FilterType::PUNCTUATOR) == FilterType::PUNCTUATOR)
        filters.push_back(new Punctuator());
    if ((filterFlags & FilterType::STEMMER) == FilterType::STEMMER)
        filters.push_back(new StemFilter());
    if ((filterFlags & FilterType::STOPWORDS) == FilterType::STOPWORDS)
        filters.push_back(new StopWords());
    if ((filterFlags & FilterType::STEMMER_RU) == FilterType::STEMMER_RU)
        filters.push_back(new RussianPorterStemmer());
    if ((filterFlags & FilterType::STEMMER_EN) == FilterType::STEMMER_EN)
        filters.push_back(new EnglishStemmer());

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

SearchResult Engine::find(std::string query) const {
    return std::move(searcher->search(query));
}

std::map<FileId, std::vector<std::string>> Engine::returnResultInContext(const SearchResult &result, size_t contextSymbols) {
    if(!contextSymbols) contextSymbols++;

    std::map<FileId, std::vector<std::string>> res;
    std::vector<char> buffer;
    for (const auto& [fileId, tokenInfos] : result.posMap) {
        FileStorage fileStorage(fileId);
        Pos phraseStart = tokenInfos.front().pos;
        size_t contextStart = (phraseStart >= contextSymbols) ? phraseStart - contextSymbols : 0;

        fileStorage.read(buffer, contextSymbols * 2 + result.queryOriginalSize, contextStart);

        std::string context(buffer.begin(), buffer.end());
        // Чтобы не обрезалось первое и последнее слово в trimContext
        if(!contextStart) context = ' ' + context;

        res[fileId].push_back("..." + trimContext(context) + "...");
    }

    return res;
}

std::string Engine::trimContext(const std::string &context) {
    size_t start = 0;
    size_t end = context.size() - 1;
    while (start < context.size() && context[start] != ' ')
        start++;
    while (end > 0 && context[end] != ' ')
        end--;
    if (start + 1 < context.size()) start++;
    return context.substr(start, end - start);
}

std::string Engine::getFilename(const std::string &path) {
    return std::move(std::filesystem::path(path).filename().string());
}

Engine::~Engine() {
    delete searcher;
    delete dataHandler;
    delete indexStorage;
    for (auto f : filters) delete f;

    instancesNumber--;
    Logger::debug("Engine", "Destructed (number of instances = {})", instancesNumber);
}

void Engine::flush() {
    dataHandler->flush();
    FileStorage::saveStorageMeta();
}

size_t Engine::getFilesize() {
    checkRFSReadiness();
    return readableFileStorage->getFilesize();
}

std::string Engine::getFilename() {
    checkRFSReadiness();
    return readableFileStorage->getFilename();
}

bool Engine::isEnd() {
    checkRFSReadiness();
    return readableFileStorage->isEnd();
}

size_t Engine::read(std::vector<char> &buffer, size_t bytesToRead, size_t startPos) {
    checkRFSReadiness();
    return readableFileStorage->read(buffer, bytesToRead, startPos);
}

size_t Engine::read(std::vector<char> &buffer, size_t bytesToRead) {
    checkRFSReadiness();
    return readableFileStorage->read(buffer, bytesToRead);
}
