#ifndef LIB_AFFIR_LOWERCASER_H
#define LIB_AFFIR_LOWERCASER_H

#include "base.h"
#include <algorithm>
#include <cctype>
#include <unordered_map>

/**
 * @brief The Lowercaser class converts all characters of a token to lowercase.
 *
 * This filter inherits from Base and implements the process method to transform the input token
 * so that all its characters are in lowercase. It is typically used to normalize text before further processing.
 */
class Lowercaser : public Base {
public:
    /**
     * @brief Converts the input token to lowercase.
     *
     * This method creates a copy of the input token and applies std::transform along with std::tolower
     * to convert each character to its lowercase equivalent.
     *
     * @param token The input token to be processed.
     * @return std::string The resulting token with all characters converted to lowercase.
     */
    void process(std::string& token) override;

    /**
     * @brief Returns the processing order for the Lowercaser filter.
     *
     * This method specifies the order in which the Lowercaser filter should be applied relative to other filters.
     *
     * @return FilterOrder The order value corresponding to Lowercaser.
     */
    FilterOrder getOrder() const override {
        return FilterOrder::Lowercaser;
    }

private:
    const std::unordered_map<std::string, std::string> lower_map = {
        {"А", "а"}, {"Б", "б"}, {"В", "в"}, {"Г", "г"}, {"Д", "д"},
        {"Е", "е"}, {"Ё", "ё"}, {"Ж", "ж"}, {"З", "з"}, {"И", "и"},
        {"Й", "й"}, {"К", "к"}, {"Л", "л"}, {"М", "м"}, {"Н", "н"},
        {"О", "о"}, {"П", "п"}, {"Р", "р"}, {"С", "с"}, {"Т", "т"},
        {"У", "у"}, {"Ф", "ф"}, {"Х", "х"}, {"Ц", "ц"}, {"Ч", "ч"},
        {"Ш", "ш"}, {"Щ", "щ"}, {"Ъ", "ъ"}, {"Ы", "ы"}, {"Ь", "ь"},
        {"Э", "э"}, {"Ю", "ю"}, {"Я", "я"},
        {"\xD0\x90", "\xD0\xB0"},
        {"\xD0\x81", "\xD1\x91"},
        {"\xD0\xA3", "\xD1\x83"} 
    };

};

#endif // LIB_AFFIR_LOWERCASER_H