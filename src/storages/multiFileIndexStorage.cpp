#include "multiFileIndexStorage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

MultiFileIndexStorage::MultiFileIndexStorage() {
    // Создаём директорию, если она не существует
    if (!std::filesystem::exists(storageDir)) {
        std::filesystem::create_directory(storageDir);
    }

    // Загружаем fileCounter из файлов
    std::ifstream meta(metadataFile);
    if (meta) {
        std::string line;
        while (std::getline(meta, line)) {
            fileCounter++;
        }
    }
}

MultiFileIndexStorage::~MultiFileIndexStorage() {}

// Функция создания индекса
void MultiFileIndexStorage::createIndex(std::unordered_map<std::string, BigToken>& data) {
    fileCounter++;
    std::string filename = storageDir + "/index_" + std::to_string(fileCounter) + ".json";

    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error creating index file: " << filename << std::endl;
        return;
    }

    // Загружаем текущие метаданные
    auto metadata = loadMetadata();

    // Записываем данные в JSON-формате
    outFile << "{\n";
    bool firstEntry = true;

    for (const auto& [token, bigToken] : data) {
        if (!firstEntry) outFile << ",\n";
        firstEntry = false;

        outFile << "  \"" << token << "\": " << posMapToJson(bigToken.getFilePositions());

        // Обновляем metadata
        metadata[token].push_back(filename);
    }

    outFile << "\n}\n";
    outFile.close();

    // Сохраняем обновлённые метаданные
    saveMetadata(metadata);
}

// Функция получения индекса
void MultiFileIndexStorage::getRawIndex(const std::string& body, std::vector<PosMap>& output) {
    auto metadata = loadMetadata();

    if (metadata.find(body) == metadata.end()) {
        std::cerr << "Token not found: " << body << std::endl;
        return;
    }

    std::cout << "Найден токен '" << body << "' в файлах: ";
    for (const auto& file : metadata[body]) {
        std::cout << file << " ";
    }
    std::cout << std::endl;

    // Загружаем данные из файлов
    for (const auto& file : metadata[body]) {
        std::cout << "Открываем файл: " << file << std::endl;

        std::ifstream inFile(file);
        if (!inFile) {
            std::cerr << "Error reading index file: " << file << std::endl;
            continue;
        }

        std::stringstream buffer;
        buffer << inFile.rdbuf();
        std::string jsonStr = buffer.str();
        inFile.close();

        std::cout << "Содержимое файла:\n" << jsonStr << std::endl;

        // Ищем нужный токен в JSON
        size_t pos = jsonStr.find("\"" + body + "\":");
        if (pos == std::string::npos) {
            std::cerr << "Токен '" << body << "' не найден в файле " << file << std::endl;
            continue;
        }

        // Найдём начало объекта `{`
        size_t start = jsonStr.find("{", pos);
        if (start == std::string::npos) {
            std::cerr << "Ошибка: не найден открывающий '{' для токена " << body << std::endl;
            continue;
        }

        // Найдём конец JSON-объекта с помощью подсчёта `{` и `}`
        int balance = 1;  // Открывающая `{` уже найдена
        size_t end = start + 1;
        while (end < jsonStr.size() && balance > 0) {
            if (jsonStr[end] == '{') balance++;
            if (jsonStr[end] == '}') balance--;
            end++;
        }

        if (balance != 0) {
            std::cerr << "Ошибка: несбалансированные фигурные скобки в JSON для токена " << body << std::endl;
            continue;
        }

        std::string tokenData = jsonStr.substr(start, end - start);
        std::cout << "Извлечённые данные токена:\n" << tokenData << std::endl;

        output.push_back(jsonToPosMap(tokenData));
        std::cout << "Парсинг JSON завершён, добавлено " << output.back().size() << " записей.\n";
    }
}



// Загрузка метаданных
std::unordered_map<std::string, std::vector<std::string>> MultiFileIndexStorage::loadMetadata() {
    std::unordered_map<std::string, std::vector<std::string>> metadata;
    std::ifstream inFile(metadataFile);
    if (!inFile) return metadata;

    std::string token, file;
    while (inFile >> token >> file) {
        metadata[token].push_back(file);
    }

    return metadata;
}

// Сохранение метаданных
void MultiFileIndexStorage::saveMetadata(const std::unordered_map<std::string, std::vector<std::string>>& metadata) {
    std::ofstream outFile(metadataFile);
    if (!outFile) {
        std::cerr << "Error saving metadata file" << std::endl;
        return;
    }

    for (const auto& [token, files] : metadata) {
        for (const auto& file : files) {
            outFile << token << " " << file << "\n";
        }
    }
}

// Преобразование PosMap в JSON-строку
std::string MultiFileIndexStorage::posMapToJson(const PosMap& posMap) {
    std::stringstream json;
    json << "{";
    bool firstFile = true;

    for (const auto& [fileId, positions] : posMap) {
        if (!firstFile) json << ",";
        firstFile = false;
        json << "\"" << fileId << "\":[";

        bool firstPos = true;
        for (const auto& pos : positions) {
            if (!firstPos) json << ",";
            firstPos = false;
            json << "{" << "\"pos\":" << pos.pos << ",\"wordPos\":" << pos.wordPos << "}";
        }

        json << "]";
    }
    json << "}";
    return json.str();
}

PosMap MultiFileIndexStorage::jsonToPosMap(const std::string& jsonStr) {
    PosMap posMap;

    size_t pos = 0;
    while (true) {
        // Находим следующий ключ (ID документа)
        size_t keyStart = jsonStr.find("\"", pos);
        if (keyStart == std::string::npos) break;
        size_t keyEnd = jsonStr.find("\"", keyStart + 1);
        if (keyEnd == std::string::npos) break;

        std::string docIdStr = jsonStr.substr(keyStart + 1, keyEnd - keyStart - 1);
        int docId = std::stoi(docIdStr);  // Преобразуем строковый ID в число

        // Находим начало массива позиций
        size_t arrayStart = jsonStr.find("[", keyEnd);
        if (arrayStart == std::string::npos) break;

        size_t arrayEnd = jsonStr.find("]", arrayStart);
        if (arrayEnd == std::string::npos) break;

        std::string positionsStr = jsonStr.substr(arrayStart + 1, arrayEnd - arrayStart - 1);
        pos = arrayEnd + 1;  // Продолжаем парсинг дальше

        // Разбираем объекты в массиве
        size_t objStart = 0;
        while (true) {
            size_t posStart = positionsStr.find("\"pos\":", objStart);
            if (posStart == std::string::npos) break;
            posStart += 6;  // Длина "pos":

            size_t posEnd = positionsStr.find(",", posStart);
            if (posEnd == std::string::npos) break;

            unsigned long posValue = std::stoi(positionsStr.substr(posStart, posEnd - posStart));

            size_t wordPosStart = positionsStr.find("\"wordPos\":", posEnd);
            if (wordPosStart == std::string::npos) break;
            wordPosStart += 10;  // Длина "wordPos":

            size_t wordPosEnd = positionsStr.find("}", wordPosStart);
            if (wordPosEnd == std::string::npos) break;

            unsigned long wordPosValue = std::stoi(positionsStr.substr(wordPosStart, wordPosEnd - wordPosStart));

            posMap[docId].push_back({posValue, wordPosValue});

            objStart = wordPosEnd + 1;
        }
    }

    return posMap;
}


