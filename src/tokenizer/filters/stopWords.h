#ifndef LIB_AFFIR_STOPWORDS_H
#define LIB_AFFIR_STOPWORDS_H

#include "base.h"
#include <string>
#include <unordered_set>

class StopWords : public Base {
public:
    std::string process(const std::string &token) const override {}    
private:
    // Статические контейнеры для стоп-слов
    static std::unordered_set<std::string> stopWordsRu;
    static std::unordered_set<std::string> stopWordsEn;
    // Флаг инициализации
    static bool initialized;
    // Метод для загрузки стоп-слов из файлов
    static void initialize();
};

#endif // LIB_AFFIR_STOPWORDS_H
