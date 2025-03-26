//
// Created by Shon on 14.03.2025.
//


#ifndef LIB_AFFIR_COMMON_H
#define LIB_AFFIR_COMMON_H

#include <unordered_map>
#include <vector>
#include "models/tokenInfo.h"
#include <cstdint>

// Type alias for FileId to represent file identifier.
typedef uint64_t FileId;
typedef uint64_t Pos;
typedef std::unordered_map<FileId, std::vector<TokenInfo>> PosMap;

#endif //LIB_AFFIR_COMMON_H
