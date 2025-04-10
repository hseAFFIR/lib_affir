#include "englishStemmer.h"
#include <algorithm>
#include <regex>
#include <cctype>
#include <iostream>


// Вспомогательные функции

bool EnglishStemmer::ends_with(const std::string &s, const std::string &suffix) {
    return s.size() >= suffix.size() &&
           s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool EnglishStemmer::contains_any(const std::string &s, const std::string &chars) {
    for (char c : s)
        if (chars.find(c) != std::string::npos)
            return true;
    return false;
}

std::string EnglishStemmer::to_lower(const std::string &s) {
    std::string res = s;
    std::transform(res.begin(), res.end(), res.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return res;
}

// Конструктор – инициализирует константы
EnglishStemmer::EnglishStemmer()
    : english_vowels("aeiouy"),
      english_doubles({"bb", "dd", "ff", "gg", "mm", "nn", "pp", "rr", "tt"}),
      english_valid_li_endings("cdeghkmnrt"),
      english_exceptions1({
          {"skies", "sky"}, {"dying", "die"}, {"lying", "lie"},
          {"tying", "tie"}, {"idly", "idl"}, {"gently", "gentl"}, {"ugly", "ugli"},
          {"early", "earli"}, {"only", "onli"}, {"singly", "singl"}, {"sky", "sky"},
          {"news", "news"}, {"howe", "howe"}, {"atlas", "atlas"}, {"cosmos", "cosmos"},
          {"bias", "bias"}, {"andes", "andes"}
      }),
      english_exceptions2({"inning", "outing", "herring", "canning", "earring",
                            "evening", "proceed", "exceed", "succeed"}) {}

// Определение регионов R1 и R2 согласно алгоритму
std::pair<long long, long long> EnglishStemmer::english_set_regions(const std::string &word) const {
    long long wordSize = word.size();
    long long R1 = -1, R2 = -1;
    // Находим R1: первая позиция, где символ не-гласный следует за гласным
    for (long long i = 1; i < wordSize; ++i) {
        if (english_vowels.find(word[i]) == std::string::npos &&
            english_vowels.find(word[i - 1]) != std::string::npos) {
            R1 = (i + 1 < wordSize) ? (i + 1) : -1; 
            break;
        }
    }
    // Особые случаи
    if (word.rfind("commun", 0) == 0) {
        R1 = (wordSize > 6) ? 6 : -1;
    }
    if (word.rfind("arsen", 0) == 0 || word.rfind("gener", 0) == 0) {
        R1 = (wordSize > 5) ? 5 : -1;
    }
    // Находим R2 в пределах R1
    if (R1 != -1) {
        for (long long i = R1; i < wordSize; ++i) {
            if (english_vowels.find(word[i]) == std::string::npos &&
                english_vowels.find(word[i - 1]) != std::string::npos) {
                R2 = (i + 1 < wordSize) ? (i + 1) : -1;
                break;
            }
        }
    }
    return {R1, R2};
}

// Реализация шага замены суффиксов (часть шагов 0, 1a и 1c)
std::string EnglishStemmer::english_replace_suffixes(std::string &res) const {

    // Шаг 0: удаляем окончания "'s" или "'s'"
    if (res.size() >= 3 && ends_with(res, "'s'"))
        res.resize(res.size() - 3);
    else if (res.size() >= 2 && ends_with(res, "'s"))
        res.resize(res.size() - 2);

    // Обновляем регионы
    auto regions = english_set_regions(res);

    // Шаг 1a: заменяем "sses" на "ss"
    if (ends_with(res, "sses")){
        res.replace(res.size()-4, 4, "ss");
        regions = english_set_regions(res);
    }

    // Шаг 1a: обрабатываем окончания "ied" и "ies"
    if (ends_with(res, "ied") || ends_with(res, "ies")) {
        // Если основа слова длиннее 1 символа, заменяем на "i", иначе – на "ie"
        if (res.size() - 3 > 1){
            res.replace(res.size()-3, 3, "i");
            regions = english_set_regions(res);
        }
        else{
            res.replace(res.size()-3, 3, "ie");
            regions = english_set_regions(res);
        }
    }
    

    // Шаг 1a: если слово оканчивается на "s", но не на "us" или "ss"
    if (ends_with(res, "s") && !ends_with(res, "us") && !ends_with(res, "ss")) {
        std::string preceding = res.substr(0, res.size() - 1);
        if (preceding.size() > 1 && contains_any(preceding.substr(0, preceding.size() - 1), english_vowels)) {
            res = preceding;
            regions = english_set_regions(res);
        }
    }

    // Если слово входит во второй список исключений, возвращаем его без изменений
    if (english_exceptions2.find(res) != english_exceptions2.end())
        return res;

    // Шаг 1b: обработка окончаний "eedly" и "eed"
    std::vector<std::string> suffixes1 = {"eedly", "eed"};
    for (const auto &suff : suffixes1) {
        if (ends_with(res, suff)) {
            // Определяем позицию начала R1
            long long r1_start = regions.first;
            if (res.size() >= suff.size() && (res.size() - suff.size() >= r1_start)) {
                res.replace(res.size() - suff.size(), suff.size(), "ee");
                regions = english_set_regions(res);
            }
            if (res.size() > 2 &&
                (res.back() == 'y' || res.back() == 'Y') &&
                (english_vowels.find(res[res.size() - 2]) == std::string::npos)) {
                res.back() = 'i';
            }
            return res;
        }
    }

    // Шаг 1b: обработка окончаний "edly", "ingly", "ing", "ed"
    std::vector<std::string> suffixes2 = {"edly", "ingly", "ing", "ed"};
    for (const auto &suff : suffixes2) {
        if (ends_with(res, suff)) {
            std::string stem = res.substr(0, res.size() - suff.size());
            if (contains_any(stem, english_vowels)) {
                res = stem;
                regions = english_set_regions(res);
                std::string syllable = (res.size() < 3) ? res : res.substr(res.size() - 3);
                std::string lastTwo = res.substr(res.size() - 2);
                bool isDouble = false;
                for (const auto &dbl : english_doubles) {
                    if (lastTwo == dbl) {
                        isDouble = true;
                        break;
                    }
                }
                if (ends_with(res, "at") || ends_with(res, "bl") || ends_with(res, "iz"))
                    res += "e";
                if (isDouble && res.size() > 2) {
                    // Если буква перед двойным окончанием не является a, e или o
                    std::string aeo = res.substr(0, res.size() - 2);
                    if (std::string("aeo").find(aeo) == std::string::npos)
                        res.resize(res.size() - 1);
                }
                else if (res.size() > 1 && !isDouble && english_is_short(syllable, regions.first))
                    res += "e";
                
                break;
            }
        }
    }

    // Шаг 1c: если слово оканчивается на "y" или "Y" после согласной, заменяем на "i"
    if (res.size() > 2 &&
        (res.back() == 'y' || res.back() == 'Y') &&
        (english_vowels.find(res[res.size() - 2]) == std::string::npos)) {
        res.back() = 'i';
    }

    return res;
}

bool EnglishStemmer::english_is_short(const std::string &word, const long long &R1) const {
    return english_is_short_syllable(word) && (R1 == -1);
}

bool EnglishStemmer::english_is_short_syllable(const std::string &word) const {
    if (word.size() == 2 &&
        english_vowels.find(word[0]) != std::string::npos &&
        english_vowels.find(word[1]) == std::string::npos)
        return true;
    if (word.size() < 3)
        return false;
    if ( (english_vowels.find(word[word.size() - 3]) == std::string::npos) &&
         (english_vowels.find(word[word.size() - 2]) != std::string::npos) &&
         (english_vowels.find(word[word.size() - 1]) == std::string::npos) &&
         (std::string("wxyY").find(word[word.size() - 1]) == std::string::npos) )
        return true;
    return false;
}

std::string EnglishStemmer::step2(std::string &res, const long long &R1) const {
    if (R1 != -1) {
        // Список суффиксов и замен
        std::vector<std::pair<std::string, std::string>> suffixes = {
            {"ational", "ate"}, {"tional", "tion"}, {"enci", "ence"}, {"anci", "ance"},
            {"abli", "able"},    {"entli", "ent"}, {"izer", "ize"}, {"ization", "ize"},
            {"ation", "ate"},    {"ator", "ate"},  {"alism", "al"}, {"aliti", "al"},
            {"alli", "al"},      {"fulness", "ful"}, {"ousli", "ous"}, {"ousness", "ous"},
            {"iveness", "ive"},  {"iviti", "ive"}, {"biliti", "ble"}, {"bli", "ble"},
            {"ogi", "og"},       {"fulli", "ful"}, {"lessli", "less"}, {"li", ""}
        };
        for (const auto &p : suffixes) {
            const std::string &suff = p.first;
            const std::string &repl = p.second;
            if (ends_with(res, suff)) {
                if (res.size() - suff.size() >= R1) {
                    if (suff == "ogi" && res.size() > 3 && res[res.size() - 4] == 'l') {
                        res.replace(res.size() - suff.size(), suff.size(), repl);
                    }
                    else if (suff == "li" && res.size() > 2 &&
                             english_valid_li_endings.find(res[res.size() - 3]) == std::string::npos) {
                        break;
                    }
                    else {
                        res.replace(res.size() - suff.size(), suff.size(), repl);
                    }
                    break;
                }
                else {
                    break;
                }
            }
        }
    }
    return res;
}

std::string EnglishStemmer::step3(std::string &res, const long long &R1, const long long &R2) const {
    if (R1 != -1) {
        std::vector<std::pair<std::string, std::string>> suffixes = {
            {"ational", "ate"}, {"tional", "tion"}, {"alize", "al"}, {"icate", "ic"},
            {"iciti", "ic"},    {"ical", "ic"},   {"ful", ""},   {"ness", ""},
            {"ative", ""}
        };
        for (const auto &p : suffixes) {
            const std::string &suff = p.first;
            const std::string &repl = p.second;
            if (ends_with(res, suff)) {
                if (res.size() - suff.size() >= R1) {
                    if (suff == "ative") {
                        if (res.size() - suff.size() >= R2)
                            res.replace(res.size() - suff.size(), suff.size(), repl);
                        break;
                    }
                    res.replace(res.size() - suff.size(), suff.size(), repl);
                    break;
                }
                else {
                    break;
                }
            }
        }
    }
    return res;
}

std::string EnglishStemmer::step4(std::string &res, const long long &R2) const {
    std::vector<std::string> suffixes = {"al", "ance", "ence", "er", "ic", "able",
                                         "ible", "ant", "ement", "ment", "ent", "ism",
                                         "ate", "iti", "ous", "ive", "ize", "ion"};
    for (const auto &suff : suffixes) {
        if (ends_with(res, suff)) {
            if (res.size() - suff.size() >= R2) {
                if (suff == "ion" && res.size() > 3 &&
                    (res[res.size() - 4] == 's' || res[res.size() - 4] == 't')) {
                    res.resize(res.size() - 3);
                }
                else if (suff != "ion") {
                    res.resize(res.size() - suff.size());
                }
            }
            break;
        }
    }
    return res;
}

std::string EnglishStemmer::step5(std::string &res, const long long &R1, const long long &R2) const {
    bool e_flag = false, l_flag = false;
    if (!res.empty()) {
        if (res.back() == 'e')
            e_flag = true;
        else if (res.back() == 'l')
            l_flag = true;
    }
    if (e_flag) {
        if (res.size() - 1 >= R2) {
            res.resize(res.size() - 1);
        }
        else if (res.size() - 1 >= R1 && !english_is_short_syllable(res.substr(0, res.size() - 1))) {
            res.resize(res.size() - 1);
        }
    }
    if (l_flag && res.size() > 1 && res[res.size() - 2] == 'l') {
        if (res.size() - 1 >= R2)
            res.resize(res.size() - 1);
    }
    return res;
}

std::string EnglishStemmer::process(const std::string &inputWord) const {
    std::string word = inputWord;
    word = to_lower(word);
    // Если слово присутствует в исключениях – возвращаем его преобразование
    if (english_exceptions1.find(inputWord) != english_exceptions1.end())
        return english_exceptions1.at(inputWord);

    if (!word.empty() && word[0] == 'y')
        word[0] = 'Y';

    // Замена строчных "y" на "Y", если они после гласной
    for (long long i = 1; i < word.size(); ++i) {
        if ((word[i] == 'y') && (english_vowels.find(word[i - 1]) != std::string::npos))
            word[i] = 'Y';
    }

    word = english_replace_suffixes(word);
    auto regions = english_set_regions(word);
    long long R1 = regions.first;
    long long R2 = regions.second;

    word = step2(word, R1);
    regions = english_set_regions(word);
    R1 = regions.first;
    R2 = regions.second;

    word = step3(word, R1, R2);
    regions = english_set_regions(word);
    R1 = regions.first;
    R2 = regions.second;

    word = step4(word, R2);
    regions = english_set_regions(word);
    R1 = regions.first;
    R2 = regions.second;

    word = step5(word, R1, R2);
    return to_lower(word);
}
