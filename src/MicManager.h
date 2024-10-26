#ifndef MIC_MANAGER_H
#define MIC_MANAGER_H
/**
 * @file MicManager.h
 * @brief Header file for the MicManager class, which provides functionality to control 
 * and read data from an I2S microphone, including gain adjustment and high-pass filtering.
 * 
 * The MicManager class encapsulates microphone setup, gain configuration, and output filtering.
 * Designed for use with I2S-based microphones, it filters microphone input with a high-pass 
 * filter to remove DC offset and low-frequency noise, delivering cleaner audio data for 
 * processing or playback.
 * 
 * @dependencies 
 * - I2SManager.h: Provides control over the I2S interface used to communicate with the microphone.
 * - Arduino.h: Required for general Arduino functions and types.
 * 
 * @note Set up an I2SManager instance before using MicManager for audio data to ensure proper 
 * I2S initialization and audio handling.
 */
#include "I2SManager.h"
#include"Arduino.h"

class MicManager {
public:
    MicManager();// Constructor, accepting a ConfigManager object for managing configuration
    void begin();// Initialize the microphone
    void setGain(int gain);// Set the microphone gain
    int readOutput();// Read the microphone output value
private:
 float alpha = 0.98;                  // Filter coefficient for the high-pass filter
    float lastMicValue = 0.0;            // Previous microphone value for high-pass filter
    float filteredMicValue = 0.0;        // High-pass filtered microphone value

};

#endif // MIC_MANAGER_H