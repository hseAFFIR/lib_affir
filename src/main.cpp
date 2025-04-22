#include <iostream>
#include <tuple>
#include <fstream>
#include <chrono>
#include "tokenizer/filters/russianPorterStemmer.h"
//
// int main() {
//     system("chcp 65001");
//     RussianPorterStemmer russianPorterStemmer;
//     std:: string word="асигнация";
//     std:: string result="";
//     russianPorterStemmer.process(word);
//
//     std::cout << word << std::endl;
// }
int main() {
    system("chcp 65001");
    std::ifstream vocFile("C://Users//okosh//CLionProjects//lib//src//voc.txt");
    std::ifstream outputFile("C://Users//okosh//CLionProjects//lib//src//output.txt");

    if (!vocFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Не удалось открыть один из файлов.\n";
        return 1;
    }

    auto stemmer = new RussianPorterStemmer();

    std::string vocLine, outputLine;
    int total = 0;
    int correct = 0;
    int incorrect = 0;
    int errors = 0;

    auto start = std::chrono::high_resolution_clock::now();

    while (std::getline(vocFile, vocLine) && std::getline(outputFile, outputLine)) {
        std::string result = vocLine;
        try {
            stemmer->process(result);
        } catch (const std::exception& e) {
            ++errors;
            std::cerr << "Ошибка при обработке строки:\n"
                      << "  Ввод: " << vocLine << "\n"
                      << "  Сообщение: " << e.what() << "\n";
        } catch (...) {
            ++errors;
            std::cerr << "Неизвестная ошибка при обработке строки:\n"
                      << "  Ввод: " << vocLine << "\n";
        }
        if (result == outputLine) {
            ++correct;
        } else {
            ++incorrect;
            std::cerr << "Несовпадение:\n"
                      << "  Ввод:     " << vocLine << "\n"
                      << "  Ожидание: " << outputLine << "\n"
                      << "  Результат:" << result << "\n";
        }
        ++total;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedSeconds = end - start;

    std::cout << "\nРезультаты:\n";
    std::cout << "Всего обработано: " << total << "\n";
    std::cout << "Совпадений:       " << correct << "\n";
    std::cout << "Ошибок:           " << incorrect << "\n";
    std::cout << "Сбоев обработки:  " << errors << "\n";
    std::cout << "Время выполнения: " << elapsedSeconds.count() << " секунд\n";


    return 0;
}
