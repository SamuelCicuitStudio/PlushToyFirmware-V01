#include "WAVFileWriter.h"
#include <string.h> // For memcpy
#include "I2SManager.h"

/**
 * @brief Constructs a WAVFileWriter object.
 * 
 * Initializes the WAV file header and opens the specified file for writing.
 *
 * @param file_name The name of the file to write the WAV data to.
 * @param num_channels The number of audio channels (1 for mono, 2 for stereo).
 * @param sample_rate The sample rate in Hz (e.g., 44100 for CD quality).
 */
WAVFileWriter::WAVFileWriter(const char *file_name, short num_channels, int sample_rate, int duration_seconds,String Folder) {
    // Construct the full file path
    String fullPath = Folder + "/" + String(file_name) + String(EXTENSION);
    m_file = SD.open(fullPath.c_str(), FILE_WRITE); // Open the file for writing on the SD card

    // Calculate number of samples needed for the specified duration
    m_totalSamples = sample_rate * duration_seconds; // Total samples for the duration
    m_channels = num_channels;
    m_sampleRate = sample_rate;

    // Initialize the WAV header
    strncpy(m_header.riff, "RIFF", 4);
    strncpy(m_header.wave, "WAVE", 4);
    strncpy(m_header.fmt, "fmt ", 4);
    strncpy(m_header.data, "data", 4);

    m_header.chunk_size = 16; // PCM format chunk size
    m_header.format_tag = 1;  // PCM format
    m_header.num_chans = num_channels; // 1 for mono, 2 for stereo
    m_header.srate = sample_rate;// Sample rate
    m_header.bits_per_samp = 16;// 16 bits per sample
    m_header.bytes_per_sec = m_header.srate * m_header.bits_per_samp / 8 * m_header.num_chans;
    m_header.bytes_per_samp = m_header.bits_per_samp / 8 * m_header.num_chans;

    m_header.dlength = m_totalSamples * m_channels * 2; // Data length for the total samples
    m_header.flength = m_header.dlength + sizeof(m_header) - 8; // Total file length

    // Write the initial header (it will be updated later on close)
    m_file.write((uint8_t*)&m_header, sizeof(m_header));

    // Initialize sample counter
    m_samplesWritten = 0;
}


/**
 * @brief Destroys the WAVFileWriter object.
 * 
 * Ensures that the file is closed properly and the WAV header is updated.
 */
WAVFileWriter::~WAVFileWriter() {
    close();
}

/**
 * @brief Writes a single audio frame to the WAV file.
 * 
 * Writes the left and right audio samples of a frame to the file, updating the 
 * data byte count accordingly.
 *
 * @param left_sample Left channel sample (for mono, this will be the only sample).
 * @param right_sample Right channel sample (optional, for stereo).
 */
void WAVFileWriter::writeFrame(int16_t left_sample, int16_t right_sample) {
    // Only write if we have not exceeded the total samples for the duration
    if (m_samplesWritten < m_totalSamples) {
        // Write left channel sample (always write, even in mono)
        m_file.write((uint8_t*)&left_sample, sizeof(left_sample)); // Write left channel sample

        // If in stereo mode, write the right channel sample
        if (CHANNEL == 2) {
            m_file.write((uint8_t*)&right_sample, sizeof(right_sample)); // Write right channel sample
        }

        // Increment the number of samples written
        m_samplesWritten++;
    } else {
        // If we have reached the maximum sample count, you may choose to handle it.
        // For example, logging a message or resetting the written count can be done here.
    }

    // Reset watchdog timer to prevent task reset
    esp_task_wdt_reset();
}


/**
 * @brief Closes the WAV file and updates the WAV header with correct sizes.
 * 
 * This method finalizes the WAV file by updating the header with the correct 
 * data size and closing the file.
 */
void WAVFileWriter::close() {
    // Update the WAV header length before closing
    m_header.dlength = m_samplesWritten * m_channels * 2; // Update data length based on actual samples written
    m_header.flength = m_header.dlength + sizeof(m_header) - 8; // Update total file length

    // Write the updated header to the file
    m_file.seek(0); // Go back to the start of the file
    m_file.write((uint8_t*)&m_header, sizeof(m_header)); // Write the updated header
    m_file.close(); // Close the file
        if (DEBUGMODE) {
        Serial.println("SpeakerManager: Recording stopped.");
    };

}
