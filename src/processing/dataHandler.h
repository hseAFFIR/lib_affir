#ifndef LIB_AFFIR_DATAHANDLER_H
#define LIB_AFFIR_DATAHANDLER_H

#include <iostream>
#include <string>
#include <vector>
#include "../tokenizer/filters/base.h"
#include "../tokenizer/tokenizer.h"
#include "../common.h"

class DataHandler {
public:
    DataHandler(const std::vector<Base*> &filters);
    void processText(const std::string &text, const std::string &filename);
    const std::vector<Base*>& getFilters() const { return filters; };
    //FileId getFileId(){ return fileId; };
    //void setFileId(FileId &fileId){ DataHandler::fileId = fileId; };
private:
    //FileId fileId;
    std::vector<Base*> filters;
};

#endif