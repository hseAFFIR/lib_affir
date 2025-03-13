#ifndef LIB_AFFIR_LOWERCASER_H
#define LIB_AFFIR_LOWERCASER_H

#include "base.h"
#include <algorithm>
#include <cctype>

class Lowercaser : public Base {
public:
    std::string process(std::string& token) override {}
};

#endif // LIB_AFFIR_LOWERCASER_H