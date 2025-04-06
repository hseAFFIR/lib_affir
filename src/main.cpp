#include "searcher/search.h"
#include "tokenizer/tokenizer.h"
#include "logger/logger.h"
#include <filesystem>
#include <iostream>

int main() {
    system("chcp 65001");
    Logger::init("logs/log.txt");
    Logger::info("Main", "Запуск");


    // удаление index_files (убрать!)
    std::string folderPath = "index_files";
    try {
        std::filesystem::remove_all(folderPath);
    } catch (const  std::filesystem::filesystem_error& e) {
        std::cerr << "Ошибка при удалении папки: " << e.what() << std::endl;
        return 1;
    }

    MultiFileIndexStorage storage;
    Indexer indexer(1024, storage);
    PhraseSearcher searcher(storage, indexer);


    indexer.addToken(Token("не", 100, 1, 1));  // pos=100, wordPos=1, fileId=1
    indexer.addToken(Token("иван", 110, 3, 1)); // pos=110, wordPos=2
    indexer.addToken(Token("ы", 200, 4, 1));
    indexer.addToken(Token("саша", 105, 2, 1));


    indexer.saveTo();
    storage.saveMetadata();


// Поиск слова
    auto wordResults = searcher.searchWord("иван");
    for (const auto& match : wordResults) {
        std::cout << "Found '" << match.word << "' in file " << match.fileId
                  << " at position " << match.absolutePos << std::endl;
    }

// Поиск фразы
    auto phraseResults = searcher.searchPhrase("саша иван");
    for (const auto& phrase : phraseResults) {
        std::cout << "Found phrase in file " << phrase.fileId << " at positions: ";
        for (const auto& word : phrase.words) {
            std::cout << word.absolutePos << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}