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

    std::cout << "Total processing time (buffer - "<< buffer <<" bytes): "
              << diff.count() << " seconds\n";

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

    std::cout << "Total Tokenizer+Indexer+IndexStorage time (buffer - "<< buffer <<" bytes): "
              << diff.count() << " seconds\n";

}

void runTestGetTokenInCreatedIndex(std::string &text, size_t buffer ,std::vector<Base*> &filters, IIndexStorage& storage) {
    Tokenizer tk(filters);
    Indexer ind(buffer, storage);

    tk.tokenizeFiltered(text, [&ind](Token token) {
        auto start = std::chrono::high_resolution_clock::now();
        ind.getTokenInfo(token.getBody());
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> diff = end - start;
        std::cout << "Total getTokenInfo time: "
                  << diff.count() << " ms\n";
    });
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




size_t countOccurrences(const std::string& text, const std::string& word) {
    size_t count = 0;
    size_t pos = 0;
    while ((pos = text.find(word, pos)) != std::string::npos) {
        ++count;
        pos += word.length();
    }
    return count;
}

void searchWordInFiles(const std::string& folderPath, const std::string& word) {
    std::unordered_map<std::string, std::string> fileContents;

    // Загрузка всех файлов заранее
    for (const auto &entry: fs::directory_iterator(folderPath)) {
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

    // Засекаем время поиска
    auto start = std::chrono::high_resolution_clock::now();

    size_t totalOccurrences = 0;
    for (const auto &[filename, content]: fileContents) {
        size_t count = countOccurrences(content, word);
        totalOccurrences += count;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::cout << "Total occurrences of \"" << word << "\": " << totalOccurrences << '\n';
    std::cout << "Search time: " << elapsed.count() << " ms\n";
}



