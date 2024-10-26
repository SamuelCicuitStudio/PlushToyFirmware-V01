#ifndef SPIFLASH_MANAGER_H
#define SPIFLASH_MANAGER_H

#include <Arduino.h>
#include <SPIFFS.h>


/**
 * @class SPIFlashManager
 * @brief Manages file operations on the SPIFFS filesystem for ESP32.
 *
 * The SPIFlashManager class provides a straightforward interface for 
 * managing files on the SPIFFS filesystem. It allows for initializing 
 * the SPIFFS environment, writing data to files, reading data from 
 * files, checking for file existence, and retrieving file sizes. 
 * This class is designed to facilitate the storage and retrieval 
 * of arbitrary file types in embedded applications, enhancing 
 * the ability to manage resources in an ESP32 environment.
 *
 * Usage:
 * - Create an instance of SPIFlashManager.
 * - Call the `begin()` method to initialize the SPIFFS filesystem.
 * - Use `writeFile()` to write data to a file.
 * - Use `readFile()` to read data from a file.
 * - Use `fileExists()` to check if a specific file is available.
 * - Use `getFileSize()` to obtain the size of a file.
 *
 * Example:
 * @code
 * SPIFlashManager flashManager;
 * flashManager.begin();
 * const char* data = "Hello, World!";
 * flashManager.writeFile("/hello.txt", (const uint8_t*)data, strlen(data));
 * @endcode
 */
class SPIFlashManager {
public:
    
    SPIFlashManager();// Constructor
    void begin();// Initialize the SPIFFS filesystem
    bool writeFile(const String& filename, const uint8_t* data, size_t size);// Write data to a file in SPIFFS
    bool readFile(const String& filename, uint8_t* buffer, size_t bufferSize);// Read data from a file in SPIFFS
    bool fileExists(const String& filename);// Check if a file exists in SPIFFS
    size_t getFileSize(const String& filename);// Get the size of a file

private:
    
    File openFile(const String& filename, const char* mode);// Helper function to open a file
};

#endif // SPIFLASH_MANAGER_H
