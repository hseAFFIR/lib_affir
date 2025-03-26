#ifndef LIB_AFFIR_TOKENINFO_H
#define LIB_AFFIR_TOKENINFO_H

#include <cstdint>

/**
 * @brief Represents the position information of a token within a file.
 *
 * This struct holds the position (pos) and word position (wordPos) of a token
 * in the original data stream.
 */
struct TokenInfo {
    uint64_t pos;    ///< The absolute position in the data stream.
    uint64_t wordPos;///< The position within a word or sentence.
};

#endif //LIB_AFFIR_TOKENINFO_H
