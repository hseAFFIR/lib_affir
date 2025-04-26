#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "multiFileIndexStorage.h"
#include "../../../logger/logger.h"

using namespace affir;

bool MultiFileIndexStorage::isMetadataLoaded = false;

MultiFileIndexStorage::MultiFileIndexStorage() {
    LOG_INFO(Logger::logger, "MultiFileIndexStorage module init");
    // Create folder for storing indexes
    if (!std::filesystem::exists(storageDir)) {
        std::filesystem::create_directory(storageDir);
    }

    loadStorageMeta();
}

void MultiFileIndexStorage::createIndex(const std::unordered_map<std::string, BigToken> &data) {
    std::string filename = storageDir + "/index_" + std::to_string(fileCounter++) + ".json";
    std::ofstream outFile(filename, std::ios::binary);

    if (!outFile) {
        LOG_ERROR(Logger::logger, "Error creating index file: {}", filename);
        return;
    }

    for (const auto &[token, bigToken]: data) {
        size_t offset = static_cast<size_t>(outFile.tellp());

        outFile << posMapToJson(bigToken.getFilePositions()) << '\n';

        metadata[token].emplace_back(filename, offset);
    }
}

PosMap MultiFileIndexStorage::getRawIndex(const std::string &body) {
    PosMap output;

    auto it = metadata.find(body);
    if (it == metadata.end()) return std::move(output);

    for (const auto &[file, offset]: it->second) {
        std::ifstream inFile(file, std::ios::binary);
        if (!inFile) continue;

        inFile.seekg(offset);
        std::string line;
        if (std::getline(inFile, line)) {
            jsonToPosMap(line, output);
        }
    }
    return std::move(output);
}

void MultiFileIndexStorage::saveStorageMeta() {
    std::fstream outFile(metadataFile, std::ios::in | std::ios::ate);
    long pos = outFile.tellg();

    char ch;
    while (pos > 0) {
        outFile.seekg(--pos);
        outFile.get(ch);
        if (ch == metaChar) {
            LOG_DEBUG(Logger::logger, "Deleted file counter");
            outFile.close();
            std::filesystem::resize_file(metadataFile, pos);
            break;
        }
    }
    outFile.open(metadataFile, std::ios::app);
    for (const auto &[token, entries]: metadata) {
        outFile << token;
        for (const auto &[file, offset]: entries) {
            outFile << " " << file << " " << offset;
        }
        outFile << "\n";
    }
    outFile << ':' << fileCounter << "\n";
    outFile.close();
}

void MultiFileIndexStorage::loadStorageMeta() {
    if (isMetadataLoaded)
        return;
    isMetadataLoaded = true;

    metadata.clear();

    std::ifstream inFile(metadataFile);
    std::string line;

    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;
        if (token[0] == metaChar) {
            // Start read from the second char, since first is metaChar
            fileCounter = std::atoi(token.data() + 1);
            continue;
        }

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

void MultiFileIndexStorage::jsonToPosMap(const std::string &jsonStr, PosMap &posMap) {
    size_t pos = 0;

    while (pos < jsonStr.size()) {
        pos = jsonStr.find('\"', pos);
        if (pos == std::string::npos) break;
        size_t endKey = jsonStr.find('\"', pos + 1);
        if (endKey == std::string::npos) break;

        unsigned long fileId = std::stoul(jsonStr.substr(pos + 1, endKey - pos - 1));
        pos = jsonStr.find('[', endKey);
        if (pos == std::string::npos) break;
        pos++;

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

            posMap[fileId].emplace_back(TokenInfo{posValue, wordPosValue});

            while (pos < jsonStr.size() && (jsonStr[pos] == ',' || isspace(jsonStr[pos]))) pos++;
        }
        pos = jsonStr.find(']', pos) + 1;
    }
}

void MultiFileIndexStorage::close() {

}
