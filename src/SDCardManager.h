#ifndef SDCARD_MANAGER_H
#define SDCARD_MANAGER_H
/**
 * @file SDCardManager.h
 * @brief Manages SD card initialization and file handling for audio recordings.
 *
 * The SDCardManager class provides a simple interface for managing SD card functionality, including
 * initializing the SD card, retrieving the name of the next file for recording, and accessing the 
 * last recorded file. It utilizes the SD library to interact with the SD card filesystem, enabling
 * applications to record, store, and access audio files with ease.
 * 
 * ## Key Features
 * - **SD Card Initialization:** Simplifies the process of initializing and verifying SD card readiness.
 * - **Filename Management:** Generates filenames for sequential recordings, assisting with file organization.
 * - **File Retrieval:** Accesses the most recent recorded filename for playback or other operations.
 * 
 * ## Dependencies
 * This class depends on:
 * - The `SD.h` library for SD card filesystem management.
 * - `SPI.h` for SPI communication with the SD card.
 * - `I2SManager.h` for potential integration with audio playback or recording.
 * 
 * ## Example Usage
 * ```
 * SDCardManager sdManager;
 * sdManager.begin();
 * String nextFile = sdManager.getNextRecordingFilename();
 * ```
 * 
 * This class is designed for embedded systems such as ESP32, where audio data is recorded and saved to an SD card.
 * 
 * @note Ensure the SD card is correctly inserted and initialized before calling filename management methods.
 */
#include "I2SManager.h"
#include <SD.h>
#include <SPI.h>

class SDCardManager {
public:
    // Constructor, accepting a ConfigManager object for managing configuration
    SDCardManager();

    // Initialize the SD card
    void begin();
    String getNextRecordingFilename();
    String getLastRecordedFilename();
};

#endif // SDCARD_MANAGER_H