//
// Created by Shon on 14.03.2025.
//


#ifndef LIB_AFFIR_COMMON_H
#define LIB_AFFIR_COMMON_H

#include <unordered_map>
#include <vector>
#include "models/tokenInfo.h"

// Type alias for FileId to represent file identifier.
typedef unsigned long FileId;
typedef unsigned long Pos;
typedef std::unordered_map<unsigned long, std::vector<TokenInfo>> PosMap;

#endif //LIB_AFFIR_COMMON_H
