//
// Created by okosh on 14.03.2025.
//

#ifndef BASE_H
#define BASE_H
#include <string>


class base {
public:
    virtual ~base() = default; // Виртуальный деструктор

    // Bиртуальный метод process
    virtual std::string process(const std::string& token) const = 0;

};



#endif //BASE_H

