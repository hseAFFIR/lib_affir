// tokenizer.h
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <string>
#include <functional>
#include "filters/base.h"
#include "../models/token.h"

class Tokenizer {
public:
    // Конструктор с передачей владения фильтрами
    explicit Tokenizer(const std::vector<Base*>& filters);
    
    // Итератор для пошаговой обработки
    class Iterator {
    public:
        Iterator(Tokenizer* tokenizer, std::string buffer)
            : tokenizer_(tokenizer), buffer_(std::move(buffer)) {}
            
        Token operator*() const { return current_token_; }
        
        Iterator& operator++() {
            current_token_ = next_token_();
            return *this;
        }
        
        bool operator!=(const Iterator& other) const {
            return current_pos_ < buffer_.size();
        }
        
    private:
        Token next_token_() {
            // Реализация логики токенизации
            // ...
            return Token();
        }
        
        Tokenizer* tokenizer_;
        std::string buffer_;
        size_t current_pos_ = 0;
        Token current_token_;
    };

    // Функция для использования в data_handler, стоило бы переехать в .cpp
    void tokenize(const std::string& buffer, 
                std::function<void(Token)> add_token) 
    {
        for(auto it = begin(buffer); it != end(); ++it) {
            add_token(*it);
        }
    }

    Iterator begin(const std::string& buffer) {
        return Iterator(this, buffer);
    }
    
    Iterator end() const { return Iterator(nullptr, ""); }

private:
    std::vector<Base*> filters;
};
#endif