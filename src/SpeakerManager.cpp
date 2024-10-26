#include "SpeakerManager.h"

// Constructor definition
SpeakerManager::SpeakerManager(
        WAVFileReader* wavfileReader,
        WAVFileWriter* wavfileWriter,
        MicManager* micManager,
        i2s_pin_config_t* i2sPins,
        I2SManager* i2SManager)
    : currentVolume(0), 
      i2SManager(i2SManager), 
      wavfileReader(wavfileReader), 
      wavfileWriter(wavfileWriter),  
      isPaused(false),
      i2sPins(i2sPins),
      micManager(micManager){}

/**
 * @brief Initializes the I2S amplifier and configures I2S pins.
 *
 * This method sets up the I2S pins for audio output and initializes the
 * I2S amplifier. It prints the initialization status to the Serial monitor
 * if DEBUGMODE is enabled.
 *
 * It should be called before any playback or recording starts to ensure
 * proper configuration of the audio hardware.
 */
void SpeakerManager::begin() {
    if (DEBUGMODE) {
        Serial.println("###########################################################");
        Serial.println("#              Starting Speaker Manager                   #");
        Serial.println("###########################################################");
    }
    // Configure I2S pins
    i2sPins = new i2s_pin_config_t;
    i2sPins ->bck_io_num = I2S_BCLK_PIN,
    i2sPins ->ws_io_num = I2S_SD_MODE_PIN,
    i2sPins ->data_out_num = I2S_DIN_PIN,
    i2sPins ->data_in_num = I2S_PIN_NO_CHANGE;
    
    if (DEBUGMODE) {
        Serial.println("SpeakerManager: I2S amplifier initialized successfully.");
    }
}

// Starts playback with the given WAV file.
void SpeakerManager::startPlayback(const char* file_name) {
    esp_task_wdt_reset(); // Reset watchdog timer
    stopPlayback(); // Clean up previous resources

    // Initialize WAV file reader
    wavfileReader = new WAVFileReader(file_name, *i2sPins);

    // Open the new WAV file
    if (wavfileReader && wavfileReader->open()) { // Open the specified WAV file
        wavfileReader->startPlayback(); // Start playback in the background
    } else {
        Serial.println("Failed to open WAV file for playback.");
        delete wavfileReader; // Clean up if failed to open
        wavfileReader = nullptr; // Avoid dangling pointer
    }
}

// Stops playback and cleans up resources.
void SpeakerManager::stopPlayback() {
    if (wavfileReader) {
        wavfileReader->stopPlayback(); // Stop the WAV file playback
        delete wavfileReader; // Delete the WAV file reader to free memory
        wavfileReader = nullptr; // Avoid dangling pointer
    }
}

// Pauses the current playback.
void SpeakerManager::pausePlayback() {
    if (wavfileReader) {
        wavfileReader->pausePlayback(); // Implement pause in WAVFileReader
    }
}

// Resumes the paused playback.
void SpeakerManager::resumePlayback() {
    if (wavfileReader) {
        wavfileReader->resumePlayback(); // Implement resume in WAVFileReader
    }
}

// Sets the volume level for the I2S amplifier.
void SpeakerManager::setVolume(int volume) {
    currentVolume = constrain(volume, 0, 100); // Ensure volume is within range (0-100)
    // Code to set volume in the I2S amplifier would go here

    if (DEBUGMODE) {
        Serial.print("SpeakerManager: Volume set to ");
        Serial.println(currentVolume);
    }
}
/**
 * @brief Starts recording to a WAV file.
 *
 * @param file_name The name of the WAV file to record to. The file will 
 *                  be created if it does not exist, and must be in a valid 
 *                  format to ensure proper recording.
 *
 * This method stops any ongoing playback before initializing the WAV file writer.
 * It should be called to initiate audio recording to a specified WAV file.
 */
void SpeakerManager::startRecording() {
    // Clean up previous resources
    if (DEBUGMODE)Serial.println("SpeakerManager: Stop Playback.");
    stopPlayback(); // Ensure playback is stopped before recording
}

/**
 * @brief Stops recording and cleans up resources.
 *
 * This method closes the WAV file writer and frees up memory. It should 
 * be called when recording is no longer needed to prevent resource leaks.
 */
void SpeakerManager::stopRecording() {
    if (wavfileWriter) {
        wavfileWriter->close(); // Ensure writing is stopped
        delete wavfileWriter;
        wavfileWriter = nullptr;
        
    if (DEBUGMODE) {
        Serial.println("SpeakerManager: Recording stopped.");
    };
    }
}

/**
 * @brief Periodically reads audio data and writes to the WAV file.
 *
 * This method captures audio data from the microphone and writes the
 * data to the currently active WAV file. It should be called regularly
 * to ensure continuous recording.
 *
 * The method reads the output from the microphone manager and converts 
 * the data into a format suitable for writing to the WAV file.
 */
void SpeakerManager::recordAudio(const int duration_seconds, const char *file_name, const int sample_rate,String Folder) {
    // Calculate total samples to record based on the recording length and sample rate
    const int buffer_size = sample_rate * (duration_seconds / 1000); 
    buffer = new short int[buffer_size]; // Allocate buffer for audio samples
    int sampleIndex = 0; // To track the number of samples recorded

    // Create the WAV file writer before starting the recording
    wavfileWriter = new WAVFileWriter(file_name, CHANNEL, sample_rate, duration_seconds / 1000,Folder);

    esp_task_wdt_reset();
    unsigned long startTime = micros(); // Use micros for higher precision
    unsigned long sampleInterval = 1000000 / sample_rate; // Calculate interval in microseconds
    unsigned long nextSampleTime = startTime; // Initialize next sample time

    // Record audio for the specified duration
    while (micros() - startTime < (duration_seconds * 1000)) {
        esp_task_wdt_reset();

        // Check if it's time to read the next sample
        if (micros() >= nextSampleTime) {
            // Read microphone output values
            if (sampleIndex < buffer_size) { // Ensure we don't exceed the buffer size
                buffer[sampleIndex] = micManager->readOutput(); // Read sample from the microphone
                sampleIndex++; // Move to the next sample index
            }
            // Update the time for the next sample
            nextSampleTime += sampleInterval; 
        };
        // Check if the stop button is pressed
        if (!digitalRead(BUTTON_02_PIN)) {
            delay(50); // Adjusted debounce delay for better responsiveness
            Serial.println("Stop button pressed");
            // Wait until button is released
            while (!digitalRead(BUTTON_02_PIN)) {
                delay(10); // Small delay to prevent rapid looping
            }
            break; // Exit the recording loop if button is pressed
        }
    };


    // Write the recorded samples to the WAV file
    for(int i = 0; i< buffer_size; i++) wavfileWriter->writeFrame (buffer[i],buffer[i]); // Use a single function to write all frames
    
    wavfileWriter->close(); // Close the WAV file
    delete[] buffer; // Free the allocated buffer
    esp_task_wdt_reset();
}