#include "SDCardManager.h"

/**
 * @brief Constructor for the SDCardManager class.
 * 
 * @param configManager Pointer to the ConfigManager object for managing configurations.
 */
SDCardManager::SDCardManager() {}

/**
 * @brief Initializes the SD card and prepares the environment for recording files.
 * 
 * This method initializes the SD card using the defined SPI pins. It also creates 
 * a recording folder if it doesn't already exist.
 */
void SDCardManager::begin() {
    if (DEBUGMODE) {
        Serial.println("###########################################################");
        Serial.println("#                 Starting SD card Manager                #");
        Serial.println("###########################################################");
    }

    // Set up SPI pins for SD card
    SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN);
    
    // Initialize the SD card
    if (!SD.begin(SPI_CS_SD_PIN)) {
        if (DEBUGMODE) {
            Serial.println("SDCardManager: SD Card initialization failed!");
        }
    } else {
        if (DEBUGMODE) {
            Serial.println("SDCardManager: SD Card initialized successfully.");
        }
    }

    // Create the recording folder if it doesn't exist
    
    if (!SD.exists(RECORDING_FOLDER_PATH)) {
        SD.mkdir(RECORDING_FOLDER_PATH);
        if (DEBUGMODE) {
            Serial.println("Created WebRecording directory");
        }
    }
}

/**
 * @brief Retrieves the next available recording filename.
 * 
 * This function scans the SD card for existing recording files and constructs
 * the next available filename using an incremented index (e.g., recording01).
 * 
 * @return String The next available recording filename without extension or path.
 */
String SDCardManager::getNextRecordingFilename() {
    int index = 1;
    String filename;

    // Loop to determine the next available filename
    do {
        // Construct the filename without the folder path and extension
        filename = String(BASED_RECORDING_NAME) + 
                   (index < 10 ? "0" : "") + String(index); // Zero-padded filename
        index++;
    } while (SD.exists(String(RECORDING_FOLDER_PATH) + "/" + filename + String(EXTENSION))); // Check for existing files on the SD card

    return filename;  // Return the next available filename without extension or path
}



/**
 * @brief Retrieves the filename of the most recent recorded file.
 * 
 * This method scans the recording directory for files and returns the filename
 * of the most recently modified recording.
 * 
 * @return String The filename of the latest recorded file.
 */
String SDCardManager::getLastRecordedFilename() {
    String latestFilename;
    File dir = SD.open(RECORDING_FOLDER_PATH);  // Open the recording directory
    File latestFile;
    uint32_t latestTime = 0;  // Variable to track the latest modification time

    // Check if the directory can be opened
    if (!dir) {
        Serial.println("Failed to open directory");
        return "";  // Return an empty string if the directory can't be opened
    }

    // Iterate through files in the directory
    while (File file = dir.openNextFile()) {
        if (!file.isDirectory() && String(file.name()).endsWith(EXTENSION)) {
            uint32_t modificationTime = file.getLastWrite();  // Get file's modification time
            if (modificationTime > latestTime) {
                latestTime = modificationTime;
                latestFilename = String(file.name());  // Update the latest filename
            }
        }
        file.close();  // Close the file after reading
    }

    if (latestFilename.length() > 0) {
        Serial.print("Latest recorded file: ");
        Serial.println(latestFilename);  // Log the latest filename
    } else {
        Serial.println("No WAV files found in the directory.");
    }

    return latestFilename;  // Return the latest recorded filename
}