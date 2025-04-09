#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <unordered_map>

#include "../tokenizer/filters/filters.h"
#include "../processing/dataHandler.h"
#include "../storages/indexes/multi/multiFileIndexStorage.h"
#include "../storages/indexes/iIndexStorage.h"
#include "../indexer/indexer.h"

namespace fs = std::filesystem;
/**
 * @brief Time test for DataHandler::processText
 *
 * @param folderPath path to test data
 * @param buffer Maximum buffer size (in bytes) before flushing to storage.
 * @param storage Instance of object MFIS or SFIS
 */
void runTestProcessingWithoutReadTime(const std::string& folderPath, size_t buffer ,std::vector<Base*> &filters, IIndexStorage& storage) {
    DataHandler dh(filters, buffer, storage);

    // Загружаем все файлы заранее, чтобы не учитывать время чтения
    std::unordered_map<std::string, std::string> fileContents;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::ifstream file(entry.path());
            if (!file) {
                std::cerr << "Could not open file: " << entry.path() << std::endl;
                continue;
            }

            std::string text((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
            std::string filename = entry.path().filename().string();
            fileContents[filename] = std::move(text);
        }
    }

    // Засекаем только время обработки
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& [filename, text] : fileContents) {
        dh.processText(text, filename);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "Total processing time (buffer - "<< buffer <<" bites}): "
              << diff.count() << " seconds\n";

    for (auto filter : filters) {
        delete filter;
    }
}


void runTestTokIndIndStor(const std::string& folderPath, size_t buffer ,std::vector<Base*> &filters, IIndexStorage& storage) {
    // Загружаем все файлы заранее, чтобы не учитывать время чтения
    std::unordered_map<std::string, std::string> fileContents;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::ifstream file(entry.path());
            if (!file) {
                std::cerr << "Could not open file: " << entry.path() << std::endl;
                continue;
            }

            std::string text((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
            std::string filename = entry.path().filename().string();
            fileContents[filename] = std::move(text);
        }
    }

    FileId id = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& [filename, text] : fileContents) {
        Tokenizer tk(filters);
        Indexer ind(buffer, storage);

        tk.tokenizeFiltered(text, [&ind](Token token) {

            ind.addToken(token);
        }, id);

        ind.saveTo();
        storage.saveStorageMeta();

        id++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "Total Tokenizer+Indexer+IndexStorage time (buffer - "<< buffer <<" bites}): "
              << diff.count() << " seconds\n";

    for (auto filter : filters) {
        delete filter;
    }

}

#if defined(_WIN32)
#include <windows.h>
    #include <psapi.h>

    void printPeakMemoryUsage() {
        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
            std::cout << "Peak memory usage: "
                      << pmc.PeakWorkingSetSize / 1024 << " KB (Windows)" << std::endl;
        } else {
            std::cerr << "Failed to get memory info on Windows" << std::endl;
        }
    }

#elif defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>

void printPeakMemoryUsage() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        // macOS reports ru_maxrss in bytes, Linux in kilobytes
#if defined(__APPLE__)
        std::cout << "Peak memory usage: "
                  << usage.ru_maxrss / 1024 << " KB (macOS)" << std::endl;
#else
        std::cout << "Peak memory usage: "
                      << usage.ru_maxrss << " KB (Linux/Unix)" << std::endl;
#endif
    } else {
        std::cerr << "Failed to get memory usage on Unix" << std::endl;
    }
}

#else
void printPeakMemoryUsage() {
        std::cerr << "Memory usage not supported on this platform" << std::endl;
    }
#endif




