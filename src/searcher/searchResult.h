//
// Created by Shon on 19.04.2025.
//

#ifndef LIB_AFFIR_SEARCHRESULT_H
#define LIB_AFFIR_SEARCHRESULT_H

#include <string>
#include <iostream>
#include <sstream>
#include "../common.h"

/**
 * @brief Structure for storing search results
 */
class SearchResult {
public:
    std::string query;       ///< Tokenized query
    size_t query_original_size{};
    PosMap posMap;           ///< Position map in files

    std::string toString() const {
        std::ostringstream buffer;
        buffer << "Tokenized query (original size = " << query_original_size << "): " << query << std::endl;
        for (const auto& [fileId, tokenInfos] : posMap) {
            buffer << "File ID: " << fileId << std::endl;
            buffer << "Positions: ";
            for (const auto& info : tokenInfos)
                buffer << "\n\t(pos=" << info.pos << ", wordPos=" << info.wordPos << ")";
            buffer << "\n\n";
        }
        std::string output = buffer.str();
        if (output.size() >= 2)
            output.erase(output.size() - 2);
        return output;
    }
};


#endif //LIB_AFFIR_SEARCHRESULT_H
