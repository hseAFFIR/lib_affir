//
// Created by okosh on 14.03.2025.
//
#include "russianPorterStemmer.h"
#include <algorithm>
#include <regex>
#include "lowercaser.h"

/**
 * Проверяет, заканчивается ли строка указанным суффиксом.
 * @param str Исходная строка.
 * @param suffix Суффикс для проверки.
 * @return true, если строка заканчивается суффиксом, иначе false.
 */
bool endsWith(const std::string& str, const std::string& suffix) {
    if (suffix.size() > str.size()) return false;
    return std::equal(str.rbegin(), str.rbegin() + suffix.size(), suffix.rbegin());
}

/**
 * Заменяет букву 'ё' на 'е' в заданном слове.
 * @param word Слово для обработки.
 */
void replaceLetter(std::string& word) {
    for (size_t i = 0; i < word.length() - 1; ++i) {
        // Поиск UTF-8 символа 'ё' (0xD1 0x91)
        if ((unsigned char)word[i] == 0xD1 && (unsigned char)word[i+1] == 0x91) {
            // Замена на 'е' (0xD0 0xB5)
            word[i] = (char)0xD0;
            word[i+1] = (char)0xB5;
        }
    }
}

/**
 * Проверяет, является ли символ в указанной позиции гласной.
 * @param text Исходная строка.
 * @param index Индекс символа для проверки.
 * @return true, если символ является гласной, иначе false.
 */
bool isVowel(const std::string& text, size_t index) {
    // Проверка корректности индекса (учитывая многобайтовые символы)
    if (index >= text.size()) return false; // Если индекс выходит за пределы строки
    unsigned char first = text[index];
    // Если первый байт находится в диапазоне кириллической кодировки
    if (first == 0xD0 || first == 0xD1) {
        unsigned char second = text[index + 1];
        // Гласные в кириллице
        if ((first == 0xD0) & (second==0xB0 || second == 0xB5 || second == 0xB8 ||second == 0xBE)){
            return true;}
        if ((first == 0xD1) & (second == 0x83 || second == 0x8B || second == 0x8D || second == 0x8E || second == 0x8F)){
            return true;
        }
    }
    return false; // Если символ не является гласным, возвращаем false
}

/**
 * Конструктор – инициализирует константы.
 */
RussianPorterStemmer::RussianPorterStemmer()
    : vowels("аеиоуыэюя"),
      perfectiveGerund1({"в", "вши", "вшись"}),
      perfectiveGerund2({"ив", "ивши", "ившись", "ыв", "ывши", "ывшись"}),
      perfectiveGerund({"в", "вши", "вшись", "ив", "ивши", "ившись", "ыв", "ывши", "ывшись"}),
      participle1({"ем", "нн", "вш", "ющ", "щ"}),
      participle2({"ивш", "ывш", "ующ"}),
      participle({"ем", "нн", "вш", "ющ", "щ", "ивш", "ывш", "ующ"}),
      adjective({
          "ее", "ие", "ые", "ое", "ими", "ыми", "ей", "ий", "ый", "ой", "ем", "им", "ым", "ом",
          "его", "ого", "ему", "ому", "их", "ых", "ую", "юю", "ая", "яя", "ою", "ею"
      }),
      reflexive({"ся", "сь"}),
      verb1({
          "ла", "на", "ете", "йте", "ли", "й", "л", "ем", "н", "ло", "но", "ет", "ют",
          "ны", "ть", "ешь", "нно"
      }),
      verb2({
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

/**
 * Устанавливает регионы RV, R1 и R2 для стемминга.
 * @param token Исходное слово.
 * @param rv Регион RV для установки.
 * @param r1 Регион R1 для установки.
 * @param r2 Регион R2 для установки.
 */
void RussianPorterStemmer::setRussianRegions(std::string &token, std::string &rv, std::string &r1, std::string &r2) {
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
    // === R2: то же самое, но внутри R1 ===
    for (size_t i = 0; i + 3 < r1.size(); i += 2) {
        if (isVowel(r1, i) && !isVowel(r1, i + 2)) {
            r2 = r1.substr(i + 4);
            break;
        }
    }
}

/**
 * Первый шаг алгоритма стемминга.
 * @param word Исходное слово для обработки.
 * @return Обработанное слово после первого шага.
 */
void RussianPorterStemmer::step1(std::string & word) {
    bool endsWithPGerund = false;
    std::string rv, rv1, rv2;
    setRussianRegions(word, rv, rv1, rv2);
    // Проверка окончаний на соответствие совершенным деепричастиям
    for (const auto& ending : perfectiveGerund) {
        size_t endingSize = ending.size();
        size_t rvSize = rv.size();
        if (rvSize >= endingSize && rv.compare(rvSize - endingSize, endingSize, ending) == 0) {
            // Для первого типа совершенного деепричастия
            if (std::find(perfectiveGerund1.begin(), perfectiveGerund1.end(), ending) != perfectiveGerund1.end()) {
                if (endsWith(rv, "а" + ending) || endsWith(rv, "я" + ending)) {
                    word.replace(word.size() - endingSize, endingSize, "");
                    endsWithPGerund = true;
                    break;
                }
            }
            // Для второго типа совершенного деепричастия
            if (std::find(perfectiveGerund2.begin(), perfectiveGerund2.end(), ending) != perfectiveGerund2.end()) {
                word.replace(word.size() - endingSize, endingSize, "");
                endsWithPGerund = true;
                break;
            }
        }
    }
    // Если не найдено, применяем первый шаг для других случаев
    if (!endsWithPGerund) {
        step1If(word);
    }
}

/**
 * Подшаг первого шага для обработки специфических случаев.
 * @param word Исходное слово для обработки.
 * @return Обработанное слово после подшага первого шага.
 */
void RussianPorterStemmer::step1If(std::string & word) {
    std::string rv, r1, r2;
    setRussianRegions(word, rv, r1, r2);  // Используем setRussianRegions как есть
    // Проверка рефлексивных окончаний
    for (const auto& ending : reflexive) {
        size_t endingSize = ending.size();
        size_t rvSize = rv.size();
        if (rvSize >= endingSize && rv.compare(rvSize - endingSize, endingSize, ending) == 0) {
            word.replace(word.size() - endingSize, endingSize, "");
            setRussianRegions(word, rv, r1, r2);  // Используем setRussianRegions как есть
            break;
        }
    }
    // Проверка прилагательных
    for (const auto& ending : adjective) {
        size_t endingSize = ending.size();
        size_t rvSize = rv.size();
        if (rvSize >= endingSize && rv.compare(rvSize - endingSize, endingSize, ending) == 0) {
            word.replace(word.size() - endingSize, endingSize, "");
            setRussianRegions(word, rv, r1, r2);  // Используем setRussianRegions как есть
            // Проверка причастий
            for (const auto& participleEnding : participle) {
                rvSize = rv.size();
                size_t participleEndingSize = participleEnding.size();
                if (rvSize >= participleEndingSize && rv.compare(rvSize - participleEndingSize, participleEndingSize, participleEnding) == 0) {
                    if (std::find(participle1.begin(), participle1.end(), participleEnding) != participle1.end()) {
                        if (endsWith(rv, "а" + participleEnding) || endsWith(rv, "я" + participleEnding)) {
                            word.replace(word.size() - participleEndingSize, participleEndingSize, "");
                            setRussianRegions(word, rv, r1, r2);  // Используем setRussianRegions как есть
                            return;
                        }
                    }
                    if (std::find(participle2.begin(), participle2.end(), participleEnding) != participle2.end()) {
                        word.replace(word.size() - participleEndingSize, participleEndingSize, "");
                        setRussianRegions(word, rv, r1, r2);  // Используем setRussianRegions как есть
                        return;
                    }
                }
            }
            return;
        }
    }
    // Проверка глагольных окончаний
    for (const auto& ending : verb) {
        size_t endingSize = ending.size();
        size_t rvSize = rv.size();
        if (rvSize >= endingSize && rv.compare(rvSize - endingSize, endingSize, ending) == 0) {
            if (std::find(verb1.begin(), verb1.end(), ending) != verb1.end()) {
                if (endsWith(rv, "а" + ending) || endsWith(rv, "я" + ending)) {
                    word.replace(word.size() - endingSize, endingSize, "");
                    setRussianRegions(word, rv, r1, r2);  // Используем setRussianRegions как есть
                    return;
                }
            }
            if (std::find(verb2.begin(), verb2.end(), ending) != verb2.end()) {
                word.replace(word.size() - endingSize, endingSize, "");
                setRussianRegions(word, rv, r1, r2);  // Используем setRussianRegions как есть
                return;
            }
        }
    }
    // Проверка существительных
    for (const auto& ending : noun) {
        size_t endingSize = ending.size();
        size_t rvSize = rv.size();
        if (rvSize >= endingSize && rv.compare(rvSize - endingSize, endingSize, ending) == 0) {
            word.replace(word.size() - endingSize, endingSize, "");
            return;
        }
    }
}

/**
 * Обрабатывает слово через все шаги стемминга.
 * @param word Исходное слово для обработки.
 */
void RussianPorterStemmer::process(std::string &word) {
    replaceLetter(word);
    std::string rv, r1, r2;
    setRussianRegions(word, rv, r1, r2);
    // Шаг 1
    step1(word);
    setRussianRegions(word, rv, r1, r2);
    // Шаг 2
    size_t rvSize = rv.size();
    if (rvSize >= 2 && (unsigned char)rv[rvSize - 1] ==0xb8  && (unsigned char)rv[rvSize - 2]== 0xd0) {
        word.replace(word.size() - 2, 2, "");
        setRussianRegions(word, rv, r1, r2);
    }
    // Шаг 3
    for (const auto& ending : derivational) {
        size_t endingSize = ending.size();
        size_t r2Size = r2.size();
        if (r2Size >= endingSize && r2.compare(r2Size - endingSize, endingSize, ending) == 0) {
            word.replace(word.size() - endingSize, endingSize, "");
            setRussianRegions(word, rv, r1, r2);
            break;
        }
    }
    // Шаг 4
    for (const auto& ending : superlative) {
        size_t endingSize = ending.size();
        rvSize = rv.size();
        if (rvSize >= endingSize && rv.compare(rvSize - endingSize, endingSize, ending) == 0) {
            word.replace(word.size() - endingSize, endingSize, "");
            setRussianRegions(word, rv, r1, r2);
            break;
        }
    }
    // Обработка окончаний "нн" или "ь"
    rvSize = rv.size();
    if (rvSize >= 4 &&
    (unsigned char)rv[rvSize - 4] == 0xD0 &&
    (unsigned char)rv[rvSize - 3] == 0xBD &&  // 'н'
    (unsigned char)rv[rvSize - 2] == 0xD0 &&
    (unsigned char)rv[rvSize - 1] == 0xBD) {  // 'н'
        word.replace(word.size() - 2, 2, "");  // Удаляем одну 'н' (двухбайтовый символ)
        setRussianRegions(word, rv, r1, r2);
    } else if (rvSize >= 2 &&
               (unsigned char)rv[rvSize - 2] == 0xD1 &&
               (unsigned char)rv[rvSize - 1] == 0x8C) {  // 'ь'
        word.replace(word.size() - 2, 2, "");  // Удаляем 'ь'
        setRussianRegions(word, rv, r1, r2);
    }
}