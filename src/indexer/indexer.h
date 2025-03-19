#ifndef LIB_AFFIR_INDEXER_H
#define LIB_AFFIR_INDEXER_H
#include <iostream>
#include <vector>
#include <string>
#include "../models/bigToken.h"
#include "../models/token.h"


/**
 * @brief Manages token storage with a buffer-based approach.
 *
 * The Indexer class handles tokens by storing them temporarily in a buffer (unordered_map) until
 * it reaches a predefined size limit (in bytes). When the buffer is full, it is flushed to persistent
 * storage. This ensures efficient batch processing while managing memory constraints.
 */
class Indexer {
private:
    using BufferType = std::unordered_map<std::string, BigToken>;

    BufferType buffer; ///< Temporary storage for tokens before persisting.
    unsigned long maxBufferSizeInBytes; ///< Maximum allowed buffer size in bytes.
    unsigned long currentSizeInBytes; ///< Current buffer size in bytes.

    /**
     * @brief Saves the current buffer to persistent storage and clears it.
     *
     * This method is automatically called when the buffer reaches its size limit.
     * It transfers all tokens from the buffer to the storage and resets the buffer.
     */
    void saveTo();

    /**
     * @brief Clears the buffer and resets its size counter.
     */
    void clearBuffer();

public:
    /**
     * @brief Constructs an Indexer instance with a specified buffer size limit.
     *
     * @param bufferSize Maximum buffer size (in bytes) before flushing to storage.
     */
    explicit Indexer(unsigned long bufferSize);

    /**
     * @brief Adds a new token to the buffer, handling overflow.
     *
     * If adding the token exceeds the buffer's size limit, the buffer is saved to storage and cleared.
     * The token is then added to the now-empty buffer.
     *
     * @param token The token to add.
     */
    void addToken(const Token& token);

    /**
     * @brief Retrieves BigToken information by token body.
     *
     * Searches the buffer for the BigToken associated with the provided body string.
     *
     * @param tokenName The body of the token to query.
     * @return Pointer to the BigToken instance if found, nullptr otherwise.
     */
    const BigToken* getTokenInfo(const std::string& tokenName) const;

    /**
     * @brief Returns the current buffer contents and clears the buffer.
     *
     * This method returns a copy of the buffer's contents and then clears it.
     *
     * @return A copy of the buffer's contents.
     */
    BufferType getBufferWithClear();

    /**
     * @brief Returns a constant reference to the current buffer contents.
     *
     * This method provides read-only access to the buffer's current state.
     *
     * @return A constant reference to the buffer.
     */
    const BufferType& getBuffer() const;

};

#endif // LIB_AFFIR_INDEXER_H