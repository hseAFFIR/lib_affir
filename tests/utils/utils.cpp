#include "utils.h"
#include <iostream>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>

// Текущее использование памяти
size_t getCurrenMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    return pmc.WorkingSetSize;
}

// Пиковое использование памяти
void printPeakMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        std::cout << "Peak memory usage: "
                  << pmc.PeakWorkingSetSize / 1024 << " KB (Windows)\n";
    } else {
        std::cerr << "Failed to get memory info\n";
    }
}

#elif defined(__APPLE__)
#include <mach/mach.h>
#include <sys/resource.h>

// Текущее использование памяти (RSS)
size_t getCurrenMemoryUsage() {
    task_vm_info_data_t vm_info;
    mach_msg_type_number_t count = TASK_VM_INFO_COUNT;
    kern_return_t result = task_info(
            mach_task_self(),
            TASK_VM_INFO,
            (task_info_t)&vm_info,
            &count
    );

    if (result != KERN_SUCCESS) return 0;
    return vm_info.internal + vm_info.compressed;
}

// Пиковое использование памяти
void printPeakMemoryUsage() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        std::cout << "Peak memory usage: "
                  << usage.ru_maxrss / 1024 << " KB (macOS)\n";
    } else {
        std::cerr << "Failed to get memory info\n";
    }
}

#elif defined(__unix__)

#include <sys/resource.h>
#include <unistd.h>
#include <fstream>

// Текущее использование памяти
size_t getCurrenMemoryUsage() {
    std::ifstream statm("/proc/self/statm");
    size_t rss;
    statm >> rss >> rss; // Второе значение - RSS в страницах
    statm.close();
    return rss * sysconf(_SC_PAGESIZE);
}

// Пиковое использование памяти
void printPeakMemoryUsage() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        std::cout << "Peak memory usage: "
                  << usage.ru_maxrss << " KB (Linux)\n";
    }
    else {
        std::cerr << "Failed to get memory info\n";
    }
}

#else
void printPeakMemoryUsage() {
    std::cerr << "Not supported\n";
}

size_t getCurrenMemoryUsage() {
    return 0;
}
#endif

size_t countOccurrences(const std::string &text, const std::string &word) {
    size_t count = 0;
    size_t pos = 0;
    while ((pos = text.find(word, pos)) != std::string::npos) {
        ++count;
        pos += word.length();
    }
    return count;
}

/**
 * @brief Нативный поиск
 *
 * @param folderPath путь к папке с датасетом
 * @param word слово для поиска
 */
void searchWordInFiles(const std::string &folderPath, const std::string &word) {
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

std::string printIndStorageType(IndexStorageType &indexStorageType) {
    if (indexStorageType == IndexStorageType::MULTI) {
        return "Multi";
    }
    else if (indexStorageType == IndexStorageType::SINGLE) {
        return "Single";
    }

    return "error";
}

