#ifndef LIB_AFFIR_HTMLER_H
#define LIB_AFFIR_HTMLER_H

#include "base.h"

class Htmler : public Base {
public:
    std::string process(const std::string &token) const override {}
    
    FilterOrder getOrder() const override {
        return FilterOrder::Htmler;
    }
};

#endif // LIB_AFFIR_HTMLER_H