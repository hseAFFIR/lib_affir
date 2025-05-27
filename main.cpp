#include "affir/logger/logger.h"
#include "affir/engine.h"

using namespace affir;

int main() {
    system("chcp 65001");
    Logger::init(Logger::Level::Debug, "logs/log.log");
    LOG_INFO(Logger::logger, "Application started");

    Engine engine(EngineFocus::POSES, FilterType::HTMLER | FilterType::LOWERCASER, IndexStorageType::SINGLE);
    // Или без фильтров: Engine engine(EngineFocus::NATIVE, Engine::IndexStorageType::SINGLE); // Либо можно передать FilterType::NONE

//    engine.proceed(
//            "Horatio says 'tis but our fantasy,\n"
//            "And will not let belief take hold of him\n"
//            "Touching this dreaded sight, twice seen of us:","Some file");

    // Или передать путь до файла. Вторым параметром по желанию можно указать CHUNK_SIZE
//    engine.proceed("/path/to/file");

    FileId fileId = 1;
    engine.openFS(fileId);
    std::vector<char> buffer;
    engine.read(buffer);
    LOG_INFO(Logger::logger, "Read 512 bytes of the given file ({}):\n{}", fileId, buffer.data());

    auto res = engine.find("sweet dreams");
    LOG_INFO(Logger::logger, "Result:\n{}", res.toString());

    size_t contextSymbols = 20;
    LOG_INFO(Logger::logger, "Result in context in file ({}): {}", fileId,
             Engine::returnResultInContext(res, contextSymbols)[fileId].front());

    return 0;
}
