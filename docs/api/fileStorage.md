# FileStorage
## Table of Contents
<!-- TOC -->
* [FileStorage](#filestorage)
  * [Table of Contents](#table-of-contents)
  * [Overview](#overview)
  * [How to use](#how-to-use)
<!-- TOC -->
## Overview
A module for saving input data (text) onto the disk. 
Under the hood, the input information is saved in a single file. 
The module stores the correspondence between an identification of this data and its metadata such as filename (text name), 
the beginning position of the input data in the file storage and its size (in bytes).

A path to single file combining all input text is defined in `DATA_FILENAME_PATH` (equals `"storage_raw"`). 
A path to the structure describing metadata of the text is defined in `META_FILENAME_PATH` (equals `"storage_metadata.bin"`).

## How to use
- By default, during class initialization the function `loadStorageMeta()` responsible for 
loading metadata from `META_FILENAME_PATH` is launched once automatically. 
### To save data
```c++
#include "storages/files/fileStorage.h"
#include <fstream>
#include <vector>
#include "logger/logger.h"

...

const size_t INPUT_FILESIZE = 4096;
const size_t CHUNK_SIZE = 512;
// Initialise FileStorage in input mode
FileStorage storage("my_input_filename", INPUT_FILESIZE);

Logger::info("ClassName", "File ID in the storage is {}", storage.getId());

std::ifstream file("path_to_my_file.txt");

std::vector<char> buffer(CHUNK_SIZE);

// Read unless reached the end of file.
while (file.read(buffer.data(), CHUNK_SIZE) || file.gcount() > 0) {
    std::string_view data(buffer.data(), file.gcount());
    storage.write(data);
}

...

// Do not forget to manually save metadata in the end of your program!
FileStorage::saveStorageMeta();
```

### To read data
```c++
#include "storages/files/fileStorage.h"

...
// Some file id that you will get from IndexStorage
FileId someFileId = ...;
// Initialise FileStorage in output mode
FileStorage storage(someFileId);

Logger::info("ClassName", "Filename is {}\nFile size is {}",
             storage.getFilename(), storage.getFilesize());

std::vector<char> buffer;
while (!storage.isEnd()) {
    size_t bytesRead = storage.read(buffer);
    Logger::info("ClassName", "Bytes read — {}", bytesRead);
    Logger::info("ClassName", "Buffer is {}", std::string(buffer.begin(), buffer.end()));
}

...
```