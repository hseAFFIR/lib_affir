#ifndef LIB_AFFIR_COMMON_H
#define LIB_AFFIR_COMMON_H

#include <unordered_map>
#include <vector>
#include <cstdint>

// Type alias for FileId to represent file identifier.
typedef uint64_t FileId;
typedef uint64_t Pos;
struct TokenInfo {
    Pos pos;    ///< The absolute position in the data stream.
    Pos wordPos;///< The position within a word or sentence.
};
typedef std::unordered_map<FileId, std::vector<TokenInfo>> PosMap;

#endif //LIB_AFFIR_COMMON_H
