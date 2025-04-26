# FileStorage
## Table of Contents
<!-- TOC -->
* [FileStorage](#filestorage)
  * [Table of Contents](#table-of-contents)
  * [Overview](#overview)
  * [How to use](#how-to-use)
<!-- TOC -->
## Overview
The main AFFIR API. It implements:
1. Input data saving to internal storage (it takes either path to a file or raw text data)
2. Indexation of input data  
2. Search over indexed data
3. Getting search result in context
4. Reading original input data from file storage

## How to use
### Initialization

Class takes 3 parameters:
1. Engine focus:
It is required because of the specific of UTF-8 encoding. English symbol size equals 1 byte, whereas a russian one is 2 bytes.
   - `EngineFocus::POSES`\
Focus on storaging 'clear' positions, that means **1 character = 1 position**.
However, this leads to **incorrect display of search results in the context**.
   - `EngineFocus::CONTEXT`\
On the contrary, it saves the positions of tokens according 
to their native encoding (mostly UTF-8), i.e. **1 character ≠ 1 position**,
however, it allows you to output the **correct context for search results**.
2. Applying filters (`FilterType::`):
   - `NONE`
   - `HTMLER`
   - `LOWERCASER`
   - `PUNCTUATOR`
   - `STEMMER`
   - `STOPWORDS`
   - `STEMMER_RU`
   - `STEMMER_EN` 
3. Index storage type:
   - [Multi file type](indexStorage.md#multi-approach) (`IndexStorageType::MULTI`)
   - [Single file type](indexStorage.md#single-approach) (`IndexStorageType::SINGLE`)
4. _(Optional)_ Buffer size for [DataHandler](dataHandler.md)
(it is a pool of handled tokens which are going to be saved into index storage).
**Default = 16 Mb**

#### Examples

```C++
#include "affir/engine.h"

using namespace affir;

int main() {
    
    Engine engine_16mb(EngineFocus::POSES,
                  FilterType::HTMLER | FilterType::LOWERCASER,
                  IndexStorageType::MULTI);
    
    Engine engine_1mb(EngineFocus::POSES,
              FilterType::HTMLER | FilterType::LOWERCASER,
              IndexStorageType::MULTI, 1024 * 1024 * 1);
    
    Engine engineWithoutFilters1(EngineFocus::NATIVE, Engine::IndexStorageType::SINGLE);
    Engine engineWithoutFilters2(EngineFocus::NATIVE, FilterType::NONE, Engine::IndexStorageType::SINGLE);
    
    return 0;
}
```

### Practical usage
#### Proceeding input data
```C++
...
engine.proceed("My text that I'm gonna to tokenize and indexing", "The_name_of_my_file");
// Or
engine.proceed("/path/to/file_1"); // DEFAULT_CHUNK_SIZE = 512
size_t CHUNK_SIZE_FOR_READING_FILE = 1024
engine.proceed("/path/to/file_2", CHUNK_SIZE_FOR_READING_FILE);
// Or (not recommended)
FileStorage fileStorage(my_filename, filesize);
engine.proceed("Text chunk 1", fileStorage);
engine.proceed("Text chunk 2", fileStorage);
flush(); // Do not forget to save metadata (needed only for this approach)
...
```
#### Reading original input data
```C++
// Firstly, open file storage (it is obligatory step)
FileId fileId = 1;
engine.openFS(fileId);
// Read data as you wish
std::vector<char> buffer;
while (!storage.isEnd()) {
    size_t bytesRead = storage.read(buffer);
    Logger::info(Logger::logger, "Bytes read — {}", bytesRead);
    Logger::info(Logger::logger, "Buffer is {}", std::string(buffer.begin(), buffer.end()));
}
// Or
engine.read(buffer, CUSTOM_BYTES_BLOCK); // DEFAULT BYTES_BLOCK = 512
```

#### Finding text
```C++
auto res = engine.find("Карелия");
// It will display clear positions if only
// you've chosen EngineFocus::POSES mode during engine initialisation
Logger::info(Logger::logger, "Result:\n{}", res.toString());

// It will display correct context if only
// you've chosen EngineFocus::CONTEXT mode during engine initialisation
size_t customContextSymbols = 10; // DEFAULT_CONTEXT_SYMBOLS = 20
auto contextResultsMap = Engine::returnResultInContext(res, contextSymbols);
FileId fileId = 1;
Logger::info(Logger::logger, "Result in context in file ({}): {}", fileId, contextResultsMap[fileId][0]);
```

