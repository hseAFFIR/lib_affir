#ifndef LIB_AFFIR_BASE_H
#define LIB_AFFIR_BASE_H

#include <string>
#include "..\..\models\token.h"

class Base {
    public:
        virtual std::string process(std::string& token) = 0;
        virtual ~Base() = default;
};

#endif // LIB_AFFIR_BASE_H