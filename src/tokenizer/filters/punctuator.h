#ifndef LIB_AFFIR_PUNCTUATOR_H
#define LIB_AFFIR_PUNCTUATOR_H

#include "base.h"
#include <cctype>

class Punctuator : public Base {
public:
    std::string process(const std::string &token) const override {}
};

#endif // LIB_AFFIR_PUNCTUATOR_H