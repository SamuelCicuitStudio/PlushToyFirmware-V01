#include "SPIFlashManager.h"

/**
 * @brief Constructor for the SPIFlashManager class.
 */
SPIFlashManager::SPIFlashManager() {}

/**
 * @brief Initializes the SPIFFS filesystem.
 * 
 * This method sets up the SPIFFS filesystem for use, allowing for file
 * operations such as reading and writing.
 */
void SPIFlashManager::begin() {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
    } else {
        Serial.println("SPIFFS Mounted Successfully");
    }
}

/**
 * @brief Writes data to a file in SPIFFS.
 * 
 * This method writes the provided data to a file on the SPIFFS filesystem.
 * 
 * @param filename The name of the file to write.
 * @param data Pointer to the data to write.
 * @param size The size of the data in bytes.
 * @return true if the file was written successfully, false otherwise.
 */
bool SPIFlashManager::writeFile(const String& filename, const uint8_t* data, size_t size) {
    File file = openFile(filename, "w");
    if (!file) {
        Serial.println("Failed to open file for writing");
        return false;
    }

    file.write(data, size);
    file.close();
    return true;
}

/**
 * @brief Reads data from a file in SPIFFS.
 * 
 * This method reads the contents of a file from the SPIFFS filesystem 
 * into the provided buffer.
 * 
 * @param filename The name of the file to read.
 * @param buffer Pointer to the buffer to store the read data.
 * @param bufferSize The size of the buffer.
 * @return true if the file was read successfully, false otherwise.
 */
bool SPIFlashManager::readFile(const String& filename, uint8_t* buffer, size_t bufferSize) {
    File file = openFile(filename, "r");
    if (!file) {
        Serial.println("Failed to open file for reading");
        return false;
    }

    size_t bytesRead = file.read(buffer, bufferSize);
    file.close();
    return (bytesRead > 0);
}

/**
 * @brief Checks if a file exists in SPIFFS.
 * 
 * @param filename The name of the file to check.
 * @return true if the file exists, false otherwise.
 */
bool SPIFlashManager::fileExists(const String& filename) {
    return SPIFFS.exists(filename);
}

/**
 * @brief Gets the size of a file in SPIFFS.
 * 
 * @param filename The name of the file.
 * @return The size of the file in bytes, or 0 if the file does not exist.
 */
size_t SPIFlashManager::getFileSize(const String& filename) {
    if (!fileExists(filename)) {
        Serial.println("File does not exist");
        return 0;
    }

    File file = SPIFFS.open(filename, "r");
    size_t size = file.size();
    file.close();
    return size;
}

/**
 * @brief Helper function to open a file.
 * 
 * @param filename The name of the file to open.
 * @param mode The mode to open the file in (e.g., "r" for reading, "w" for writing).
 * @return A File object representing the opened file.
 */
File SPIFlashManager::openFile(const String& filename, const char* mode) {
    return SPIFFS.open(filename, mode);
}
