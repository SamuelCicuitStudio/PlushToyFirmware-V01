#include "I2SManager.h"

/**
 * @brief Constructs the I2SManager with the specified pin configuration and sample rate.
 * 
 * Initializes the I2S configuration with parameters suitable for 16-bit stereo audio.
 * It also installs the I2S driver and sets the provided pins for the I2S communication.
 * 
 * @param pins Configuration structure defining the GPIO pins for I2S signals.
 * @param sample_rate The sample rate for audio data, typically 44100Hz or 48000Hz for audio playback.
 */
I2SManager::I2SManager(i2s_pin_config_t pins, int sample_rate) {
    i2s_config.sample_rate = sample_rate;
    i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
    i2s_config.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
    i2s_config.communication_format = I2S_COMM_FORMAT_I2S;
    i2s_config.dma_buf_count = 8;
    i2s_config.dma_buf_len = 1024;

    i2s_set_pin(I2S_NUM_0, &pins);
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);

    playing = false;
}

/**
 * @brief Begins I2S audio streaming.
 * 
 * Starts the I2S driver, allowing audio samples to be written to the I2S peripheral.
 * Sets the `playing` flag to true, indicating that I2S streaming is active.
 */
void I2SManager::begin() {
    i2s_start(I2S_NUM_0);
    playing = true;
}

/**
 * @brief Writes a single audio sample to the I2S peripheral.
 * 
 * Sends the provided 16-bit sample to the I2S hardware, blocking until the sample is 
 * written to the I2S buffer. Only writes if `playing` is true.
 * 
 * @param sample The 16-bit signed integer sample to send to the I2S peripheral.
 */
void I2SManager::writeSample(int16_t sample) {
    if (playing) {
        size_t bytes_written;
        i2s_write(I2S_NUM_0, &sample, sizeof(sample), &bytes_written, portMAX_DELAY);
    }
}

/**
 * @brief Pauses the I2S audio stream.
 * 
 * Stops the I2S driver, effectively pausing audio output. Sets `playing` to false to
 * indicate that I2S streaming is inactive.
 */
void I2SManager::pause() {
    if (playing) {
        i2s_stop(I2S_NUM_0);
        playing = false;
    }
}

/**
 * @brief Resumes the I2S audio stream.
 * 
 * Restarts the I2S driver if it was previously paused, setting the `playing` flag to true.
 */
void I2SManager::resume() {
    if (!playing) {
        i2s_start(I2S_NUM_0);
        playing = true;
    }
}

/**
 * @brief Stops the I2S audio stream.
 * 
 * Completely halts the I2S driver and sets `playing` to false. Should be used when
 * terminating audio output.
 */
void I2SManager::stop() {
    if (playing) {
        i2s_stop(I2S_NUM_0);
        playing = false;
    }
}

/**
 * @brief Checks if the I2S stream is currently active.
 * 
 * @return true if the I2S stream is active, false otherwise.
 */
bool I2SManager::isPlaying() {
    return playing;
}
