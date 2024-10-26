#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H
/**
 * @brief Reads the battery voltage from the BQ2589x battery management IC.
 *
 * This function retrieves the current battery voltage by reading
 * from the ADC (Analog-to-Digital Converter) of the BQ2589x device.
 * The voltage is calculated based on a base voltage and a scaling
 * factor defined by the device's specifications. The returned value
 * is expressed in millivolts (mV).
 *
 * @return int The battery voltage in millivolts (mV). A positive 
 * value indicates a successful read, while a negative value indicates 
 * an error in reading from the device. In the case of an error,
 * the function will return the error code.
 *
 * @note Ensure that the BQ2589x is properly initialized and powered
 * on before calling this function to guarantee accurate readings.
 */

#include <Arduino.h>
#include <Wire.h>
#include <BQ2589x.h>
#include "ConfigManager.h"

class PowerManager {
public:
    // Constructor
    PowerManager();

    // Initialize power management settings
    void begin();
    void setChargingCurrent(uint16_t current);
    void setChargingVoltage(uint16_t voltage);
    void setShipMode(bool enable);
    void enterSleepMode(uint32_t duration);
    void wakeUp();
    uint8_t getBatteryLevel();
    float readThermistor();
    bool isBatteryLow();

private:
    // Private members for power management
    uint16_t chargingCurrent; ///< Current for charging in milliamps
    uint16_t chargingVoltage; ///< Voltage for charging in millivolts
    void initBatteryManagement();
    uint16_t readBatteryVoltage();
    uint8_t calculateBatteryLevel(uint16_t voltage);
    bq2589x CHARGER;
};

#endif // POWER_MANAGER_H
