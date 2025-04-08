# MultifileIndexStorage
## Table of Contents
<!-- TOC -->
* [FileStorage](#filestorage)
    * [Table of Contents](#table-of-contents)
    * [Overview](#overview)
    * [How to use](#how-to-use)
<!-- TOC -->
## Overview
Module for saving and loading tokens(inverted index).
Under the hood the input information is stored in different files, for each input data a different file is created.
The module converts token information into a json-like format, writes them to a file and saves the token positions in the files into a metadata file. Collects token information from files by metadata.

The `storageDir` specifies the storage directory with the index files. 
The path to the metadata is specified in `metadataFile`. 

## How to use
- By default, when the class is initialized, the `loadMetadata()` function responsible for
  loading metadata from `metadataFile` is started once automatically.
- After you finish working with the module, call `saveMetadata()` to save the metadata to a file

### To save data
```c++
#include "storages/multiFileIndexStorage.h"

...

MultiFileIndexStorage storage;

//data for save
std::unordered_map<std::string, BigToken> data;

storage.createIndex(data);

// Don't forget to save the metadata after creating the index
storage.saveMetadata();

...

```
### To load raw index
```c++
#include "storages/multiFileIndexStorage.h"

...

MultiFileIndexStorage storage;

// vector for raw index
std::vector<PosMap> results;

storage.getRawIndex("example", results);

...

```