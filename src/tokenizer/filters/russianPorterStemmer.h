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
 * Класс реализует алгоритм Портера для русского языка. Он удаляет морфологические окончания,
 * чтобы привести слова к их корням, облегчая анализ и поиск.
 */
class RussianPorterStemmer : public Base {
private:
    /**
     * @brief Устанавливает области R1, R2 и RV в слове, согласно правилам стемминга.
     *
     * @param token Исходное слово.
     * @param rv Область RV.
     * @param r1 Область R1.
     * @param r2 Область R2.
     */
    void setRussianRegions(std::string &token, std::string &rv, std::string &r1, std::string &r2);

    /**
     * @brief Первый шаг стемминга, удаляет деепричастия, глаголы и существительные.
     *
     * @param token Слово для обработки.
     * @return std::string Результат после применения шага 1.
     */
    std::string step1(std::string token);

    /**
     * @brief Вспомогательный метод для первого шага, если были удалены деепричастия.
     *
     * @param token Слово для обработки.
     * @return std::string Результат после дополнительной обработки.
     */
    std::string step1If(std::string token);

    std::string vowels; ///< Гласные буквы, используемые для выделения областей слова.

    std::vector<std::string> perfectiveGerund1; ///< Суффиксы совершенных деепричастий (группа 1).
    std::vector<std::string> perfectiveGerund2; ///< Суффиксы совершенных деепричастий (группа 2).
    std::vector<std::string> perfectiveGerund;  ///< Общий список суффиксов деепричастий.

    std::vector<std::string> participle; ///< Суффиксы причастий (общие).
    std::vector<std::string> adjective;  ///< Суффиксы прилагательных.

    std::vector<std::string> participle1; ///< Суффиксы причастий (группа 1).
    std::vector<std::string> participle2; ///< Суффиксы причастий (группа 2).

    std::vector<std::string> reflexive;  ///< Суффиксы возвратных глаголов.

    std::vector<std::string> verb1; ///< Суффиксы глаголов (группа 1).
    std::vector<std::string> verb2; ///< Суффиксы глаголов (группа 2).
    std::vector<std::string> verb;  ///< Общий список суффиксов глаголов.

    std::vector<std::string> noun;        ///< Суффиксы существительных.
    std::vector<std::string> superlative; ///< Суффиксы превосходной степени.
    std::vector<std::string> derivational;///< Суффиксы производных форм.

public:
    /**
     * @brief Конструктор класса RussianPorterStemmer.
     *
     * Инициализирует списки суффиксов и гласные буквы.
     */
    RussianPorterStemmer();

    /**
     * @brief Обрабатывает слово, применяя алгоритм Портера.
     *
     * @param token Слово, которое необходимо привести к основе.
     */
    void process(std::string &token) override;

    /**
     * @brief Возвращает порядок применения фильтра.
     *
     * @return FilterOrder Порядок применения: RussianPorterStemmer.
     */
    FilterOrder getOrder() const override {
        return FilterOrder::RussianPorterStemmer;
    }
};

#endif // RUSSIAN_PORTER_STEMMER_H
