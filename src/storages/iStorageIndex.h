#ifndef LIB_AFFIR_ISTORAGEINDEX_H
#define LIB_AFFIR_ISTORAGEINDEX_H

#include <vector>
#include <iostream>
#include <string>
#include "../models/bigToken.h"

/**
 * @brief Interface for a storage index.
 *
 * This abstract class defines the structure for indexing and retrieving
 * raw index data. Implementations should provide their own logic for
 * creating and accessing an index.
 */
class IStorageIndex {
    /**
     * @brief Creates an index from the given data.
     *
     * @param data A map where keys are strings and values are BigToken objects.
     */
    virtual void createIndex(std::unordered_map<std::string, BigToken>& data) = 0;
    /**
     * @brief Retrieves the raw index from the files by given token bodies.
     *
     * @param bodies A vector of strings representing the bodies to tokens.
     * @param[out] output A vector of references to PosMap where results will be stored.
     */
    virtual void getRawIndex(const std::vector<std::string>& bodies, std::vector<PosMap&>&) = 0;
};


#endif //LIB_AFFIR_ISTORAGEINDEX_H
