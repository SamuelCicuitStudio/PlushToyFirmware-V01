#ifndef I2SMANAGER_H
#define I2SMANAGER_H

/**
 * @file I2SManager.h
 * @brief Provides an interface to control I2S audio playback on the ESP32.
 * 
 * The I2SManager class facilitates initializing and managing I2S audio communication
 * on the ESP32, including playback control (play, pause, resume, stop) and handling
 * audio samples for real-time audio output. This class is designed for applications 
 * requiring stereo audio output with a configurable sample rate.
 * 
 * Usage Example:
 * @code
 * i2s_pin_config_t pins = {
 *     .bck_io_num = 26,
 *     .ws_io_num = 25,
 *     .data_out_num = 22,
 *     .data_in_num = I2S_PIN_NO_CHANGE
 * };
 * int sampleRate = 44100;
 * 
 * I2SManager i2sManager(pins, sampleRate);
 * i2sManager.begin();
 * i2sManager.writeSample(sample);
 * i2sManager.pause();
 * i2sManager.resume();
 * i2sManager.stop();
 * @endcode
 */
#include <Arduino.h>
#include <driver/i2s.h>
#include "OtaManager.h"

class I2SManager {
public:
    I2SManager(i2s_pin_config_t pins, int sample_rate);
    void begin();
    void writeSample(int16_t sample);
    void pause();
    void resume();
    void stop();
    bool isPlaying();

private:
    i2s_config_t i2s_config;
    bool playing;
};

#endif // I2SMANAGER_H