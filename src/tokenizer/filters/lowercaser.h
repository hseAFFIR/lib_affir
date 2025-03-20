#ifndef LIB_AFFIR_LOWERCASER_H
#define LIB_AFFIR_LOWERCASER_H

#include "base.h"
#include <algorithm>
#include <cctype>

class Lowercaser : public Base {
public:
    std::string process(const std::string& token) const override {}

    FilterOrder getOrder() const override {
        return FilterOrder::Lowercaser;
    }
};

#endif // LIB_AFFIR_LOWERCASER_H