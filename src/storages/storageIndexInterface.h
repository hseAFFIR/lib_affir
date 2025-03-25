#ifndef LIB_AFFIR_STORAGEINDEXINTERFACE_H
#define LIB_AFFIR_STORAGEINDEXINTERFACE_H

#include <vector>
#include <iostream>
#include <string>
#include "../models/bigToken.h"


class StorageIndexInterface {
    virtual void createIndex(std::unordered_map<std::string, BigToken>& data) = 0;
    virtual void getRawIndex(const std::vector<std::string>& bodies, std::vector<PosMap&>&) = 0;
};


#endif //LIB_AFFIR_STORAGEINDEXINTERFACE_H
