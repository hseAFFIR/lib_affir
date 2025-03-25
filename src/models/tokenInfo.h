#ifndef LIB_AFFIR_TOKENINFO_H
#define LIB_AFFIR_TOKENINFO_H

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

#endif //LIB_AFFIR_TOKENINFO_H
