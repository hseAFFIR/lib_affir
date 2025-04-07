#include "searcher/search.h"
#include "indexer/indexer.h"
#include "tokenizer/filters/stemFilter.h"
#include "storages/indexes/single/singleIndexStorage.h"
#include <iostream>
#include "logger/logger.h"

void test1();
void test2();
void test3();
void check();
void test4();

int main() {
    // Logger
    system("chcp 65001"); // Для корректного отображения русского языка в логах
    Logger::init("logs/log.txt");
    Logger::info("Main", "Application started");

//    test1();
//    test2();
//    test1();
//    test3();
    test4();
    return 0;
}
void test4() {
    BigToken bigToken("token5");
    bigToken.addPosition(10009, TokenInfo(1, 2));

    std::unordered_map<std::string, BigToken> data;
    data["token5"] = bigToken;

    SingleIndexStorage singleIndexStorage;
    singleIndexStorage.createIndex(data);
    singleIndexStorage.saveStorageMeta();
}


void check() {
    SingleIndexStorage singleIndexStorage;
    std::vector<PosMap> vector;
    singleIndexStorage.getRawIndex("token2", vector);
    singleIndexStorage.close();
}

void test3() {
    BigToken bigToken("token1");
    bigToken.addPosition(1000, TokenInfo(1, 2));
    bigToken.addPosition(1000, TokenInfo(3, 4));
    bigToken.addPosition(2000, TokenInfo(11, 22));
    bigToken.addPosition(2000, TokenInfo(33, 44));

    BigToken bigToken2("token2");
    bigToken2.addPosition(1005, TokenInfo(10, 20));
    bigToken2.addPosition(1005, TokenInfo(30, 40));
    bigToken2.addPosition(2005, TokenInfo(110, 220));
    bigToken2.addPosition(2005, TokenInfo(330, 440));

    std::unordered_map<std::string, BigToken> data;
    data["token3"] = bigToken;
    data["token4"] = bigToken2;

    SingleIndexStorage singleIndexStorage;
    singleIndexStorage.createIndex(data);
    singleIndexStorage.saveStorageMeta();
}

void test2() {
    BigToken bigToken("token1");
    bigToken.addPosition(30, TokenInfo(1, 2));
    bigToken.addPosition(30, TokenInfo(3, 4));
    bigToken.addPosition(40, TokenInfo(11, 22));
    bigToken.addPosition(40, TokenInfo(33, 44));

    bigToken.addPosition(10, TokenInfo(19, 2));
    bigToken.addPosition(10, TokenInfo(39, 4));
    bigToken.addPosition(20, TokenInfo(119, 22));
    bigToken.addPosition(20, TokenInfo(339, 44));
    bigToken.addPosition(30, TokenInfo(19, 2));
    bigToken.addPosition(30, TokenInfo(39, 4));
    bigToken.addPosition(40, TokenInfo(119, 22));
    bigToken.addPosition(40, TokenInfo(339, 44));

    BigToken bigToken2("token2");
    bigToken2.addPosition(300, TokenInfo(10, 20));
    bigToken2.addPosition(300, TokenInfo(30, 40));
    bigToken2.addPosition(400, TokenInfo(110, 220));
    bigToken2.addPosition(400, TokenInfo(330, 440));

    bigToken2.addPosition(100, TokenInfo(109, 20));
    bigToken2.addPosition(100, TokenInfo(309, 40));
    bigToken2.addPosition(200, TokenInfo(119, 220));
    bigToken2.addPosition(200, TokenInfo(3309, 440));
    bigToken2.addPosition(300, TokenInfo(109, 20));
    bigToken2.addPosition(300, TokenInfo(309, 40));
    bigToken2.addPosition(400, TokenInfo(1109, 220));
    bigToken2.addPosition(400, TokenInfo(3309, 440));

    std::unordered_map<std::string, BigToken> data;
    data["token1"] = bigToken;
    data["token2"] = bigToken2;

    SingleIndexStorage singleIndexStorage;
    singleIndexStorage.createIndex(data);
    singleIndexStorage.saveStorageMeta();
}

void test1() {
    BigToken bigToken("token1");
    bigToken.addPosition(10, TokenInfo(1, 2));
    bigToken.addPosition(10, TokenInfo(3, 4));
    bigToken.addPosition(20, TokenInfo(11, 22));
    bigToken.addPosition(20, TokenInfo(33, 44));

    BigToken bigToken2("token2");
    bigToken2.addPosition(100, TokenInfo(10, 20));
    bigToken2.addPosition(100, TokenInfo(30, 40));
    bigToken2.addPosition(200, TokenInfo(110, 220));
    bigToken2.addPosition(200, TokenInfo(330, 440));

    std::unordered_map<std::string, BigToken> data;
    data["token1"] = bigToken;
    data["token2"] = bigToken2;

    SingleIndexStorage singleIndexStorage;
    singleIndexStorage.createIndex(data);
    singleIndexStorage.saveStorageMeta();
}
