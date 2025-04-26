#include "../tests.h"

void groupTest(std::string dataPath) {
    std::vector<IndexStorageType> storages = {IndexStorageType::MULTI, IndexStorageType::SINGLE};
    std::vector<size_t> buffer_sizes = {1024*512, 1024*1024, 1024*1024*2, 1024*1024*4, 1024*1024*8, 1024*1024*16, 1024*1024*32};



    for (auto &sT: storages) {
        for (auto &b: buffer_sizes) {
            runEngineTestString(dataPath, EngineFocus::POSES, FilterType::NONE, sT, b);
        }
    }
}