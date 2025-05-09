#include "../tests.h"

std::string filtersCsv;

void filtersTests(std::string dataPath) {
    std::vector<FilterType> filtersNames = {FilterType::STEMMER, FilterType::HTMLER, FilterType::STOPWORDS, FilterType::LOWERCASER, FilterType::PUNCTUATOR, FilterType::STEMMER_RU, FilterType::STEMMER_EN, FilterType::NONE};

    class EmptyFilter : public Base{
        void process(std::string &token) override{}
        FilterOrder getOrder() const override {
            return FilterOrder::EnglishStemmer;
        }
    };

    std::vector<Base*> filters = {new StemFilter(), new Htmler(), new StopWords(), new Lowercaser(), new Punctuator(), new RussianPorterStemmer(), new EnglishStemmer(), new EmptyFilter()};

    for (int i = 0; i < 8; i++) {
        Tokenizer tokenizer(TokenizerMode::CLEAR_POSES, {filters[i]});
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
        std::cout << "Name: tokenizer+filter\n";
        std::cout << "Buffer - " << -1 << " bytes | indexStorage - " << -1
                  << std::endl;

        std::cout << "Filters - " << filterTypeToString(filtersNames[i]) << std::endl;



        std::clock_t cpuStart = std::clock();
        auto start = std::chrono::high_resolution_clock::now();
        size_t startMem = getCurrenMemoryUsage();


        for (auto &[filename, text]: fileContents) {
            tokenizer.tokenize(text, 0);

            while (tokenizer.hasNext()) {
                Token token = tokenizer.next();
            }
        }


        std::clock_t cpuEnd = std::clock();
        auto end = std::chrono::high_resolution_clock::now();
        size_t endMem = getCurrenMemoryUsage();

        size_t difMemKb = (endMem - startMem) / (1024);
        double cpuTime = static_cast<double>(cpuEnd - cpuStart) / CLOCKS_PER_SEC;
        std::chrono::duration<double> diff = end - start;

        std::cout << "Wall-clock Time: " << diff.count() << " seconds\n";
        std::cout << "CPU Time: " << cpuTime << " seconds\n";
        std::cout << "==================================================================\n";


        filtersCsv.append(formatCsvString(0, IndexStorageType::MULTI, filterTypeToString(filtersNames[i]), diff, cpuTime,  0));

    }


}

void saveFiltersTestCsvToFile() {
    std::ofstream outFile("testResultFilters.csv");

    outFile <<"wallClockTime;cpuTime;memory;buffer;storage;filter\n" + filtersCsv;
    outFile.close();

    std::cout << "Test results saved :)\n";
}