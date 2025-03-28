#include "encodingHandler.h"
#include <locale>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

void EncodingHandler::initConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#else
    setlocale(LC_ALL, "en_US.UTF-8");
#endif
}

bool EncodingHandler::isValidUTF8(const std::string& text) const {
    for (size_t i = 0; i < text.size();) {
        unsigned char c = text[i];
        int len = 0;
        if ((c & 0x80) == 0x00) len = 1;
        else if ((c & 0xE0) == 0xC0) len = 2;
        else if ((c & 0xF0) == 0xE0) len = 3;
        else if ((c & 0xF8) == 0xF0) len = 4;
        else return false;
        
        if (i + len > text.size()) return false;
        for (int j = 1; j < len; ++j) {
            if ((text[i + j] & 0xC0) != 0x80) return false;
        }
        i += len;
    }
    return true;
}

#ifdef _WIN32
std::string EncodingHandler::convertFromWindowsEncoding(const std::string& text) {
    int wsize = MultiByteToWideChar(1251, 0, text.c_str(), -1, nullptr, 0);
    std::wstring wstr(wsize, 0);
    MultiByteToWideChar(1251, 0, text.c_str(), -1, &wstr[0], wsize);
    
    int utf8_size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string utf8_str(utf8_size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8_str[0], utf8_size, nullptr, nullptr);
    return utf8_str;
}
#endif

std::string EncodingHandler::convertToUTF8(const std::string& text) {
    if (isValidUTF8(text)) return text;

#ifdef _WIN32
    std::string converted = convertFromWindowsEncoding(text);
    if (isValidUTF8(converted)) return converted;
#endif

    throw std::runtime_error("Unsupported encoding or conversion failed");
}

std::string EncodingHandler::processInput(const std::string& text) {
    try {
        return convertToUTF8(text);
    } 
    catch (const std::runtime_error& e) {
        if (!isValidUTF8(text)) {
            throw std::runtime_error("Invalid encoding: input is not UTF-8 and conversion failed");
        }
        return text;
    }
}