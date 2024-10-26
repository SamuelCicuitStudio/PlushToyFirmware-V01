#include "PowerManager.h"

// Constructor
PowerManager::PowerManager() : chargingCurrent(0), chargingVoltage(0) {}

/**
 * @brief Initialize power management settings.
 */
void PowerManager::begin() {
    initBatteryManagement();
}

/**
 * @brief Set the charging current.
 * 
 * @param current Charging current in mA.
 */
void PowerManager::setChargingCurrent(uint16_t current) {
    CHARGER.set_charge_current(current);
}

/**
 * @brief Set the charging voltage.
 * 
 * @param voltage Charging voltage in mV.
 */
void PowerManager::setChargingVoltage(uint16_t voltage) {
    CHARGER.set_charge_voltage(voltage);
}

/**
 * @brief Enable or disable ship mode.
 * 
 * @param enable True to enable ship mode, false to disable.
 */
void PowerManager::setShipMode(bool enable) {
    // Code to enable or disable ship mode on the battery management IC
}

/**
 * @brief Enter sleep mode.
 * 
 * @param duration Duration in seconds for the sleep mode.
 */
void PowerManager::enterSleepMode(uint32_t duration) {
    // Code to put the ESP32 into sleep mode for the specified duration
    esp_sleep_enable_timer_wakeup(duration * 1000); // Convert to microseconds
    esp_deep_sleep_start();
}

/**
 * @brief Wake up from sleep mode.
 */
void PowerManager::wakeUp() {
    // Code to handle waking up from sleep mode
}

/**
 * @brief Check battery level.
 * 
 * @return Battery level as a percentage (0-100).
 */
uint8_t PowerManager::getBatteryLevel() {
    uint16_t voltage = readBatteryVoltage();
    return calculateBatteryLevel(voltage);
}

/**
 * @brief Check if the battery level is below 20%.
 * 
 * @return True if battery level is below 20%, false otherwise.
 */
bool PowerManager::isBatteryLow() {
    uint8_t batteryLevel = getBatteryLevel();
    return batteryLevel < 20; // Return true if battery level is less than 20%
}

/**
 * @brief Read the thermistor value and calculate temperature.
 * 
 * @return Temperature in Celsius.
 */
float PowerManager::readThermistor() {
    uint32_t sum = 0;                     // Use uint32_t to avoid overflow
    int samples[NUMSAMPLES];              // Store NTC samples

    // Take NUMSAMPLES in a row, with a slight delay
    for (int i = 0; i < NUMSAMPLES; i++) {
        samples[i] = analogRead(PIN_THERMISTOR);
        sum += samples[i];                 // Accumulate sample values
        delay(10);
    }

    // Calculate the average value directly from the sum
    float average = static_cast<float>(sum) / NUMSAMPLES;

    // Convert the average value to resistance
    average = (pow(2, ADC_RESOLUTION) - 1) / average - 1;  // Max ADC value based on resolution
    average = SERIESRESISTOR / average;

    // Calculate temperature using the Steinhart-Hart equation
    float steinhart = average / THERMISTORNOMINAL;             // R/Ro
    steinhart = log(steinhart);                                 // ln(R/Ro)
    steinhart /= BCOEFFICIENT;                                  // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15);          // + (1/To)
    steinhart = 1.0 / steinhart;                                // Invert
    steinhart -= 273.15;                                        // Convert absolute temp to Celsius

    return steinhart;
}

/**
 * @brief Initialize the battery management IC.
 */
void PowerManager::initBatteryManagement() {
    // Configure pin modes for the battery management IC
    pinMode(PGOOD_PIN, INPUT);     // PGOOD pin: Open-drain active low indicator for power good status
    pinMode(STATUS_PIN, INPUT);     // STATUS pin: Open-drain active low indicator for charging status
    pinMode(PSEL_PIN, OUTPUT);       // PSEL pin: Power source selection
    // PSEL [LOW = Adapter, HIGH = USB]
    digitalWrite(PSEL_PIN, LOW);     // Set to LOW to select adapter power source

    pinMode(INT_PIN, INPUT);        // INT pin: Open-drain interrupt output for charger status and fault conditions
    pinMode(OTG_PIN, OUTPUT);        // OTG pin: Boost mode enable
    // OTG [LOW = Off, HIGH = Boost]
    digitalWrite(OTG_PIN, LOW);      // Set to LOW to disable boost mode

    pinMode(CE_PIN, OUTPUT);         // CE pin: Charge enable
    // CE [LOW = Charge, HIGH = Idle]
    digitalWrite(CE_PIN, LOW);       // Set to LOW to enable charging

    // Initialize I2C communication with the battery management IC
    Wire.begin(SDA_PIN, SCL_PIN);   // Start I2C with specified SDA and SCL pins
    CHARGER.begin(&Wire, BQ2589x_ADDR); // Initialize the charger with the I2C address
    CHARGER.disable_watchdog_timer(); // Disable the watchdog timer for stability
    CHARGER.adc_start(0);             // Start the ADC conversion
    CHARGER.disable_charger();        // Disable the charger initially
    CHARGER.set_charge_voltage(DEFAULT_CHARGE_VOLTAGE); // Set the desired charging voltage
    CHARGER.set_charge_current(DEFAULT_CHARGE_CURRENT);   // Set the desired charging current
    CHARGER.set_otg_voltage(DEFAULT_BOOST_VOLTAGE);      // Set the desired OTG voltage
    CHARGER.set_otg_current(DEFAULT_BOOST_CURRENT);       // Set the desired OTG current
}

/**
 * @brief Read the battery voltage.
 * 
 * @return Battery voltage in mV.
 */
uint16_t PowerManager::readBatteryVoltage() {
    // Code to read and return the battery voltage
    return CHARGER.adc_read_battery_volt();
}

/**
 * @brief Calculate the battery level percentage based on the voltage.
 * 
 * @param voltage Battery voltage in mV.
 * @return Battery level as a percentage (0-100).
 */
uint8_t PowerManager::calculateBatteryLevel(uint16_t voltage) {
    // Example logic to calculate battery level based on voltage
    if (voltage < 3200) return 0; // 0% for below 3.2V
    if (voltage > 4200) return 100; // 100% for above 4.2V
    return map(voltage, 3200, 4200, 0, 100); // Map between 3.2V and 4.2V
}
