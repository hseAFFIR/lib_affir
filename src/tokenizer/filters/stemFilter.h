//
// Created by okosh on 14.03.2025.
//

#ifndef STEM_FILTER_H
#define STEM_FILTER_H

#include "base.h"
#include "russianPorterStemmer.h"
#include "englishStemmer.h"

class StemFilter : public Base {
private:
    RussianPorterStemmer russian_stemmer;
    EnglishStemmer english_stemmer;

    std::string detect_language(const std::string &token) const;

public:
    StemFilter();

    std::string process(const std::string &token) const override;
};

#endif // STEM_FILTER_H