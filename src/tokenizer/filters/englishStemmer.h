//
// Created by okosh on 14.03.2025.
//

#ifndef ENGLISH_STEMMER_H
#define ENGLISH_STEMMER_H

#include "base.h"
#include <string>

class EnglishStemmer : public Base {
public:
    // Реализация метода process
    std::string process(const std::string &token) const override;
};

#endif // ENGLISH_STEMMER_H
