#include "multiFileIndexStorage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

MultiFileIndexStorage::MultiFileIndexStorage() {
    if (!std::filesystem::exists(storageDir)) {
        std::filesystem::create_directory(storageDir);
    }

    fileCounter = 0;
    for (const auto &entry : std::filesystem::directory_iterator(storageDir)) {
        std::string filename = entry.path().filename().string();
        if (filename.starts_with("index_") && filename.ends_with(".json")) {
            try {
                std::string numStr = filename.substr(6, filename.length() - 6 - 5); // "index_123.json" -> 123
                unsigned long num = std::stoul(numStr);
                if (num > fileCounter) {
                    fileCounter = num;
                }
            } catch (...) {
                // Ignore invalid files
            }
        }
    }
    fileCounter++; // Next file will be fileCounter
}

MultiFileIndexStorage::~MultiFileIndexStorage() {}

void MultiFileIndexStorage::createIndex(std::unordered_map<std::string, BigToken> &data) {
    std::string filename = storageDir + "/index_" + std::to_string(fileCounter) + ".json";
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error creating index file: " << filename << std::endl;
        return;
    }

    auto metadata = loadMetadata();

    for (const auto &[token, bigToken] : data) {
        std::streampos offset = outFile.tellp();
        std::string entry = posMapToJson(bigToken.getFilePositions()) + "\n";
        outFile << entry;
        metadata[token].emplace_back(filename, static_cast<size_t>(offset));
    }

    outFile.close();
    saveMetadata(metadata);
    fileCounter++;
}

void MultiFileIndexStorage::getRawIndex(const std::string &body, std::vector<PosMap> &output) {
    auto metadata = loadMetadata();

    if (metadata.find(body) == metadata.end()) {
        std::cerr << "Token not found: " << body << std::endl;
        return;
    }

    for (const auto &[file, offset] : metadata[body]) {
        std::ifstream inFile(file, std::ios::binary);
        if (!inFile) {
            std::cerr << "Error opening file: " << file << std::endl;
            continue;
        }

        inFile.seekg(offset);
        std::string line;
        if (!std::getline(inFile, line)) {
            std::cerr << "Error reading line from file: " << file << " at offset " << offset << std::endl;
            continue;
        }

        try {
            output.push_back(jsonToPosMap(line));
        } catch (const std::exception &e) {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
        }
    }
}

std::unordered_map<std::string, std::vector<std::pair<std::string, size_t>>> MultiFileIndexStorage::loadMetadata() {
    std::unordered_map<std::string, std::vector<std::pair<std::string, size_t>>> metadata;
    std::ifstream inFile(metadataFile);
    if (!inFile) return metadata;

    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string token, file;
        size_t offset;
        if (iss >> token >> file >> offset) {
            metadata[token].emplace_back(file, offset);
        }
    }

    return metadata;
}

void MultiFileIndexStorage::saveMetadata(const std::unordered_map<std::string, std::vector<std::pair<std::string, size_t>>> &metadata) {
    std::ofstream outFile(metadataFile);
    if (!outFile) {
        std::cerr << "Error saving metadata file" << std::endl;
        return;
    }

    for (const auto &[token, entries] : metadata) {
        for (const auto &[file, offset] : entries) {
            outFile << token << " " << file << " " << offset << "\n";
        }
    }
}

std::string MultiFileIndexStorage::posMapToJson(const PosMap &posMap) {
    std::stringstream json;
    json << "{";
    bool firstFile = true;
    for (const auto &[fileId, positions] : posMap) {
        if (!firstFile) json << ",";
        firstFile = false;
        json << "\"" << fileId << "\":[";
        bool firstPos = true;
        for (const auto &pos : positions) {
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