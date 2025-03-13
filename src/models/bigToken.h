#ifndef LIB_AFFIR_BIGTOKEN_H
#define LIB_AFFIR_BIGTOKEN_H


#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Represents the position information of a token within a file.
 *
 * This struct holds the position (pos) and word position (wordPos) of a token
 * in the original data stream.
 */
struct TokenInfo {
    unsigned long long pos;    ///< The absolute position in the data stream.
    unsigned long long wordPos;///< The position within a word or sentence.
};

/**
 * @brief Aggregates all occurrences of a token across different files.
 *
 * The BigToken class stores the token's text body and maps file identifiers
 * to lists of its positions within those files. This allows efficient querying
 * of where a token appears across multiple files.
 */
class BigToken {
public:
    std::string body; ///< The text content of the token.

    /**
     * @brief Maps file IDs to lists of token positions in those files.
     *
     * Each key is a file identifier, and the value is a vector of TokenInfo
     * objects describing the token's occurrences in that file.
     */
    std::unordered_map<unsigned long long, std::vector<TokenInfo>> filePositions;

    /**
     * @brief Constructs a BigToken with an empty body and no file positions.
     */
    BigToken() = default;

    /**
     * @brief Constructs a BigToken with a specified body.
     *
     * @param body The text content of the token.
     */
    explicit BigToken(const std::string& body) : body(body) {}
};


#endif //LIB_AFFIR_BIGTOKEN_H
