#include <unordered_map>
#include <vector>
#include <string>
#include "../indexer/indexer.h"
#include "../tokenizer/filters/stemFilter.h"

class Search {
public:
    Search(Indexer& indexer, StemFilter& stemFilter);

    /**
     * @brief Поиск словосочетаний или отдельных слов.
     * @param text Текст для поиска (может быть фразой).
     * @return Хешмапу, где:
     *   - Ключ: fileId.
     *   - Значение: вектор пар (абсолютная позиция, относительная позиция).
     */
    std::unordered_map<unsigned long, std::vector<TokenInfo>> find(const std::string& text);

private:
    Indexer& indexer;
    StemFilter& stemFilter;

    // Разбивает текст на токены (слова или фразы в кавычках).
    std::vector<std::string> splitIntoWords(const std::string& phrase) const;
    std::vector<std::string> processTokens(const std::vector<std::string>& tokens) const;
    bool isPhrase(const std::string& text) const;
};