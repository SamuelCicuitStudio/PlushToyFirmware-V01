#ifndef WAVFILEREADER_H
#define WAVFILEREADER_H

#include <Arduino.h>
#include <SD.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "I2SManager.h"  // Include the I2SOutput header

/**
 * @file WAVFileReader.h
 * @brief WAV File Reader Class for ESP32
 *
 * The `WAVFileReader` class provides functionality to read and play back WAV audio files from an SD card
 * using the ESP32 platform. It manages the WAV file format, including parsing the file header and 
 * streaming audio samples through I2S output. The class leverages FreeRTOS for handling playback in 
 * a separate task, allowing for efficient audio streaming without blocking the main program flow.
 *
 * ## Key Features:
 * - Reads WAV files and extracts audio data from the SD card.
 * - Provides playback control (play, pause, stop, resume).
 * - Utilizes I2S for audio output to speakers or other audio devices.
 * - Supports checking the playback state and ensuring smooth audio handling.
 *
 * ## Usage:
 * 1. Instantiate the `WAVFileReader` with the desired file name and I2S pin configuration.
 * 2. Call the `open()` method to prepare the WAV file for playback.
 * 3. Use `startPlayback()`, `pausePlayback()`, and `stopPlayback()` to control audio playback.
 * 4. Monitor the playback state and check for end-of-file conditions using `isEnd()`.
 *
 * ## Dependencies:
 * - Arduino core libraries
 * - SD library for file handling
 * - FreeRTOS for multitasking
 *
 * ## Example:
 * ```cpp
 * WAVFileReader wavReader("audio.wav", i2sPins);
 * if (wavReader.open()) {
 *     wavReader.startPlayback();
 * }
 * ```
 */

// WAV file header structure
struct wav_header_ {
    char riff[4];           // "RIFF"
    int32_t flength;        // File length
    char wave[4];           // "WAVE"
    char fmt[4];            // "fmt "
    int32_t chunk_size;     // Size of the fmt chunk
    int16_t format_tag;     // Format tag
    int16_t num_chans;      // Number of channels
    int32_t srate;          // Sample rate
    int32_t bytes_per_sec;  // Bytes per second
    int16_t bytes_per_samp; // Bytes per sample
    int16_t bits_per_samp;  // Bits per sample
    char data[4];           // "data"
    int32_t dlength;        // Data length
};

class WAVFileReader {
public:
    enum PlaybackState { STOPPED, PLAYING, PAUSED }; // Playback states

    WAVFileReader(const char* file_name, i2s_pin_config_t i2sPins);
    ~WAVFileReader();
    bool open();
    void startPlayback();  // Start playback
    void stopPlayback();   // Stop playback
    void pausePlayback();  // Pause playback
    void resumePlayback(); // Resume playback
    bool isEnd();          // Check if end of data is reached
    int getSampleRate();   // Get the sample rate
    bool readSample(int16_t &sample); // Read a sample from the WAV file

private:
    static void playbackTask(void* parameter); // FreeRTOS task for playback
    File m_file;                // File object for WAV file
    wav_header_ m_header;        // WAV file header
    int32_t m_dataSize;        // Size of the audio data
    int32_t m_currentPos;      // Current position in the audio data
    I2SManager* m_i2sOutput;    // Pointer to I2SOutput object
    i2s_pin_config_t m_i2sPins; // Pin configuration for I2S output
    TaskHandle_t xPlaybackTask; // Task handle for playback task
    volatile PlaybackState m_playbackState; // Current playback state
    SemaphoreHandle_t xSemaphore; // Semaphore for synchronization
};

#endif // WAVFILEREADER_H
