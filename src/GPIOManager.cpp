#include "GPIOManager.h"

/**
 * @brief Constructs the GPIOManager and initializes GPIOs.
 * 
 * @param configManager Pointer to the configuration manager instance.
 */
GPIOManager::GPIOManager(){

}

/**
 * @brief Configures GPIO pins with the appropriate modes.
 * 
 * This function sets the modes for various GPIO pins, such as LEDs, buttons, and microphone
 * control pins. It ensures that the pins are configured as either inputs or outputs as needed.
 */
void GPIOManager::GPinitGPIOs() {
    if (DEBUGMODE) {
        Serial.println("###########################################################");
        Serial.println("#                    Initializing GPIO Manager             #");
        Serial.println("###########################################################");
    }

    // Set pin modes for LEDs, buttons, and microphone
    pinMode(LED_PIN, OUTPUT);       // Configure LED1 pin as output
    pinMode(BUTTON_01_PIN, INPUT_PULLUP);  // Configure Button1 pin as input with pull-up
    pinMode(BUTTON_02_PIN, INPUT_PULLUP);  // Configure Button2 pin as input with pull-up

}