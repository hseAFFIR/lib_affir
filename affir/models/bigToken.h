#ifndef LIB_AFFIR_BIGTOKEN_H
#define LIB_AFFIR_BIGTOKEN_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../common.h"

namespace affir {
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
        PosMap filePositions;
        uint32_t posMapSize{};
    public:
        const std::string &getBody() const { return body; }

        void setBody(const std::string &b) { BigToken::body = b; }

        const PosMap &getFilePositions() const;

        /**
         * @brief Constructs a BigToken with an empty body and no file positions.
         */
        BigToken() = default;

        /**
         * @brief Constructs a BigToken with a specified body.
         *
         * @param body The text content of the token.
         */
        explicit BigToken(std::string body) : body(std::move(body)) { }

        BigToken(BigToken &&other) noexcept
                : body(std::move(other.body)),
                  filePositions(std::move(other.filePositions)),
                  posMapSize(other.posMapSize) {
            other.posMapSize = 0;  // Ensure the moved-from object is in a valid state
        }

        BigToken &operator=(BigToken &&other) noexcept {
            if (this != &other) {  // Self-assignment check
                body = std::move(other.body);
                filePositions = std::move(other.filePositions);
                posMapSize = other.posMapSize;
                other.posMapSize = 0;  // Ensure the moved-from object is in a valid state
            }
            return *this;
        }

        /**
         * @brief Calculates the memory size occupied by this BigToken.
         *
         * @return The size in bytes.
         */

        size_t getPosesSize() const { return posMapSize * sizeof(TokenInfo) + filePositions.size() * sizeof(FileId); };

        size_t getFullPosesSize() const { return posMapSize * (sizeof(FileId) + sizeof(TokenInfo)); };

        /**
         * @brief Adds a new position to the token for a given file ID.
         *
         * @param fileId The ID of the file where the token appears.
         * @param info The TokenInfo structure containing position details.
         */
        void addPosition(FileId fileId, const TokenInfo &info);

        /**
        * @brief Merges new file position data into the existing structure.
        *
        * @param newFilePositions The new data to merge.
        */
        void mergeFilePositions(const PosMap &newFilePositions);

        /**
         * @brief Destroys the BigToken object.
         *
         * Ensures that any dynamically allocated resources (if added in the future) are properly released.
         */
        ~BigToken();


        explicit BigToken(const Token &token);
    };
}

#endif //LIB_AFFIR_BIGTOKEN_H
