#include "../tests.h"

/**
 * @brief Тест на время getTokenInfo в уже созданном индексе
 *
 * @param text слово для поиска
 * @param buffer Maximum buffer size (in bytes) before flushing to storage.
 * @param filters фильтры для токенизатора
 * @param storage Instance of object MFIS or SFIS
 */
void
runTestGetTokenInCreatedIndex(std::string &text, size_t buffer, std::vector<Base *> &filters, IIndexStorage &storage) {
    Tokenizer tk(TokenizerMode::CLEAR_POSES, filters);
    Indexer ind(buffer, storage);

    tk.tokenize(text);
    while (tk.hasNext()) {
        auto start = std::chrono::high_resolution_clock::now();
        ind.getTokenInfo(tk.next().body);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> diff = end - start;
        std::cout << "Total getTokenInfo time: "
                  << diff.count() << " ms\n";
    }
}
