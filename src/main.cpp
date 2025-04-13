#include "storages/indexes/multi/multiFileIndexStorage.h"
#include "logger/logger.h"
#include "engine.h"

int main() {
    Logger::init("logs/log.txt");
    Logger::info("Main", "Application started");

    Engine engine(Engine::IndexStorageType::MULTI);

    Engine::displayResult(engine.find("long text"));

    return 0;
}
