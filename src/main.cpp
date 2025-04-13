#include "storages/indexes/multi/multiFileIndexStorage.h"
#include "logger/logger.h"
#include "engine.h"

int main() {
    Logger::init("logs/log.txt");
    Logger::info("Main", "Application started");

    Engine engine(FilterType::HTMLER | FilterType::LOWERCASER, IndexStorageType::MULTI);
    // Или без фильтров: Engine engine(Engine::IndexStorageType::SINGLE); // Либо можно передать FilterType::NONE

    engine.proceed("My text", "Some file");

    // Или передать путь до файла. Вторым параметром по желанию можно указать CHUNK_SIZE
    engine.proceed("/path/to/file");

    auto res = engine.find("target text");
    Engine::displayResult(res);

    return 0;
}
