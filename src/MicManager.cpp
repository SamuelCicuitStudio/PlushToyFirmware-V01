#include "MicManager.h"

/**
 * @brief Constructor for MicManager class.
 * 
 * This constructor initializes the MicManager with a reference to the ConfigManager instance.
 * 
 * @param configManager Pointer to the ConfigManager instance, which handles configuration data.
 */
MicManager::MicManager(){}

/**
 * @brief Initializes the microphone system.
 * 
 * This method configures the necessary GPIO pins for the microphone system, sets the initial states 
 * for the gain and auto-gain control pins based on the configuration, and sets the default gain value. 
 * It prints debug information if `DEBUGMODE` is enabled.
 */
void MicManager::begin() {
    if (DEBUGMODE) {
        Serial.println("###########################################################");
        Serial.println("#                  Starting Mic Manager                   #");
        Serial.println("###########################################################");
    }
    // Configure the pins for microphone input and control
   // pinMode(MIC_OUT_PIN, INPUT);         // Microphone output pin as input
    pinMode(MIC_GAIN_PIN, OUTPUT);       // Gain control pin as output
    pinMode(MIC_AR_PIN, OUTPUT);         // Auto-gain control pin as output

    analogReadResolution(MIC_RESOLUTION);// Set analog pin resolution

    // Set initial states from configuration
    digitalWrite(MIC_GAIN_PIN, LOW);  // Initialize gain control state
    digitalWrite(MIC_AR_PIN, LOW);  // Initialize auto-gain control state

    // Initialize the microphone with default gain settings
    setGain(1); // Default gain

    if (DEBUGMODE) {
        Serial.println("MicManager: Microphone initialized successfully.");
    }
}

/**
 * @brief Sets the microphone gain.
 * 
 * This method adjusts the microphone gain by controlling the gain pin. It constrains the gain value to 
 * a predefined range (0 to 5, adjust as needed) and sets the gain pin accordingly. 
 * Debug information is printed if `DEBUGMODE` is enabled.
 * 
 * @param gain The desired gain value for the microphone (typically constrained to a certain range).
 */
void MicManager::setGain(int gain) {
    // Constrain the gain value within the allowed range
    int gainValue = constrain(gain, 0, 5); // Adjust the range as necessary for the hardware

    // Set the gain control pin based on the constrained value
    digitalWrite(MIC_GAIN_PIN, gainValue); // Adjust this to fit the hardware control mechanism

    if (DEBUGMODE) {
        Serial.print("MicManager: Gain set to ");
        Serial.println(gainValue);
    }
}

/**
 * @brief Reads the microphone output value.
 * 
 * This method reads the analog value from the microphone output pin and returns it. 
 * Debug information is printed if `DEBUGMODE` is enabled.
 * 
 * @return The current microphone output value (an integer representing the analog read result).
 */
int MicManager::readOutput() {
    int micValue = analogRead(MIC_OUT_PIN); // Read the microphone output value
    // Map the 10-bit ADC value (0-1024) to a signed 16-bit range (-1000 to 1000)
    int16_t convertedValue =  map(micValue, MIC_RESOLUTION_MIN, MIC_RESOLUTION_MAX, WAV_RESOLUTION_MIN, WAV_RESOLUTION_MAX); 
    return convertedValue; // Return the converted value
}