#ifndef LIB_AFFIR_ENCODINGHANDLER_H
#define LIB_AFFIR_ENCODINGHANDLER_H

#include <string>
#include <stdexcept>

class EncodingHandler {
public:
    /**
     * @brief Initializes the console for UTF-8 input/output (Windows-specific).
     */
    void initConsole();

    /**
     * @brief Validates if the input string is properly encoded in UTF-8.
     * @param text Input text to validate.
     * @return True if the text is valid UTF-8, false otherwise.
     */
    bool isValidUTF8(const std::string& text) const;

    /**
     * @brief Converts input text to UTF-8 encoding if it is not already valid.
     * @param text Input text to convert.
     * @return UTF-8 encoded string.
     * @throws std::runtime_error If conversion fails or unsupported encoding.
     */
    std::string convertToUTF8(const std::string& text);

    /**
     * @brief Processes raw input text (e.g., from console/file) and ensures UTF-8 encoding.
     * @param text Raw input text.
     * @return Sanitized UTF-8 string.
     * @throws std::runtime_error If text cannot be converted to UTF-8.
     */
    std::string processInput(const std::string& text);

private:
#ifdef _WIN32
    // Windows-specific codepage conversion (CP1251 → UTF-8)
    std::string convertFromWindowsEncoding(const std::string& text);
#endif
};

#endif // LIB_AFFIR_ENCODINGHANDLER_H