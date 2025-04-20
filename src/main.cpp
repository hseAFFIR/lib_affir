#include "storages/indexes/multi/multiFileIndexStorage.h"
#include "logger/logger.h"
#include "engine.h"

int main() {
    Logger::init("logs/log.txt");
    Logger::info("Main", "Application started");

    Engine engine(EngineFocus::POSES, FilterType::HTMLER | FilterType::LOWERCASER, IndexStorageType::MULTI);
    // Или без фильтров: Engine engine(EngineFocus::NATIVE, Engine::IndexStorageType::SINGLE); // Либо можно передать FilterType::NONE

    engine.proceed("Западная граница Карелии является частью государственной границы Российской Федерации и Финляндии. На востоке Карелия граничит с Архангельской областью, на юге — с Вологодской и Ленинградской областями, на севере — с Мурманской областью.", "Some file");

    // Или передать путь до файла. Вторым параметром по желанию можно указать CHUNK_SIZE
//    engine.proceed("/path/to/file");

    FileId fileId = 1;
    engine.openFS(fileId);
    std::vector<char> buffer;
    engine.read(buffer);
    Logger::info("Main", "Read 512 bytes of the given file ({}):\n{}", fileId, buffer.data());

    auto res = engine.find("Карелия");
    Logger::info("Main", "Result:\n{}", res.toString());

    size_t contextSymbols = 20;
    Logger::info("Main", "Result in context in file ({}): {}", fileId, Engine::returnResultInContext(res, contextSymbols)[fileId].front());

    return 0;
}
