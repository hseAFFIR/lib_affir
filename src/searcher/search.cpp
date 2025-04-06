#include "search.h"
#include <algorithm>
#include <unordered_set>

PhraseSearcher::PhraseSearcher(MultiFileIndexStorage& storage, Indexer& indexer)
        : storage(storage), indexer(indexer) {}

std::vector<std::string> PhraseSearcher::splitIntoWords(const std::string& phrase) {
    std::vector<std::string> words;
    std::istringstream iss(phrase);
    std::string word;

    while (iss >> word) {
        if (!word.empty() && ispunct(word.back())) {
            word.pop_back();
        }
        if (!word.empty()) {
            words.push_back(word);
        }
    }
    return words;
}

bool PhraseSearcher::areWordsConsecutive(const std::vector<WordMatch>& words) {
    for (size_t i = 1; i < words.size(); ++i) {
        if (words[i].wordSequencePos != words[i-1].wordSequencePos + 1) {
            return false;
        }
    }
    return true;
}

std::vector<WordMatch> PhraseSearcher::searchWord(const std::string& word) {
    std::vector<WordMatch> results;
    BigToken token = indexer.getTokenInfo(word);

    for (const auto& [fileId, positions] : token.getFilePositions()) {
        for (const auto& posInfo : positions) {
            results.push_back({
                                      word,
                                      posInfo.pos,
                                      posInfo.wordPos,
                                      fileId
                              });
        }
    }

    std::sort(results.begin(), results.end(), [](const WordMatch& a, const WordMatch& b) {
        return std::tie(a.fileId, a.wordSequencePos) < std::tie(b.fileId, b.wordSequencePos);
    });

    return results;
}

std::vector<PhraseMatch> PhraseSearcher::searchPhrase(const std::string& phrase) {
    std::vector<PhraseMatch> results;
    auto words = splitIntoWords(phrase);

    if (words.size() < 2) {
        return results;
    }

    // 1. Собираем BigToken для каждого слова фразы
    std::vector<BigToken> phraseTokens;
    for (const auto& word : words) {
        phraseTokens.push_back(indexer.getTokenInfo(word));
    }

    // 2. Находим общие файлы для всех слов
    std::unordered_set<int> commonFiles;
    bool firstWord = true;

    for (const auto& token : phraseTokens) {
        std::unordered_set<int> currentFiles;
        for (const auto& [fileId, _] : token.getFilePositions()) {
            currentFiles.insert(fileId);
        }

        if (firstWord) {
            commonFiles = currentFiles;
            firstWord = false;
        } else {
            std::unordered_set<int> intersection;
            for (int fileId : commonFiles) {
                if (currentFiles.count(fileId)) {
                    intersection.insert(fileId);
                }
            }
            commonFiles = intersection;
        }

        if (commonFiles.empty()) break;
    }

    // 3. Проверяем последовательность в общих файлах
    for (int fileId : commonFiles) {
        // Для каждого слова собираем его позиции в файле
        std::vector<std::vector<WordMatch>> allWordPositions;

        for (size_t i = 0; i < phraseTokens.size(); ++i) {
            const auto& positions = phraseTokens[i].getFilePositions().at(fileId);
            std::vector<WordMatch> wordMatches;

            for (const auto& posInfo : positions) {
                wordMatches.push_back({
                                              words[i],
                                              posInfo.pos,
                                              posInfo.wordPos,
                                              fileId
                                      });
            }
            allWordPositions.push_back(wordMatches);
        }

        // 4. Ищем последовательные вхождения
        for (const auto& firstWordPos : allWordPositions[0]) {
            std::vector<WordMatch> potentialPhrase = {firstWordPos};
            bool isComplete = true;

            for (size_t i = 1; i < allWordPositions.size(); ++i) {
                bool found = false;
                size_t expectedPos = firstWordPos.wordSequencePos + i;

                for (const auto& wordPos : allWordPositions[i]) {
                    if (wordPos.wordSequencePos == expectedPos) {
                        potentialPhrase.push_back(wordPos);
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    isComplete = false;
                    break;
                }
            }

            if (isComplete) {
                results.push_back({
                                          potentialPhrase,
                                          fileId
                                  });
            }
        }
    }

    return results;
}