#include "WAVFileReader.h"
#include "Config.h"

/**
 * @brief Constructor to initialize the WAV file reader.
 * 
 * @param file_name The name of the WAV file to be read.
 * @param i2sPins The pin configuration for I2S output.
 */
WAVFileReader::WAVFileReader(const char* file_name, i2s_pin_config_t i2sPins) 
    : m_i2sPins(i2sPins), m_currentPos(0), m_playbackState(STOPPED), xPlaybackTask(NULL) {

    // Attempt to open the WAV file
    m_file = SD.open(file_name);
    if (!m_file) {
        Serial.println("Failed to open WAV file.");
        return; // Early exit if file cannot be opened
    }

    // Create binary semaphore for synchronization
    xSemaphore = xSemaphoreCreateBinary();
    if (xSemaphore == NULL) {
        Serial.println("Failed to create semaphore.");
    }
}

/**
 * @brief Destructor to close the file and clean up.
 */
WAVFileReader::~WAVFileReader() {
    stopPlayback(); // Ensure playback is stopped before cleanup
    if (m_file) {
        m_file.close(); // Close the WAV file if it is open
    }
    delete m_i2sOutput; // Clean up I2SOutput if allocated
}

/**
 * @brief Open the WAV file and read the header.
 * 
 * @return true if the WAV file was opened successfully; false otherwise.
 */
bool WAVFileReader::open() {
    if (!m_file) {
        Serial.println("Failed to open WAV file.");
        return false; // Ensure the file is open before reading
    }

    // Read the WAV header
    if (m_file.read((uint8_t*)&m_header, sizeof(m_header)) != sizeof(m_header)) {
        Serial.println("Failed to read WAV header.");
        return false; // Ensure the header is read successfully
    }

    m_dataSize = m_header.dlength; // Set data size from header
    m_currentPos = 0; // Reset current position

    // Initialize I2SOutput with the sample rate
    m_i2sOutput = new I2SManager(m_i2sPins, m_header.srate);
    if (!m_i2sOutput) {
        Serial.println("Failed to initialize I2S output.");
        return false; // Ensure I2SOutput is initialized successfully
    }
    m_i2sOutput->begin(); // Start I2S output

    return true; // Return true if successful
}

/**
 * @brief Playback task implementation.
 * 
 * This function runs as a FreeRTOS task and continuously reads samples 
 * from the WAV file and sends them to the I2S output while the playback 
 * state is PLAYING.
 * 
 * @param parameter A pointer to the WAVFileReader instance.
 */
void WAVFileReader::playbackTask(void* parameter) {
    WAVFileReader* reader = static_cast<WAVFileReader*>(parameter);
    int16_t sample;

    while (true) {
        switch (reader->m_playbackState) {
            case PLAYING:
                if (reader->readSample(sample)) {
                    reader->m_i2sOutput->writeSample(sample);
                } else {
                    // End of data reached
                    reader->stopPlayback(); // Stop playback if end is reached
                    return; // Exit the loop
                }
                break;

            case PAUSED:
                xSemaphoreTake(reader->xSemaphore, portMAX_DELAY); // Wait until resumed
                break;

            case STOPPED:
                vTaskDelete(NULL); // Exit task if stopped
                return;
        }
        vTaskDelay(1); // Yield to other tasks
    }
}

/**
 * @brief Start playback of the WAV file.
 * 
 * This function creates a task to handle playback if it is currently stopped.
 */
void WAVFileReader::startPlayback() {
    if (m_playbackState == STOPPED) {
        m_playbackState = PLAYING; // Change state to PLAYING
        xTaskCreate(playbackTask, "PlaybackTask", READING_STACK_SIZE, this, 1, &xPlaybackTask);
    }
}

/**
 * @brief Stop playback of the WAV file.
 * 
 * This function stops the playback task and resets the current position.
 */
void WAVFileReader::stopPlayback() {
    if (m_playbackState != STOPPED) {
        m_playbackState = STOPPED; // Set playback state to STOPPED
        if (xPlaybackTask) {
            vTaskDelete(xPlaybackTask); // Delete playback task
            xPlaybackTask = NULL; // Reset task handle
        }
        if (m_i2sOutput) {
            m_i2sOutput->stop(); // Stop the I2S output if initialized
        }
        m_currentPos = 0; // Reset current position
    }
}

/**
 * @brief Pause playback of the WAV file.
 * 
 * This function changes the playback state to PAUSED if it is currently playing.
 */
void WAVFileReader::pausePlayback() {
    if (m_playbackState == PLAYING) {
        m_playbackState = PAUSED; // Change state to PAUSED
    }
}

/**
 * @brief Resume playback of the WAV file.
 * 
 * This function changes the playback state back to PLAYING and releases the semaphore 
 * to continue playback if it is currently paused.
 */
void WAVFileReader::resumePlayback() {
    if (m_playbackState == PAUSED) {
        m_playbackState = PLAYING; // Change state back to PLAYING
        xSemaphoreGive(xSemaphore); // Release semaphore to allow playback to continue
    }
}

/**
 * @brief Read a sample from the WAV file.
 * 
 * @param sample Reference to an int16_t variable where the read sample will be stored.
 * @return true if a sample was read successfully; false if end of data is reached.
 */
bool WAVFileReader::readSample(int16_t &sample) {
    if (m_currentPos >= m_dataSize) {
        return false; // End of data
    }

    // Read a sample (2 bytes for 16-bit audio)
    size_t bytesRead = m_file.read((uint8_t*)&sample, sizeof(sample));
    if (bytesRead != sizeof(sample)) {
        return false; // Return false if read fails
    }

    m_currentPos += sizeof(sample); // Update current position
    return true; // Return true if a sample was read
}

/**
 * @brief Check if the end of the data is reached.
 * 
 * @return true if the end of the data is reached; false otherwise.
 */
bool WAVFileReader::isEnd() {
    return (m_currentPos >= m_dataSize); // Check if current position exceeds data size
}

/**
 * @brief Get the sample rate from the WAV header.
 * 
 * @return The sample rate as an integer.
 */
int WAVFileReader::getSampleRate() {
    return m_header.srate; // Return the sample rate from the header
}