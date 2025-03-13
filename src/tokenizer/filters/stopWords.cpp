#include "StopWords.h"
#include <fstream>
#include <filesystem>
#include <string>

// Используем пространство имён для удобства работы с путями
namespace fs = std::filesystem;

// Инициализация статических полей
std::unordered_set<std::string> StopWords::stopWordsRu;
std::unordered_set<std::string> StopWords::stopWordsEn;
bool StopWords::initialized = false;

void StopWords::initialize() {
    if (initialized)
        return;

    // Определяем путь к каталогу: два уровня вверх от этого файла
    fs::path currentFilePath(__FILE__);
    fs::path folderPath = currentFilePath.parent_path().parent_path();
    
    // Формируем пути к файлам стоп-слов
    fs::path ruPath = folderPath / "filters" / "stop_words_ru.txt";
    fs::path enPath = folderPath / "filters" / "stop_words_en.txt";

    // Функция для "очистки" строки от пробельных символов по краям
    auto trim = [](const std::string &s) -> std::string {
        const std::string whitespace = " \t\n\r";
        size_t start = s.find_first_not_of(whitespace);
        if (start == std::string::npos)
            return "";
        size_t end = s.find_last_not_of(whitespace);
        return s.substr(start, end - start + 1);
    };

    // Загрузка русского списка стоп-слов
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
    // Загрузка английского списка стоп-слов
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

std::string StopWords::process(std::string& token) {
    if (!initialized)
        initialize();
    // Если токен присутствует хотя бы в одном из наборов стоп-слов,
    // возвращаем пустую строку (аналог None в Python)
    if (stopWordsEn.find(token) != stopWordsEn.end() ||
        stopWordsRu.find(token) != stopWordsRu.end()) {
        return "";
    }
    return token;
}
