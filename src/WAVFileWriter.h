#ifndef WAVFILEWRITER_H
#define WAVFILEWRITER_H

#include <SD.h> // Include your SD library
#include <Arduino.h>

/**
 * @brief WAVFileWriter Class for creating and writing WAV audio files.
 * 
 * The `WAVFileWriter` class is designed to facilitate the creation and writing of WAV audio files 
 * on an SD card using the ESP32 platform. This class handles the construction of the WAV file header 
 * and allows for writing audio samples in either mono or stereo format, providing an easy interface 
 * for audio recording applications.
 * 
 * ### Key Features:
 * - **WAV File Header Management**: Automatically constructs the necessary WAV file header structure, 
 *   ensuring compliance with the WAV format specifications.
 * - **Audio Sample Writing**: Provides functionality to write audio frames, supporting both 
 *   mono and stereo configurations.
 * - **Flexible Configuration**: Allows configuration of audio parameters such as the number of channels, 
 *   sample rate, and duration, accommodating various audio recording needs.
 * - **Efficient File Handling**: Manages the opening and closing of files on the SD card efficiently, 
 *   ensuring that resources are properly released after use.
 * 
 * ### Example Usage:
 * 
 * ```cpp
 * #include "WAVFileWriter.h"
 * 
 * // Create a WAVFileWriter instance
 * WAVFileWriter wavWriter("audio.wav", 2, 44100, 60, "AudioFolder");
 * 
 * void setup() {
 *     // Initialize SD card and WAV file writer
 *     if (wavWriter.open()) {
 *         // Write audio frames
 *         wavWriter.writeFrame(leftSample, rightSample);
 *     }
 * }
 * 
 * void loop() {
 *     // Other application logic
 * }
 * 
 * // Close the WAV file when done
 * wavWriter.close();
 * ```
 * 
 * ### Conclusion:
 * The `WAVFileWriter` class is an essential component for any audio recording project on the ESP32, 
 * simplifying the process of creating and writing WAV audio files with minimal overhead and 
 * maximum efficiency.
 */

struct wav_header {
    char riff[4];           /* "RIFF"                                  */
    int32_t flength;        /* file length in bytes                    */
    char wave[4];           /* "WAVE"                                  */
    char fmt[4];            /* "fmt "                                  */
    int32_t chunk_size;     /* size of FMT chunk in bytes (usually 16) */
    int16_t format_tag;     /* 1=PCM, 257=Mu-Law, 258=A-Law, 259=ADPCM */
    int16_t num_chans;      /* 1=mono, 2=stereo                        */
    int32_t srate;          /* Sampling rate in samples per second     */
    int32_t bytes_per_sec;  /* bytes per second = srate*bytes_per_samp */
    int16_t bytes_per_samp; /* 2=16-bit mono, 4=16-bit stereo          */
    int16_t bits_per_samp;  /* Number of bits per sample               */
    char data[4];           /* "data"                                  */
    int32_t dlength;        /* data length in bytes (filelength - 44)  */
};

class WAVFileWriter {
public:
    // Constructor to initialize the WAV file writer
    WAVFileWriter(const char* file_name, short num_channels, int sample_rate, int duration_seconds, String Folder);
    ~WAVFileWriter();
    
    // Function to write a frame of audio samples
    void writeFrame(int16_t left_sample, int16_t right_sample);

    // Function to close the WAV file
    void close();

private:
    File m_file;                     // SD file object for writing
    wav_header m_header;             // WAV file header structure
    int32_t m_totalSamples;           // Total samples for the audio duration
    int16_t m_samplesWritten;         // Samples written so far
    short m_channels;                 // Number of audio channels (1 for mono, 2 for stereo)
    int32_t m_sampleRate;             // Sampling rate (e.g., 44100 Hz)

    // Additional private members can be declared here if necessary
};

#endif // WAVFILEWRITER_H
