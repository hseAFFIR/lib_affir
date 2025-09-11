#include "../tests.h"
#include <ctime>

std::string csvFormatData;

void runEngineTestFile(std::string dataPath, EngineFocus engineFocus, FilterType filterFlags,
                   IndexStorageType indexStorageType, size_t buffer) {
    deleteStorageForTests();

    std::vector<std::string> filenames;
    std::string filter = filterTypeToString(filterFlags);

    std::cout << "============================== Test ==============================\n";
    std::cout << "Name: engine without file process\n";
    std::cout << "Buffer - " << buffer << " bytes | indexStorage - " << printIndStorageType(indexStorageType)
              << std::endl;
    std::cout << "Filters - " << filter << std::endl;

    for (const auto &entry: fs::directory_iterator(dataPath)) {
        if (entry.is_regular_file()) {
            std::ifstream file(entry.path());
            if (!file) {
                std::cerr << "Could not open file: " << entry.path() << std::endl;
                continue;
            }

            std::string text((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
            std::string filename = dataPath + "/" + entry.path().filename().string();
            filenames.push_back(filename);
            file.close();
        }
    }

    std::clock_t cpuStart = std::clock();
    auto start = std::chrono::high_resolution_clock::now();
    size_t startMem = getCurrenMemoryUsage();

    Engine engine(engineFocus, filterFlags, indexStorageType, buffer);

    for (auto &filename: filenames) {
        engine.proceed(filename);
    }

    std::clock_t cpuEnd = std::clock();
    auto end = std::chrono::high_resolution_clock::now();
    size_t endMem = getCurrenMemoryUsage();

    size_t difMemKb = (endMem - startMem) / (1024);
    engine.flush();
    double cpuTime = static_cast<double>(cpuEnd - cpuStart) / CLOCKS_PER_SEC;
    std::chrono::duration<double> diff = end - start;

    std::cout << "Wall-clock Time: " << diff.count() << " seconds\n";
    std::cout << "CPU Time: " << cpuTime << " seconds\n";
    std::cout << "Memory usage: " << difMemKb << " Kb\n";
    std::cout << "==================================================================\n";

    csvFormatData.append(formatCsvString(buffer, indexStorageType, filter, diff, cpuTime, difMemKb));

}


void runEngineTestString(std::string dataPath, EngineFocus engineFocus, FilterType filterFlags,
                           IndexStorageType indexStorageType, size_t buffer) {
    deleteStorageForTests();

    std::unordered_map<std::string, std::string> fileContents;
    std::string filter = filterTypeToString(filterFlags);

    std::cout << "============================== Test ==============================\n";
    std::cout << "Name: engine string process\n";
    std::cout << "Buffer - " << buffer << " bytes | indexStorage - " << printIndStorageType(indexStorageType)
              << std::endl;

    std::cout << "Filters - " << filter << std::endl;


    for (const auto &entry: fs::directory_iterator(dataPath)) {
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
            file.close();
        }
    }

    std::clock_t cpuStart = std::clock();
    auto start = std::chrono::high_resolution_clock::now();
    size_t startMem = getCurrenMemoryUsage();

    Engine engine(engineFocus, filterFlags, indexStorageType, buffer);

    for (auto &[filename, text]: fileContents) {
        engine.proceed(text, filename);
    }

    std::clock_t cpuEnd = std::clock();
    auto end = std::chrono::high_resolution_clock::now();
    size_t endMem = getCurrenMemoryUsage();

    size_t difMemKb = (endMem - startMem) / (1024);
    engine.flush();
    double cpuTime = static_cast<double>(cpuEnd - cpuStart) / CLOCKS_PER_SEC;
    std::chrono::duration<double> diff = end - start;

    std::cout << "Wall-clock Time: " << diff.count() << " seconds\n";
    std::cout << "CPU Time: " << cpuTime << " seconds\n";
    std::cout << "Memory usage: " << difMemKb << " Kb\n";
    std::cout << "==================================================================\n";

    csvFormatData.append(formatCsvString(buffer, indexStorageType, filter, diff, cpuTime,  difMemKb));

}

void saveEngineTestCsvToFile() {
    std::ofstream outFile("testResultEngine.csv");

    outFile <<"wallClockTime;cpuTime;memory;buffer;storage;filter\n" + csvFormatData;
    outFile.close();

    std::cout << "Test results saved :)\n";
}

void runEngieTestSearch(std::string dataPath, EngineFocus engineFocus, FilterType filterFlags,
                        IndexStorageType indexStorageType, size_t buffer){
    std::unordered_map<std::string, std::string> fileContents;
    std::string filter = filterTypeToString(filterFlags);

    std::cout << "============================== Test ==============================\n";
    std::cout << "Name: search time\n";
    std::cout << "Buffer - " << buffer << " bytes | indexStorage - " << printIndStorageType(indexStorageType)
              << std::endl;

    std::cout << "Filters - " << filter << std::endl;


    for (const auto &entry: fs::directory_iterator(dataPath)) {
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
            file.close();
        }
    }

    Engine engine(engineFocus, filterFlags, indexStorageType, buffer);

    for (auto &[filename, text]: fileContents) {
        engine.proceed(text, filename);
    }

    std::vector<std::string> words = {
            "дом", "работа", "вода", "а", "и",    // Часто употребительные русские слова
            "фаэтон", "тавтология", "ренессанс", "эпатаж", "иллюзия",  // Редко употребительные русские слова
            "house", "work", "water", "a", "and",  // Часто употребительные английские слова
            "obfuscate", "nefarious", "ineffable", "ephemeral", "verisimilitude"  // Редко употребительные английские слова
    };

    for (auto &w : words) {
        auto start = std::chrono::high_resolution_clock::now();
        size_t startMem = getCurrenMemoryUsage();

        engine.find(w);

        auto end = std::chrono::high_resolution_clock::now();
        size_t endMem = getCurrenMemoryUsage();

        size_t difMemKb = (endMem - startMem) / (1024);
        std::chrono::duration<double> diff = end - start;

        std::cout << w << " - wall-clock: "<< diff.count() << ", memory: " << difMemKb << " Kb\n";

    }

}