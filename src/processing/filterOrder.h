#ifndef LIB_AFFIR_FILTERORDER_H
#define LIB_AFFIR_FILTERORDER_H

enum class FilterOrder {
    Lowercaser = 1,
    Htmler = 2,
    Punctuator = 3,
    StopWords = 4,
    StemFilter = 5,
    EnglishStemmer = 6,
    RussianPorterStemmer = 7
};

#endif // LIB_AFFIR_FILTERORDER_H
