#include "../tests.h"

/**
 * @brief Тест на время для пайплайна обработки без учета загрузки файлов и fileStorage
 *
 * @param folderPath path to test data
 * @param buffer Maximum buffer size (in bytes) before flushing to storage.
 * @param filters фильтры для токенизатора
 * @param storage Instance of object MFIS or SFIS
 */
void runTestTokIndIndStor(const std::string &folderPath, size_t buffer, std::vector<Base *> &filters,
                          IIndexStorage &storage) {
    // Загружаем все файлы заранее, чтобы не учитывать время чтения
    std::unordered_map<std::string, std::string> fileContents;

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

    FileId id = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (const auto &[filename, text]: fileContents) {
        Tokenizer tk(filters);
        Indexer ind(buffer, storage);

        tk.tokenize(text, [&ind](Token token) {

            ind.addToken(token);
        }, id);

        ind.saveTo();
        storage.saveStorageMeta();

        id++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "Total Tokenizer+Indexer+IndexStorage time (buffer - " << buffer << " bytes): "
              << diff.count() << " seconds\n";

}
