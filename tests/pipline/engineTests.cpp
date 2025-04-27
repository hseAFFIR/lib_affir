#include "../tests.h"

std::string csvFormatData;

void runEngineTestFile(std::string dataPath, EngineFocus engineFocus, FilterType filterFlags,
                   IndexStorageType indexStorageType, const size_t buffer) {
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

    auto start = std::chrono::high_resolution_clock::now();
    size_t startMem = getCurrenMemoryUsage();

    Engine engine(engineFocus, filterFlags, indexStorageType, buffer);

    for (auto &filename: filenames) {
        engine.proceed(filename);
    }

    auto end = std::chrono::high_resolution_clock::now();
    size_t endMem = getCurrenMemoryUsage();

    size_t difMemKb = (endMem - startMem) / (1024);
    engine.flush();
    std::chrono::duration<double> diff = end - start;

    std::cout << "Time:"
              << diff.count() << " seconds\n";
    std::cout << "Memory usage: " << difMemKb << " Kb\n";
    std::cout << "==================================================================\n";

    csvFormatData.append(formatCsvString(buffer, indexStorageType, filter, diff, difMemKb));

}


void runEngineTestString(std::string dataPath, EngineFocus engineFocus, FilterType filterFlags,
                           IndexStorageType indexStorageType, const size_t buffer) {
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

    auto start = std::chrono::high_resolution_clock::now();
    size_t startMem = getCurrenMemoryUsage();

    Engine engine(engineFocus, filterFlags, indexStorageType, buffer);

    for (auto &[filename, text]: fileContents) {
        engine.proceed(text, filename);
    }

    auto end = std::chrono::high_resolution_clock::now();
    size_t endMem = getCurrenMemoryUsage();

    size_t difMemKb = (endMem - startMem) / (1024);
    engine.flush();
    std::chrono::duration<double> diff = end - start;

    std::cout << "Time:"
              << diff.count() << " seconds\n";
    std::cout << "Memory usage: " << difMemKb << " Kb\n";
    std::cout << "==================================================================\n";

    csvFormatData.append(formatCsvString(buffer, indexStorageType, filter, diff, difMemKb));

}

void saveCsvToFile() {
    std::ofstream outFile("testResult.csv");

    outFile << csvFormatData;
    outFile.close();

    std::cout << "Test results saved :)"
}