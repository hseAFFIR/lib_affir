#include "../tests.h"


const std::string indexDirPath = "index_files";
const std::string fileStorageMeta = "file_storage_metadata.bin";
const std::string fileStorage = "file_storage_raw";
void
runEngineTest(std::string dataPath, EngineFocus engineFocus, FilterType filterFlags, IndexStorageType indexStorageType,
              const size_t buffer) {

}


void runEngineSideLoadTest(std::string dataPath, EngineFocus engineFocus, FilterType filterFlags,
                           IndexStorageType indexStorageType, const size_t buffer) {
    try {
        // Удаление файлов
        if (fs::exists(fileStorageMeta)) {
            fs::remove(fs::path(fileStorageMeta));
            std::cout << "Deleted: " << fileStorageMeta << "\n";
        }

        if (fs::exists(fileStorage)) {
            fs::remove(fs::path(fileStorage));
            std::cout << "Deleted: " << fileStorage << "\n";
        }

        // Рекурсивное удаление директории
        if (fs::exists(indexDirPath)) {
            std::uintmax_t count = fs::remove_all(fs::path(indexDirPath));
            std::cout << "Deleted " << count << " items in: " << indexDirPath << "\n";
        }
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
    }


    std::unordered_map<std::string, std::string> fileContents;

    std::cout << "============================================================\n";
    std::cout << "Test: engine without file read time\n";
    std::cout << "Buffer - " << buffer << " bytes | indexStorage - " << printIndStorageType(indexStorageType)
              << std::endl;


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
    std::cout << "============================================================\n";

}