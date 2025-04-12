#include "stopWords.h"
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

std::unordered_set<std::string> StopWords::stopWordsRu;
std::unordered_set<std::string> StopWords::stopWordsEn;
bool StopWords::initialized = false;

void StopWords::initialize() {
    if (initialized)
        return;

    fs::path currentFilePath(__FILE__);
    fs::path folderPath = currentFilePath.parent_path().parent_path();
    
    fs::path ruPath = folderPath / "filters" / "stop_words_ru.txt";
    fs::path enPath = folderPath / "filters" / "stop_words_en.txt";

    auto trim = [](const std::string &s) -> std::string {
        const std::string whitespace = " \t\n\r";
        size_t start = s.find_first_not_of(whitespace);
        if (start == std::string::npos)
            return "";
        size_t end = s.find_last_not_of(whitespace);
        return s.substr(start, end - start + 1);
    };

    std::ifstream fileRu(ruPath);
    if (fileRu.is_open()) {
        std::string line;
        while (std::getline(fileRu, line)) {
            std::string word = trim(line);
            if (!word.empty())
                stopWordsRu.insert(word);
        }
        fileRu.close();
    }
    std::ifstream fileEn(enPath);
    if (fileEn.is_open()) {
        std::string line;
        while (std::getline(fileEn, line)) {
            std::string word = trim(line);
            if (!word.empty())
                stopWordsEn.insert(word);
        }
        fileEn.close();
    }
    initialized = true;
}

void StopWords::process(std::string &token) {
    if (!initialized)
        initialize();
    if (stopWordsEn.find(token) != stopWordsEn.end() ||
        stopWordsRu.find(token) != stopWordsRu.end())
        token.clear();
}
