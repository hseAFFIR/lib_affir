#include "searcher/search.h"
#include "tokenizer/tokenizer.h"
#include "logger/logger.h"
#include "storages/indexes/multi/multiFileIndexStorage.h"
#include "indexer/indexer.h"
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

    

    indexer.addToken(Token("not", 100, 1, 1));  // pos=100, wordPos=1, fileId=1
    indexer.addToken(Token("ivan", 104, 2, 1)); // pos=110, wordPos=2
    indexer.addToken(Token("carevich", 105, 3, 1));
    indexer.addToken(Token("cried", 106, 4, 1));

    indexer.addToken(Token("ivan", 4, 2, 2)); // pos=110, wordPos=2
    indexer.addToken(Token("ivan", 9, 3, 2)); // pos=110, wordPos=2
    
    indexer.addToken(Token("blow", 109, 10, 4)); // pos=110, wordPos=2
    indexer.addToken(Token("ye", 115, 11, 4)); // pos=110, wordPos=2
    indexer.addToken(Token("trumpet", 120,12, 4)); // pos=110, wordPos=2

    indexer.addToken(Token("thick", 104,12, 3)); // pos=110, wordPos=2


    indexer.saveTo();
    storage.saveMetadata();


    Search Search(indexer);
    std::vector<Search::SearchResult> results = Search.search("ivan");
    Search.printSearchResults(results);

std::vector<Search::SearchResult> results2 = Search.search("ivan carevich");
    Search.printSearchResults(results2);

// std::vector<Search::SearchResult> results3 = Search.search(""); // EXCEPTION!
    // Search.printSearchResults(results3);

    std::vector<Search::SearchResult> results4 = Search.search("ivan ivan"); // not found
    Search.printSearchResults(results4);

    // std::vector<Search::SearchResult> results5 = Search.search("русские."); // garbage error (fantom tokens) чзх?????
    // Search.printSearchResults(results5);



    // std::string searchText = "2. Blow ye the trumpet in Zion, and sound an alarm in my holy mountain: let all the inhabitants of the land tremble: for the day of the Lord cometh, for it is nigh at hand;2 A day of darkness and of gloominess, a day of clouds and of thick darkness, as the morning spread upon the mountains: a great people and a strong; there hath not been ever the like, neither shall be any more after it, even to the years of many generations.";
    std::string searchText = "blow ye";
    std::vector<Search::SearchResult> searchResults = Search.search(searchText);
    Search.printSearchResults(searchResults);


    // // пример того, что на русском токенизатор не работает.
    // std::string searchText = "курсовая работа";
    // Tokenizer tokenizer({});
    // FileId fileId = 1;

    // tokenizer.tokenizeRaw(searchText, fileId, [](Token token) {
    //     std::cout << "Token: " << token.getBody() << " | Pos: " << token.getPos() << std::endl;
    // });


    return 0;
}