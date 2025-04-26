#ifndef LIB_AFFIR_FILTERORDER_H
#define LIB_AFFIR_FILTERORDER_H

namespace affir {
    enum class FilterOrder {
        Lowercaser,
        Htmler,
        Punctuator,
        StopWords,
        StemFilter,
        EnglishStemmer,
        RussianPorterStemmer
    };
}
#endif // LIB_AFFIR_FILTERORDER_H
