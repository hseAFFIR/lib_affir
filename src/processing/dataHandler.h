#ifndef LIB_AFFIR_DATAHANDLER_H
#define LIB_AFFIR_DATAHANDLER_H

#include <iostream>
#include <string>
#include <vector>
#include "../tokenizer/filters/base.h"
#include "../tokenizer/tokenizer.h"
#include "../common.h"

/**
 * @brief The DataHandler class is responsible for processing input text by applying a set of filters,
 * storing the processed text in a file, and then tokenizing it for further indexing.
 *
 * The DataHandler receives a collection of filter objects (derived from Base) and a buffer size 
 * used by the Indexer. It sorts the filters based on a predefined order and then processes the 
 * text by writing it to a file, tokenizing it, and passing the tokens to an indexer.
 */
class DataHandler {
public:
    /**
     * @brief Constructs a new DataHandler object.
     * 
     * The constructor accepts a vector of pointers to filter objects (derived from Base)
     * and sorts them according to their order (as defined by their getOrder() method).
     * 
     * @param filters A constant reference to a vector of pointers to filter objects.
     * @param buffer The size of the buffer used by the Indexer.
     */
    DataHandler(const std::vector<Base*> &filters, const size_t buffer);

    /**
     * @brief Processes the given text by writing it to a file, tokenizing it, and indexing the tokens.
     * 
     * This function performs several operations in sequence:
     * 1. Creates a FileStorage object and writes the text into the file.
     * 2. Initializes a Tokenizer with the sorted filters to process the text.
     * 3. Creates an Indexer with the specified buffer size and passes each generated token to it.
     * 
     * @param text The input text to be processed.
     * @param filename The name of the file where the text will be stored.
     */
    void processText(const std::string &text, const std::string &filename);

    /**
     * @brief Gets the vector of filter pointers used by this DataHandler.
     * 
     * @return const std::vector<Base*>& A constant reference to the vector of filter pointers.
     */
    const std::vector<Base*>& getFilters() const { return filters; };
private:
    std::vector<Base*> filters; /**< The collection of filter objects (derived from Base) applied to the text. */
    size_t buffer; /**< The buffer size used by the Indexer. */
};

#endif