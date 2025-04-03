#ifndef LIB_AFFIR_FILESTORAGE_H
#define LIB_AFFIR_FILESTORAGE_H

#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include "../../common.h"

// Struct to hold metadata about a file.
struct DataStruct {
    Pos startPos;
    std::string filename;
    size_t filesize;

    /**
     * @brief Calculates the file's end in common raw data storage.
     *
     * @return The end position of the data stored in the storage.
     */
    Pos endPos() const {
        return startPos + filesize;
    }
};

/**
 * @class FileStorage
 *
 * @brief Class for saving and reading text data, that have to be indexed further.
 */
class FileStorage {
public:
    /**
     * @brief Constructs a FileStorage object using a file ID. Usually used to read data.
     *
     * @param fileId The file identifier.
     */
    explicit FileStorage(FileId fileId);

    /**
     * @brief Constructs a FileStorage object using a filename and filesize. Usually used to write data.
     *
     * @param filename The name of the file.
     * @param filesize The size of the file.
     */
    explicit FileStorage(const std::string& filename, size_t filesize);
    /**
     * @brief Writes data to the file storage.
     *
     * @param data The data string to write.
     */
    void write(std::string_view data);
    /**
     * @brief Reads data from the file storage into a buffer, with a specified number of bytes and start position.
     *
     * @param buffer The buffer to store read data.
     * @param bytesToRead The number of bytes to read.
     * @param startPos The starting position in the file to read from.
     * @return The number of bytes successfully read.
     */
    size_t read(std::vector<char>& buffer, size_t bytesToRead, size_t startPos);
    /**
     * @brief Reads data from the last position in file storage into a buffer, with a specified number of bytes.
     *
     * @param buffer The buffer to store read data.
     * @param bytesToRead The number of bytes to read.
     * @return The number of bytes successfully read.
     */
    size_t read(std::vector<char>& buffer, size_t bytesToRead);
    /**
     * @brief Reads a default-sized block of data from the file storage into a buffer.
     *
     * @param buffer The buffer to store read data.
     * @return The number of bytes successfully read.
     */
    size_t read(std::vector<char>& buffer) { return read(buffer, BYTES_BLOCK); }
    /**
     * @brief Checks if the file has reached the end of the data.
     *
     * @return True if the end of the data is reached, false otherwise.
     */
    bool isEnd();
    /**
     * @brief Closes the file safe and any associated file handles. Also destroys instance.
     */
    void close();

    static const unsigned short BYTES_BLOCK = 512; ///< The default block size for reading and writing.

    /**
     * @brief Gets the file ID in the storage.
     *
     * @return The file ID.
     */
    FileId getId() const;
    /**
     * @brief Gets the size of the file.
     *
     * @return The size of the file in bytes.
     */
    size_t getFilesize() const;
    /**
     * @brief Gets the filename associated with the data in storage.
     *
     * @return The filename.
     */
    std::string getFilename() const;
    /**
     * @brief Loads metadata for the storage system.
     */
    static void loadStorageMeta();
    /**
     * @brief Saves metadata for the storage system.
     * @note It has to be called before exit!
     */
    static void saveStorageMeta();
    /**
     * @brief Destructor.
     */
    ~FileStorage();
private:
    static unsigned short instancesNumber;                     ///< Counter for instances of FileStorage.
    static Pos g_cursor;                                       ///< Global cursor to track read/write positions.
    static std::unordered_map<FileId, DataStruct> dataMap;     ///< Map of file metadata by FileId.
    static bool isStorageLoaded;                               ///< Flag to indicate if storage metadata is loaded.

    /**
     * @brief Opens the file for reading and writing.
     */
    void open();

    /**
     * @brief Creates a data file if it doesn't exist.
     */
    void createDataFile();

    DataStruct dataStruct;  ///< Metadata associated with the file.
    FileId id;              ///< The unique file identifier.

    const static std::string DATA_FILENAME_PATH; ///< Path to the data file.
    const static std::string META_FILENAME_PATH; ///< Path to the metadata file.

    std::fstream dataStream; ///< File stream for reading/writing data.

    size_t currentPosition{}; ///< Current position in the file during read operations.
};


#endif //LIB_AFFIR_FILESTORAGE_H
