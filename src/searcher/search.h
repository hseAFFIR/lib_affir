#ifndef SEARCH_H
#define SEARCH_H

#include "../tokenizer/tokenizer.h"
#include "../storages/indexes/multi/multiFileIndexStorage.h"
#include "../indexer/indexer.h"
#include <vector>
#include <string>
#include <unordered_set>
#include <sstream>



struct WordMatch {
    std::string word;
    size_t absolutePos;
    size_t wordSequencePos;
    int fileId;
};

struct PhraseMatch {
    std::vector<WordMatch> words;
    int fileId;
};

class PhraseSearcher {
public:
    PhraseSearcher(MultiFileIndexStorage& storage, Indexer& indexer);

    std::vector<WordMatch> searchWord(const std::string& word);
    std::vector<PhraseMatch> searchPhrase(const std::string& phrase);

private:
    MultiFileIndexStorage& storage;
    Indexer& indexer;

    std::vector<std::string> splitIntoWords(const std::string& phrase);
    bool areWordsConsecutive(const std::vector<WordMatch>& words);
};
#endif