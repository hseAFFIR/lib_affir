#include "multiFileIndexStorage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "../../../logger/logger.h"

bool MultiFileIndexStorage::isMetadataLoaded = false;

MultiFileIndexStorage::MultiFileIndexStorage() {
    Logger::debug("MultiFileIndexStorage", "MultiFileIndexStorage module initialized");
    if (!std::filesystem::exists(storageDir)) {
        std::filesystem::create_directory(storageDir);
    }

    fileCounter = 0;
    for (const auto &entry: std::filesystem::directory_iterator(storageDir)) {
        std::string filename = entry.path().filename().string();
        if (filename.starts_with("index_") && filename.ends_with(".json")) {
            try {
                size_t num = std::stoul(filename.substr(6, filename.length() - 11));
                if (num >= fileCounter) fileCounter = num + 1;
            } catch (...) {}
        }
    }
}

void MultiFileIndexStorage::createIndex(std::unordered_map<std::string, BigToken> &data) {
    loadStorageMeta();

    std::string filename = storageDir + "/index_" + std::to_string(fileCounter++) + ".json";
    std::ofstream outFile(filename, std::ios::binary);

    if (!outFile) {
        std::cerr << "Error creating index file: " << filename << std::endl;
        return;
    }

    for (auto &[token, bigToken]: data) {
        size_t offset = static_cast<size_t>(outFile.tellp());

        outFile << posMapToJson(bigToken.getFilePositions()) << '\n';

        metadata[token].emplace_back(filename, offset);
    }
}

void MultiFileIndexStorage::getRawIndex(const std::string &body, std::vector<PosMap> &output) {
    loadStorageMeta();

    auto it = metadata.find(body);
    if (it == metadata.end()) return;

    for (const auto &[file, offset]: it->second) {
        std::ifstream inFile(file, std::ios::binary);
        if (!inFile) continue;

        inFile.seekg(offset);
        std::string line;
        if (std::getline(inFile, line)) {
            output.push_back(jsonToPosMap(line));
        }
    }
}

void MultiFileIndexStorage::saveStorageMeta() {
    std::ofstream outFile(metadataFile);
    for (const auto &[token, entries]: metadata) {
        outFile << token;
        for (const auto &[file, offset]: entries) {
            outFile << " " << file << " " << offset;
        }
        outFile << "\n";
    }
}

void MultiFileIndexStorage::loadStorageMeta() {
    if(isMetadataLoaded)
        return;
    isMetadataLoaded = true;

    metadata.clear();

    std::ifstream inFile(metadataFile);
    std::string line;

    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        std::vector<std::pair<std::string, size_t>> entries;
        std::string file;
        size_t offset;

        while (iss >> file >> offset) {
            entries.emplace_back(file, offset);
        }

        if (!entries.empty()) {
            metadata[token] = entries;
        }
    }
}

std::string MultiFileIndexStorage::posMapToJson(const PosMap &posMap) {
    std::stringstream json;
    json << "{";
    bool firstFile = true;
    for (const auto &[fileId, positions]: posMap) {
        if (!firstFile) json << ",";
        firstFile = false;
        json << "\"" << fileId << "\":[";
        bool firstPos = true;
        for (const auto &pos: positions) {
            if (!firstPos) json << ",";
            firstPos = false;
            json << pos.pos << "," << pos.wordPos;
        }
        json << "]";
    }
    json << "}";
    return json.str();
}

PosMap MultiFileIndexStorage::jsonToPosMap(const std::string &jsonStr) {
    PosMap posMap;
    size_t pos = 0;

    while (pos < jsonStr.size()) {
        pos = jsonStr.find("\"", pos);
        if (pos == std::string::npos) break;
        size_t endKey = jsonStr.find("\"", pos + 1);
        if (endKey == std::string::npos) break;

        unsigned long fileId = std::stoul(jsonStr.substr(pos + 1, endKey - pos - 1));
        pos = jsonStr.find("[", endKey);
        if (pos == std::string::npos) break;
        pos++;

        std::vector<TokenInfo> positions;
        while (pos < jsonStr.size() && jsonStr[pos] != ']') {
            while (pos < jsonStr.size() && !isdigit(jsonStr[pos]) && jsonStr[pos] != '-') pos++;
            size_t endNum = pos;
            while (endNum < jsonStr.size() && (isdigit(jsonStr[endNum]) || jsonStr[endNum] == '-')) endNum++;
            unsigned long posValue = std::stoul(jsonStr.substr(pos, endNum - pos));
            pos = endNum;

            while (pos < jsonStr.size() && !isdigit(jsonStr[pos]) && jsonStr[pos] != '-') pos++;
            endNum = pos;
            while (endNum < jsonStr.size() && (isdigit(jsonStr[endNum]) || jsonStr[endNum] == '-')) endNum++;
            unsigned long wordPosValue = std::stoul(jsonStr.substr(pos, endNum - pos));
            pos = endNum;

            positions.emplace_back(TokenInfo{posValue, wordPosValue});

            while (pos < jsonStr.size() && (jsonStr[pos] == ',' || isspace(jsonStr[pos]))) pos++;
        }

        posMap[fileId] = positions;
        pos = jsonStr.find("]", pos) + 1;
    }

    return posMap;
}

void MultiFileIndexStorage::close() {

}
