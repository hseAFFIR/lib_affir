#ifndef LIB_AFFIR_BIGTOKEN_H
#define LIB_AFFIR_BIGTOKEN_H


#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

/**
 * @brief Represents the position information of a token within a file.
 *
 * This struct holds the position (pos) and word position (wordPos) of a token
 * in the original data stream.
 */
struct TokenInfo {
    unsigned long pos;    ///< The absolute position in the data stream.
    unsigned long wordPos;///< The position within a word or sentence.
};

/**
 * @brief Aggregates all occurrences of a token across different files.
 *
 * The BigToken class stores the token's text body and maps file identifiers
 * to lists of its positions within those files. This allows efficient querying
 * of where a token appears across multiple files.
 */
class BigToken {
private:
    std::string body;    ///< The text content of the token.

    /**
     * @brief Maps file IDs to lists of token positions in those files.
     *
     * Each key is a file identifier, and the value is a vector of TokenInfo
     * objects describing the token's occurrences in that file.
     */
    std::unordered_map<unsigned long, std::vector<TokenInfo>> filePositions;
public:
    const std::string &getBody() const {
        return body;
    }

    void setBody(const std::string &b) {
        BigToken::body = b;
    }

    const std::unordered_map<unsigned long, std::vector<TokenInfo>> &getFilePositions() const;

    void setFilePositions(const std::unordered_map<unsigned long, std::vector<TokenInfo>> &filePositions);



    /**
     * @brief Maps file IDs to lists of token positions in those files.
     *
     * Each key is a file identifier, and the value is a vector of TokenInfo
     * objects describing the token's occurrences in that file.
     */


    /**
     * @brief Constructs a BigToken with an empty body and no file positions.
     */
    BigToken() = default;

    /**
     * @brief Constructs a BigToken with a specified body.
     *
     * @param body The text content of the token.
     */
    explicit BigToken(std::string  body) : body(std::move(body)) {}

    /**
     * @brief Calculates the memory size occupied by this BigToken.
     *
     * @return The size in bytes.
     */
    const size_t calculateSize() const;

    /**
     * @brief Adds a new position to the token for a given file ID.
     *
     * @param fileId The ID of the file where the token appears.
     * @param info The TokenInfo structure containing position details.
     */
    void addPosition(unsigned long long fileId, const TokenInfo& info) {
        filePositions[fileId].push_back(info);
    }



};


#endif //LIB_AFFIR_BIGTOKEN_H
