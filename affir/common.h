#ifndef LIB_AFFIR_COMMON_H
#define LIB_AFFIR_COMMON_H

#include <unordered_map>
#include <vector>
#include <cstdint>

namespace affir {
    // Type alias for FileId to represent file identifier.
    using FileId = uint64_t;
    using Pos = uint64_t;

    struct TokenInfo {
        Pos pos{};    ///< The absolute position in the data stream.
        Pos wordPos{};///< The position within a word or sentence.
    };

    struct Token {
        std::string body;
        TokenInfo info{};
        FileId fileId{};
    };

    using PosMap = std::unordered_map<FileId, std::vector<TokenInfo>>;
}
#endif //LIB_AFFIR_COMMON_H
