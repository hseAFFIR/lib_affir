//
// Created by okosh on 14.03.2025.
//

#include "russianPorterStemmer.h"
#include "../../logger/logger.h"
#include <algorithm>
#include <regex>
#include "lowercaser.h"

bool endsWith(const std::string& str, const std::string& suffix) {
    if (suffix.size() > str.size()) return false;
    return std::equal(str.rbegin(), str.rbegin() + suffix.size(), suffix.rbegin());
}


void replace_letter(std::string& word) {
    for (size_t i = 0; i < word.length() - 1; ++i) {
        // Ищем UTF-8 символ 'ё' (0xD1 0x91)
        if ((unsigned char)word[i] == 0xD1 && (unsigned char)word[i+1] == 0x91) {
            // Заменяем на 'е' (0xD0 0xB5)
            word[i] = (char)0xD0;
            word[i+1] = (char)0xB5;
        }
    }
}
bool isVowel(const std::string& text, size_t index) {
    // Проверяем корректность индекса (учитываем, что символы могут быть многобайтовыми)
    if (index >= text.size()) return false; // Если индекс выходит за пределы строки

    unsigned char first = text[index];

    // Если первый байт в пределах кириллической кодировки
    if (first == 0xD0 || first == 0xD1) {
        unsigned char second = text[index + 1];

        // Гласные в кириллице
        if ((first == 0xD0) & (second==0xB0 || second == 0xB5 || second == 0xB8 ||second == 0xBE)){ // для символов от 0xD0 до 0xD1
            return true;}
        if ((first == 0xD1) & (second == 0x83 || second == 0x8B || second == 0x8D || second == 0x8E || second == 0x8F)){
            return true;
        }
    }

    return false; // Если символ не является гласным, возвращаем false
}


// Конструктор – инициализирует константы
RussianPorterStemmer::RussianPorterStemmer()
    : vowels("аеиоуыэюя"),
      perfective_gerund_1({"в", "вши", "вшись"}),
      perfective_gerund_2({"ив", "ивши", "ившись", "ыв", "ывши", "ывшись"}),
      perfective_gerund({"в", "вши", "вшись", "ив", "ивши", "ившись", "ыв", "ывши", "ывшись"}),
      participle_1({"ем", "нн", "вш", "ющ", "щ"}),
      participle_2({"ивш", "ывш", "ующ"}),
      participle({"ем", "нн", "вш", "ющ", "щ", "ивш", "ывш", "ующ"}),
      adjective({
          "ее", "ие", "ые", "ое", "ими", "ыми", "ей", "ий", "ый", "ой", "ем", "им", "ым", "ом",
          "его", "ого", "ему", "ому", "их", "ых", "ую", "юю", "ая", "яя", "ою", "ею"
      }),
      reflexive({"ся", "сь"}),
      verb_1({
          "ла", "на", "ете", "йте", "ли", "й", "л", "ем", "н", "ло", "но", "ет", "ют",
          "ны", "ть", "ешь", "нно"
      }),
      verb_2({
          "ила", "ыла", "ена", "ейте", "уйте", "ите", "или", "ыли", "ей", "уй", "ил",
          "ыл", "им", "ым", "ен", "ило", "ыло", "ено", "ят", "ует", "уют", "ит", "ыт",
          "ены", "ить", "ыть", "ишь", "ую", "ю"
      }),
      verb({
          "ла", "на", "ете", "йте", "ли", "й", "л", "ем", "н", "ло", "но", "ет", "ют",
          "ны", "ть", "ешь", "нно", "ила", "ыла", "ена", "ейте", "уйте", "ите", "или", "ыли", "ей", "уй", "ил",
          "ыл", "им", "ым", "ен", "ило", "ыло", "ено", "ят", "ует", "уют", "ит", "ыт",
          "ены", "ить", "ыть", "ишь", "ую", "ю"
      }),
      noun({
          "а", "ев", "ов", "ие", "ье", "е", "иями", "ями", "ами", "еи", "ии", "и", "ией",
          "ей", "ой", "ий", "й", "иям", "ям", "ием", "ем", "ам", "ом", "о", "у", "ах",
          "иях", "ях", "ы", "ь", "ию", "ью", "ю", "ия", "ья", "я"
      }),
      superlative({"ейш", "ейше"}),
      derivational({"ост", "ость"}) {}
std::tuple<std::string, std::string, std::string> RussianPorterStemmer::russianSetRegions(std::string& token) {
    std::string rv, r1, r2;

    size_t tokenSize = token.size();

    // === RV: после первой гласной ===
    for (size_t i = 0; i + 1 < tokenSize; i += 2) {
        if (isVowel(token, i)) {
            rv = token.substr(i + 2);
            break;
        }
    }

    // === R1: после первой гласной + первой согласной ===
    for (size_t i = 0; i + 3 < tokenSize; i += 2) {
        if (isVowel(token, i) && !isVowel(token, i + 2)) {
            r1 = token.substr(i + 4);
            break;
        }
    }

    // === R2: то же самое, но в r1 ===
    for (size_t i = 0; i + 3 < r1.size(); i += 2) {
        if (isVowel(r1, i) && !isVowel(r1, i + 2)) {
            r2 = r1.substr(i + 4);
            break;
        }
    }

    return {rv, r1, r2};
}


std::string RussianPorterStemmer::step_1(std::string rv, std::string word) {
    bool ends_with_p_gerund = false;


    // Проверка окончаний на соответствие из perfective_gerund
    for (const auto& ending : perfective_gerund) {
        size_t endingSize = ending.size();
        size_t rvSize = rv.size();
        if (rvSize >= endingSize && rv.compare(rvSize - endingSize, endingSize, ending) == 0) {
            // Для первого типа совершенного деепричастия
            if (std::find(perfective_gerund_1.begin(), perfective_gerund_1.end(), ending) != perfective_gerund_1.end()) {

                if (endsWith(rv, "а" + ending) || endsWith(rv, "я" + ending)) {


                    word.replace(word.size() - endingSize, endingSize, "");
                    std::string r1, r2;
                    std::tie(rv, r1, r2) = russianSetRegions(word);  // Используем russianSetRegions как есть
                    ends_with_p_gerund = true;
                    break;
                }
            }
            // Для второго типа совершенного деепричастия
            if (std::find(perfective_gerund_2.begin(), perfective_gerund_2.end(), ending) != perfective_gerund_2.end()) {
                word.replace(word.size() - endingSize, endingSize, "");
                std::string r1, r2;
                std::tie(rv, r1, r2) = russianSetRegions(word);  // Используем russianSetRegions как есть
                ends_with_p_gerund = true;
                break;
            }
        }
    }

    // Если не найдено, применяем шаг 1 для других случаев
    if (!ends_with_p_gerund) {
        word = step_1_if(word);
    }

    return word;
}

std::string RussianPorterStemmer::step_1_if(std::string word) {
    std::string r1, r2, rv;
    std::tie(rv, r1, r2) = russianSetRegions(word);  // Используем russianSetRegions как есть

    // Проверка для рефлексивных окончаний
    for (const auto& ending : reflexive) {
        size_t endingSize = ending.size();
        size_t rvSize = rv.size();
        if (rvSize >= endingSize && rv.compare(rvSize - endingSize, endingSize, ending) == 0) {
            word.replace(word.size() - endingSize, endingSize, "");
            std::tie(rv, r1, r2) = russianSetRegions(word);  // Используем russianSetRegions как есть
            break;
        }
    }

    // Проверка для прилагательных
    for (const auto& ending : adjective) {
        size_t endingSize = ending.size();
        size_t rvSize = rv.size();
        if (rvSize >= endingSize && rv.compare(rvSize - endingSize, endingSize, ending) == 0) {
            word.replace(word.size() - endingSize, endingSize, "");
            std::tie(rv, r1, r2) = russianSetRegions(word);  // Используем russianSetRegions как есть

            // Проверка для причастий
            for (const auto& participle_ending : participle) {
                rvSize = rv.size();
                size_t participleEndingSize = participle_ending.size();
                if (rvSize >= participleEndingSize && rv.compare(rvSize - participleEndingSize, participleEndingSize, participle_ending) == 0) {
                    if (std::find(participle_1.begin(), participle_1.end(), participle_ending) != participle_1.end()) {

                        if (endsWith(rv, "а" + participle_ending) || endsWith(rv, "я" + participle_ending)) {


                            word.replace(word.size() - participleEndingSize, participleEndingSize, "");
                            std::tie(rv, r1, r2) = russianSetRegions(word);  // Используем russianSetRegions как есть
                            return word;
                        }
                    }
                    if (std::find(participle_2.begin(), participle_2.end(), participle_ending) != participle_2.end()) {
                        word.replace(word.size() - participleEndingSize, participleEndingSize, "");
                        std::tie(rv, r1, r2) = russianSetRegions(word);  // Используем russianSetRegions как есть
                        return word;
                    }
                }
            }
            return word;
        }
    }


    // Проверка для глагольных окончаний
    for (const auto& ending : verb) {
        size_t endingSize = ending.size();
        size_t rvSize = rv.size();
        if (rvSize >= endingSize && rv.compare(rvSize - endingSize, endingSize, ending) == 0) {
            if (std::find(verb_1.begin(), verb_1.end(), ending) != verb_1.end()) {

                if (endsWith(rv, "а" + ending) || endsWith(rv, "я" + ending)) {


                    word.replace(word.size() - endingSize, endingSize, "");
                    std::tie(rv, r1, r2) = russianSetRegions(word);  // Используем russianSetRegions как есть
                    return word;
                }
            }
            if (std::find(verb_2.begin(), verb_2.end(), ending) != verb_2.end()) {
                word.replace(word.size() - endingSize, endingSize, "");
                std::tie(rv, r1, r2) = russianSetRegions(word);  // Используем russianSetRegions как есть
                return word;
            }
        }
    }

    // Проверка для существительных
    for (const auto& ending : noun) {
        size_t endingSize = ending.size();
        size_t rvSize = rv.size();
        if (rvSize >= endingSize && rv.compare(rvSize - endingSize, endingSize, ending) == 0) {
            word.replace(word.size() - endingSize, endingSize, "");
            return word;
        }
    }

    return word;
}

void RussianPorterStemmer::process(std::string &word) {
    replace_letter(word);

    std::string rv, r1, r2;
    std::tie(rv, r1, r2) = russianSetRegions(word);

    // Step 1
    word = step_1(rv, word);
    std::tie(rv, r1, r2) = russianSetRegions(word);

    // Step 2
    size_t rvSize = rv.size();
    if (rvSize >= 2 && (unsigned char)rv[rvSize - 1] ==0xb8  && (unsigned char)rv[rvSize - 2]== 0xd0) {
        word.replace(word.size() - 2, 2, "");
        std::tie(rv, r1, r2) = russianSetRegions(word);
    }

    // Step 3
    for (const auto& ending : derivational) {
        size_t endingSize = ending.size();
        size_t r2Size = r2.size();
        if (r2Size >= endingSize && r2.compare(r2Size - endingSize, endingSize, ending) == 0) {
            word.replace(word.size() - endingSize, endingSize, "");
            std::tie(rv, r1, r2) = russianSetRegions(word);
            break;
        }
    }

    // Step 4
    for (const auto& ending : superlative) {
        size_t endingSize = ending.size();
        rvSize = rv.size();
        if (rvSize >= endingSize && rv.compare(rvSize - endingSize, endingSize, ending) == 0) {
            word.replace(word.size() - endingSize, endingSize, "");
            std::tie(rv, r1, r2) = russianSetRegions(word);
            break;
        }
    }

    // Если окончание "нн" или "ь"
    rvSize = rv.size();
    if (rvSize >= 4 &&
    (unsigned char)rv[rvSize - 4] == 0xD0 &&
    (unsigned char)rv[rvSize - 3] == 0xBD &&  // 'н'
    (unsigned char)rv[rvSize - 2] == 0xD0 &&
    (unsigned char)rv[rvSize - 1] == 0xBD) {  // 'н'

        word.replace(word.size() - 2, 2, "");  // Удаляем одну 'н' (двухбайтовый символ)
        std::tie(rv, r1, r2) = russianSetRegions(word);

    } else if (rvSize >= 2 &&
               (unsigned char)rv[rvSize - 2] == 0xD1 &&
               (unsigned char)rv[rvSize - 1] == 0x8C) {  // 'ь'

        word.replace(word.size() - 2, 2, "");  // Удаляем 'ь'
        std::tie(rv, r1, r2) = russianSetRegions(word);
               }
}


