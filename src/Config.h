#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <esp_task_wdt.h>

// ==================================================
// API Keys and Constants
// ==================================================
#define APKEY_DEEPGRAM "baba2fc8a8fec78e19ad2e8f0ae8e66e0ca2c029" ///< Deepgram API key
#define FRIENDLY_NAME "SebbAry"                               ///< Friendly name for the device
#define LANGUAGE_DEEPGRAM "fr"                                ///< Language for Deepgram API
#define LTIMEOUT_DEEPGRAM 5                                   ///< Timeout for Deepgram API in seconds

// ==================================================
// Configuration Constants
// ==================================================

// Default Wi-Fi Access Point (AP) Credentials
#define DEFAULT_AP_SSID "KidsToy"                             ///< Default SSID for AP
#define DEFAULT_AP_PASSWORD "12345678"                        ///< Default password for AP

// Debugging
#define DEBUGMODE 1                                           ///< Set to 1 to enable debug output, 0 to disable

// Serial Communication
#define SERIAL_BAUD_RATE 115200                              ///< Baud rate for serial communication

// Partition Configuration
#define CONFIG_PARTITION "config"                            ///< Partition for configuration storage

// ==================================================
// System Config Flages Name
// ==================================================
#define APWIFIMODE_FLAG "STRAP"                              ///< AP Wi-Fi mode flag
#define WIFISSID "WIFSSID"                                   ///< Wi-Fi SSID
#define WIFIPASS "WIFPASS"                                   ///< Wi-Fi password
#define RESET_FLAG "RST"                                     ///< Reset flag
#define FIRMWARE_VERSION "FIRVER"
// ==================================================
// File Paths and Naming
// ==================================================
#define EXTENSION ".wav"                                     ///< File extension for audio files

#define BASED_RECORDING_NAME "Recording"                      ///< Base name for recordings
#define TRANSCRIBE_FOLDER_PATH "/ForTranscription"           ///< Path for transcription files
#define BACKUP_FOLDER_PATH "/backupTranscription"            ///< Path for backup transcription files
#define RECORDING_FOLDER_PATH_ADD "WebRecording/"            ///< Path to web recording log file
#define RECORDING_FOLDER_PATH "/WebRecording"                ///< Path to web recordings
#define LOG_FILE_PATH "/log.txt"                             ///< Path for log file
#define BASE_TRANSCRIPTION_NAME "TranscriptionAudio"         ///< Base name for transcription audio files
#define STORY_LIST "/StoryList.txt"                          ///< Path for story list
#define RESPONSE_FOLDER_PATH "/Responses"                    ///< Path for response files

#define LANGUAGE_GTTS "fr"                                   ///< Language for Google Text-to-Speech
#define RESPONSE_MP3_FOLDER_PATH "/ResponsesMP3"            ///< Path for MP3 responses
#define BASED_RESPONSE_NAME "/AudioResponse.mp3"            ///< Base name for audio response files
#define BUFFER_TTS_SIZE 4096                                 ///< Buffer size for Text-to-Speech
#define STREAM_TIMEOUT 200                                    ///< Stream timeout in milliseconds

// ==================================================
// GPIO Pin Definitions
// ==================================================

// I2S Amplifier (MAX98357AEW) Pins
#define I2S_SD_MODE_PIN 48                                   ///< I2S SD mode pin
#define I2S_BCLK_PIN 16                                      ///< I2S BCLK pin
#define I2S_DIN_PIN 17                                       ///< I2S DIN pin
#define I2S_LRCLK_PIN 15                                     ///< I2S LRCLK pin

// ==================================================
// SPI SD Card Module Pins
// ==================================================
#define SPI_MISO_PIN 42                                      ///< SD card MISO pin (DATA0)
#define SPI_SCK_PIN 41                                       ///< SD card clock pin (CLK)
#define SPI_MOSI_PIN 39                                      ///< SD card MOSI pin (CMD)
#define SPI_CS_SD_PIN 2                                      ///< SD card chip select pin (DATA3)

// ==================================================
// Flash Memory Pins (e.g., Winbond W25Qxx Series)
// ==================================================
#define SPI_CS_FL_PIN 1                                      ///< Flash chip select pin
#define HOLD_PIN 35                                          ///< Hold pin for flash
#define WP_FL_PIN 40                                         ///< Write protect pin for flash
// ==================================================
// OTA SETTINGS
// ==================================================
#define OTA_UPDATE_URL "http/update.com"
#define DEFAULT_FIRMWARE_VERSION "1.0.0"
// ==================================================
// Microphone Module (MAX9814ETD) Pins
// ==================================================
#define MIC_OUT_PIN 38                                       ///< Microphone output pin
#define MIC_GAIN_PIN 47                                      ///< Microphone gain control pin
#define MIC_AR_PIN 21                                        ///< Microphone auto gain control pin
#define MIC_RESOLUTION 10                                    ///< Resolution of adc  microphone
#define MIC_RESOLUTION_MIN 0                                 ///< Minimum resolution for microphone
#define MIC_RESOLUTION_MAX 1024                              ///< Maximum resolution for microphone

#define WAV_RESOLUTION_MIN -10000                            ///< Minimum WAV resolution
#define WAV_RESOLUTION_MAX 10000                             ///< Maximum WAV resolution

// ==================================================
// BQ25896RTWR Battery Management System Pins
// ==================================================
#define PGOOD_PIN 7                                          ///< Power good pin: Open-drain active low indicator. LOW indicates a good input source if the voltage is within the specified range.
#define STATUS_PIN 6                                         ///< Status pin: Open-drain charge status output. LOW indicates charging in progress; HIGH indicates charging is complete or disabled. Blinks at 1 Hz on fault.
#define PSEL_PIN 8                                           ///< BQ25896 PSEL Digital Input: Selects power source. HIGH indicates a USB host source, LOW indicates an adapter source.
#define INT_PIN 0                                            ///< BQ25896 INT Digital Input: Open-drain interrupt output to report charger device status and fault conditions.
#define OTG_PIN 0                                            ///< BQ25896 OTG Digital Input: Boost mode enable pin, activated when OTG_CONFIG=1, HIGH with no input source at VBUS.
#define CE_PIN 18                                            ///< BQ25896 CE Digital Input: Active low charge enable pin. Battery charging is enabled when configured properly and CE is LOW.

#define SDA_PIN 4                                            ///< I2C SDA pin: Serial Data Line for I2C communication.
#define SCL_PIN 5                                            ///< I2C SCL pin: Serial Clock Line for I2C communication.

#define BQ2589x_ADDR 0x6B                                    ///< BQ25896 I2C Address: The address used for I2C communication with the BQ25896 chip.
#define STOREVOLTAGE 3750                                    ///< VCHG Voltage for storage: The voltage at which the battery is charged for storage.

// ==================================================
// BQ25896RTWR Charge Voltage Settings
// ==================================================
#define NUM_CHARGE_VOLTAGE_VALUES 4                         ///< Number of charge voltage values
#define CHARGE_VOLTAGE_0 3840                                ///< Charge voltage value 0 in millivolts
#define CHARGE_VOLTAGE_1 4096                                ///< Charge voltage value 1 in millivolts
#define CHARGE_VOLTAGE_2 4192                                ///< Charge voltage value 2 in millivolts
#define CHARGE_VOLTAGE_3 4208                                ///< Charge voltage value 3 in millivolts
#define DEFAULT_CHARGE_VOLTAGE CHARGE_VOLTAGE_2            ///< Default charge voltage index, corresponds to 4.192V

// ==================================================
// BQ25896RTWR Charge Current Settings
// ==================================================
#define NUM_CHARGE_CURRENT_VALUES 6                          ///< Number of charge current values
#define CHARGE_CURRENT_0 512                                 ///< Charge current value 0 in milliamps
#define CHARGE_CURRENT_1 1024                                ///< Charge current value 1 in milliamps
#define CHARGE_CURRENT_2 1536                                ///< Charge current value 2 in milliamps
#define CHARGE_CURRENT_3 2048                                ///< Charge current value 3 in milliamps
#define CHARGE_CURRENT_4 2560                                ///< Charge current value 4 in milliamps
#define CHARGE_CURRENT_5 3072                                ///< Charge current value 5 in milliamps
#define DEFAULT_CHARGE_CURRENT CHARGE_CURRENT_5             ///< Default charge current index, corresponds to 3A

// ==================================================
// BQ25896RTWR Boost Voltage Settings
// ==================================================
#define NUM_BOOST_VOLTAGE_VALUES 3                           ///< Number of boost voltage values
#define BOOST_VOLTAGE_0 4998                                 ///< Boost voltage value 0 in millivolts
#define BOOST_VOLTAGE_1 5062                                 ///< Boost voltage value 1 in millivolts
#define BOOST_VOLTAGE_2 5126                                 ///< Boost voltage value 2 in millivolts
#define DEFAULT_BOOST_VOLTAGE BOOST_VOLTAGE_1              ///< Default boost voltage index, corresponds to 5.062V

// ==================================================
// BQ25896RTWR Boost Current Settings
// ==================================================
#define NUM_BOOST_CURRENT_VALUES 5                           ///< Number of boost current values
#define BOOST_CURRENT_0 500                                  ///< Boost current value 0 in milliamps
#define BOOST_CURRENT_1 750                                  ///< Boost current value 1 in milliamps
#define BOOST_CURRENT_2 1200                                 ///< Boost current value 2 in milliamps
#define BOOST_CURRENT_3 1650                                 ///< Boost current value 3 in milliamps
#define BOOST_CURRENT_4 2150                                 ///< Boost current value 4 in milliamps
#define DEFAULT_BOOST_CURRENT BOOST_CURRENT_4               ///< Default boost current index, corresponds to 2.15A

// ==================================================
// Sleep Mode Settings
// ==================================================
#define NUM_SLEEP_TIMEOUT_VALUES 3                           ///< Number of sleep timeout values
#define SLEEP_TIMEOUT_0 60                                   ///< Sleep/Ship Mode timeout value 0 in seconds
#define SLEEP_TIMEOUT_1 120                                  ///< Sleep/Ship Mode timeout value 1 in seconds
#define SLEEP_TIMEOUT_2 300                                  ///< Sleep/Ship Mode timeout value 2 in seconds
#define DEFAULT_SLEEP_TIMEOUT_INDEX 0                        ///< Default Sleep/Ship Mode index

// ==================================================
// Power Down Mode Settings
// ==================================================
#define ENABLE_SHIP_MODE 0                                   ///< Enable/Disable Ship Mode (0 = disable, 1 = enable)

// ==================================================
// IC NTC Thermistor Pins
// ==================================================
#define PIN_THERMISTOR 37                                    ///< NTC Thermistor PIN
#define THERMISTORNOMINAL 10000                              ///< NTC Thermistor nominal resistance
#define TEMPERATURENOMINAL 25                                ///< NTC Thermistor nominal temperature
#define NUMSAMPLES 5                                         ///< Number of temperature samples to take
#define BCOEFFICIENT 3425                                    ///< NTC Thermistor B Coefficient
#define SERIESRESISTOR 10000                                 ///< NTC Thermistor series resistor
#define ADC_RESOLUTION 10
// ==================================================
// Recording Configuration
// ==================================================
#define RECORDING_LENGTH 2100                                ///< Recording length in milliseconds
#define SAMPLE_RATE 8000                                     ///< Sample rate in Hz
#define CHANNEL 1                                            ///< Mono channel

// ==================================================
// LED and Button Pin Definitions
// ==================================================
#define LED_PIN 3                                           ///< LED pin for status indication
#define BUTTON_01_PIN 9                                        ///< Button pin for input
#define BUTTON_02_PIN 10                                        ///< Button pin for input
#define LED_ON HIGH                                         ///< Define LED ON state
#define LED_OFF LOW                                         ///< Define LED OFF state

// ==================================================
// Optional Configurations
// ==================================================
#define OPTIONAL_FEATURE 0                                   ///< Optional features, 0 = disable, 1 = enable

// ==================================================
// JSON Configuration Defaults
// ==================================================
#define JSON_PARSER_BUFFER_SIZE 256                         ///< Buffer size for JSON parsing
#define JSON_PRINT_BUFFER_SIZE 512                           ///< Buffer size for JSON printing

// ==================================================
// UART Communication Settings
// ==================================================
#define UART_BAUD_RATE 9600                                 ///< UART baud rate
#define UART_TX_PIN 22                                      ///< UART transmit pin
#define UART_RX_PIN 23                                      ///< UART receive pin

// ==================================================
// End of Configuration
// ==================================================
#endif // CONFIG_H
