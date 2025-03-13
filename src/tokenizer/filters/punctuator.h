#ifndef LIB_AFFIR_PUNCTUATOR_H
#define LIB_AFFIR_PUNCTUATOR_H

#include "base.h"
#include <cctype>

class Punctuator : public Base {
public:
    std::string process(std::string& token) override {}
};

#endif // LIB_AFFIR_PUNCTUATOR_H