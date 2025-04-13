#ifndef LIB_AFFIR_IINDEXSTORAGE_H
#define LIB_AFFIR_IINDEXSTORAGE_H

#include <vector>
#include <iostream>
#include <string>
#include "../../models/bigToken.h"

/**
 * @brief Interface for a storage index.
 *
 * This abstract class defines the structure for indexing and retrieving
 * raw index data. Implementations should provide their own logic for
 * creating and accessing an index.
 */
class IIndexStorage {
public:
    virtual ~IIndexStorage() = default;

    /**
     * @brief Creates an index from the given data.
     *
     * @param data A map where keys are strings and values are BigToken objects.
     */
    virtual void createIndex(const std::unordered_map<std::string, BigToken>& data) = 0;
    /**
     * @brief Retrieves the raw index from the files by given token bodies.
     *
     * @param bodies A string representing the body of token.
     * @param[out] output A PosMap where results will be stored.
     */
    virtual PosMap getRawIndex(const std::string& body) = 0;
    /**
    * @brief Closes all file streams and flush data.
    */
    virtual void close() = 0;
    /**
    * @brief Save static index metadata into files.
    */
    virtual void saveStorageMeta() = 0;
    /**
    * @brief Load static index metadata into internal structures.
    */
    virtual void loadStorageMeta() = 0;
};


#endif //LIB_AFFIR_IINDEXSTORAGE_H
