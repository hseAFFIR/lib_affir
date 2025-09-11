#include "../tests.h"

void tokenizerPerformanceTest(std::string dataPath) {
    Tokenizer tokenizer(TokenizerMode::CLEAR_POSES);
    std::unordered_map<std::string, std::string> fileContents;

    for (const auto &entry: fs::directory_iterator(dataPath)) {
        if (entry.is_regular_file()) {
            std::ifstream file(entry.path());
            if (!file) {
                std::cerr << "Could not open file: " << entry.path() << std::endl;
                continue;
            }

            std::string text((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
            std::string filename = entry.path().filename().string();
            fileContents[filename] = std::move(text);
            file.close();
        }
    }


    std::cout << "============================== Test ==============================\n";
    std::cout << "Name: tokenizer performance\n";
    std::cout << "Filters - NONE" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    unsigned long long count;

    for (auto &[filename, text]: fileContents) {
        tokenizer.tokenize(text, 0);

        while (tokenizer.hasNext()) {
            Token token = tokenizer.next();
            count++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;

    std::cout << "Wall-clock Time: " << diff.count() << " seconds\n";
    std::cout << "Tokens count: " << count << " tokens\n";
    std::cout << "Tokens per second " << count / diff.count() << " tps\n";
    std::cout << "==================================================================\n";
}
