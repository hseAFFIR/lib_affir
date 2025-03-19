//
// Created by okosh on 14.03.2025.
//

#ifndef RUSSIAN_PORTER_STEMMER_H
#define RUSSIAN_PORTER_STEMMER_H

#include "base.h"
#include <string>
#include <vector>

/**
 * @class RussianPorterStemmer
 * @brief Класс для стемминга русских слов с использованием алгоритма Портера.
 *
 * Этот класс реализует стемминг русского языка, удаляя суффиксы и окончания для приведения слов к их основам.
 */
class RussianPorterStemmer : public Base {
private:
    std::string vowels; ///< Гласные буквы, используемые для разделения слова на части.

    // Списки окончаний для различных частей речи
    std::vector<std::string> perfective_gerund_1; ///< Окончания совершенных деепричастий (группа 1).
    std::vector<std::string> perfective_gerund_2; ///< Окончания совершенных деепричастий (группа 2).
    std::vector<std::string> adjective; ///< Окончания прилагательных.
    std::vector<std::string> participle_1; ///< Окончания причастий (группа 1).
    std::vector<std::string> participle_2; ///< Окончания причастий (группа 2).
    std::vector<std::string> reflexive; ///< Окончания возвратных глаголов.
    std::vector<std::string> verb_1; ///< Окончания глаголов (группа 1).
    std::vector<std::string> verb_2; ///< Окончания глаголов (группа 2).
    std::vector<std::string> noun; ///< Окончания существительных.
    std::vector<std::string> superlative; ///< Окончания превосходной степени прилагательных.
    std::vector<std::string> derivational; ///< Производные суффиксы.

    /**
     * @brief Инициализирует списки окончаний.
     */
    void initialize_endings();

    /**
     * @brief Определяет регионы R1, R2 и RV в слове.
     *
     * Разделяет слово на три области, которые используются в алгоритме стемминга.
     * @param word Исходное слово.
     * @return Кортеж строк (R1, R2, RV).
     */
    std::tuple<std::string, std::string, std::string> russian_set_regions(const std::string &word) const;

    /**
     * @brief Выполняет первый шаг стемминга.
     *
     * Производит обработку окончаний в области RV.
     * @param rv Часть слова, начиная с первой гласной (RV).
     * @param word Исходное слово.
     * @return Обработанное слово.
     */
    std::string step_1(const std::string &rv, const std::string &word) const;

    /**
     * @brief Удаляет окончания в области RV при определенных условиях.
     *
     * @param word Исходное слово.
     * @param rv Область RV.
     * @return Обработанное слово.
     */
    std::string step_1_if(const std::string &word, const std::string &rv) const;

public:
    /**
     * @brief Конструктор, инициализирующий списки окончаний.
     */
    RussianPorterStemmer();

    /**
     * @brief Выполняет стемминг переданного слова.
     *
     * Метод принимает русское слово и возвращает его основу (стемму).
     * @param token Входное слово.
     * @return Стеммированное слово.
     */
    std::string process(const std::string &token) const override;
};

#endif // RUSSIAN_PORTER_STEMMER_H
