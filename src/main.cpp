#include "storages/indexes/multi/multiFileIndexStorage.h"
#include "logger/logger.h"
#include "engine.h"

int main() {
    Logger::init("logs/log.txt");
    Logger::info("Main", "Application started");

    Engine engine(FilterType::HTMLER | FilterType::LOWERCASER, IndexStorageType::MULTI);
    // Или без фильтров: Engine engine(Engine::IndexStorageType::SINGLE); // Либо можно передать FilterType::NONE

//    engine.proceed("Западная граница Карелии является частью государственной границы Российской Федерации и Финляндии. На востоке Карелия граничит с Архангельской областью, на юге — с Вологодской и Ленинградской областями, на севере — с Мурманской областью.", "Some file");

    // Или передать путь до файла. Вторым параметром по желанию можно указать CHUNK_SIZE
//    engine.proceed("/path/to/file");

    auto res = engine.find("Карелия");
    engine.openFS(1);
    Logger::info("Main", "Result:\n{}", res.toString());
    Logger::info("Main", "{}", Engine::returnResultInContext(res, 20)[1].front());

    return 0;
}
