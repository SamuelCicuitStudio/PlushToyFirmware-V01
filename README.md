---

# SmartPlushToy Firmware Version:1.0.0

Welcome to the **SmartPlushToy** firmware repository! This project provides firmware for an ESP32-based plush toy designed for interaction with children through audio feedback, LED displays, games, and more. This document describes the overall architecture, class structure, and basic usage information for developers working on or contributing to this project.

## Project Structure

The firmware for SmartPlushToy is modular, with each key system component encapsulated in separate classes for better code organization, modularity, and maintainability. Below is an overview of each primary class, its purpose, and interconnections within the system.
### 0. Quick navigation
Here's a quick navigation guide to access the different class descriptions:

- [**Dependency Flow Chart**](#dependency-flow-chart)
- [**PowerManager Class**](#powermanager-class)
- [**ConfigManager Class**](#configmanager-class)
- [**WiFiManager Class**](#wifimanager-class)
- [**OtaManager Class**](#otamanager-class)
- [**I2SManager Class**](#i2smanager-class)
- [**SDCardManager Class**](#sdcardmanager-class)
- [**MicManager Class**](#micmanager-class)
- [**WAVFileReader Class**](#wavfilereader-class)
- [**WAVFileWriter Class**](#wavfilewriter-class)
- [**SpeakerManager Class**](#speaker-manager-class)
- [**SPIFlashManager Class**](#spiflashmanager-class)

### 1. Configuration Files
- **`Config.h`**: Contains global constants and system-wide `#define` directives. Includes default values for GPIO pins, partition configurations, security credentials (passwords), etc. This file acts as a central configuration point for all other classes.
- **`Configuration.h`**: Manages the setup for the Preferences Partition, enabling the system to store and retrieve non-volatile data. Responsible for initializing configurations and restoring default values.

### 2. Core Managers

Each manager encapsulates functionality for different components of the toy, making it easier to manage individual parts of the system.

- **GPIOManager**: Configures and controls GPIOs across the system. This manager abstracts direct GPIO manipulation, providing simple interfaces for other classes to use.

- **MicManager**: Handles microphone input data collection and filtering. Manages data retrieval from the analog microphone for use by the **SpeechToTextManager**.

- **SpeakerManager**: Manages the toy’s audio output. Handles I2S communication with the speaker driver and integrates with the **TextToSpeechManager** and **WavFileReader** classes for audio playback.

- **I2SManager**: Manages I2S communication specifically for audio output. Responsible for configuring I2S parameters, transmitting audio data to the speaker, and integrating with **SpeakerManager**.

- **WiFiManager**: Handles all Wi-Fi connectivity, including connecting to networks, managing credentials, and handling disconnections. Works with **OtaManager** for firmware updates over Wi-Fi.

- **SDCardManager**: Manages interactions with the SD card, including mounting, reading, and writing files, with support for loading and saving audio files.

- **SPIFlashManager**: Manages interactions with the onboard flash memory for storage of non-volatile data, including firmware updates and cached configurations.

### 3. Audio and Speech Processing

- **TextToSpeechManager**: Converts text input into audio output data, sending it to **SpeakerManager** for playback. Utilizes pre-stored or generated audio files.

- **SpeechToTextManager**: Processes audio data from **MicManager** and converts spoken words into text data for command processing by the **VocalCommandManager**.

- **WavFileReader/WavFileWriter**: Reads and writes `.wav` audio files from and to the SD card, facilitating the recording and playback of audio. Provides audio data in a format compatible with the **SpeakerManager**.

### 4. System and Power Management

- **PowerManager**: Monitors battery levels and manages charge control. Tracks battery status and provides feedback to the **RandomBehaviorManager** to adjust activity levels based on power constraints.

- **OtaManager**: Manages Over-the-Air (OTA) updates, allowing the firmware to be updated remotely. Coordinates with **SPIFlashManager** to store and load new firmware data safely.

### 5. Interaction and Feedback Managers

- **RandomBehaviorManager**: Controls non-deterministic behavior patterns like LED animations and vibration feedback. This manager is triggered by various system events and operates alongside **GameManager** for interactive responses.

- **VocalCommandManager**: Receives text commands from **SpeechToTextManager** and initiates appropriate actions across the toy, such as playing a sound, triggering a game, or adjusting settings.

- **GameManager**: Manages interactive games available on the plush toy, utilizing **SpeakerManager** for game audio, **RandomBehaviorManager** for visual feedback, and **VocalCommandManager** for voice-command-based games.

## Suggested Architecture Overview

Below is an overview of how the classes interact in the **SmartPlushToy** system:

1. **Startup & Initialization**: The system starts by initializing **Config.h** and **Configuration.h**, setting up default parameters.
2. **Power Management**: **PowerManager** checks battery status and sets constraints for system operation (e.g., disables games when the battery is low).
3. **Wi-Fi Management**: **WiFiManager** establishes connections, handles credentials, and communicates with **OtaManager** for firmware updates.
4. **I/O Management**:
   - **MicManager** collects audio input data.
   - **SpeakerManager** provides audio output for responses and game sounds, utilizing **I2SManager** to transmit audio data.
5. **Data Handling**:
   - **SDCardManager** and **SPIFlashManager** manage audio and configuration data.
   - **WavFileReader** and **WavFileWriter** provide data interfaces for reading and saving `.wav` files.
6. **Interactive Features**:
   - **VocalCommandManager** processes user commands and initiates interactions.
   - **RandomBehaviorManager** and **GameManager** provide feedback and engage users through LED animations, audio, and vibrations.
7. **Text and Audio Processing**:
   - **TextToSpeechManager** generates responses, while **SpeechToTextManager** converts audio to text for command recognition.

### Example Flow: "Play Sound" Command

1. **MicManager** captures audio, and **SpeechToTextManager** identifies the "play sound" command.
2. **VocalCommandManager** triggers **SpeakerManager**, which uses **I2SManager** to play an audio file, loading data via **WavFileReader** from the SD card.
3. **RandomBehaviorManager** may trigger LED animations or vibration patterns to accompany the sound.

## Class Interaction Pointers

Here's a summary of which classes will call or depend on each other:

- **Main Program**: Initializes **Configuration** and sets up the main loop.
- **Configuration**: Interacts with **PowerManager**, **WiFiManager**, **GPIOManager**, **SDCardManager**, **SPIFlashManager** for initialization.
- **PowerManager**: Monitors battery state, interacts with **RandomBehaviorManager** for power-based constraints.
- **WiFiManager**: Interacts with **OtaManager** for updates and may call methods in **Configuration** to save Wi-Fi credentials.
- **MicManager**: Supplies audio data to **SpeechToTextManager**.
- **SpeechToTextManager**: Sends recognized text commands to **VocalCommandManager**.
- **VocalCommandManager**: Initiates actions in **SpeakerManager**, **RandomBehaviorManager**, and **GameManager** based on voice commands.
- **SpeakerManager**: Utilizes **I2SManager** for audio output, gets audio files from **WavFileReader** and passes audio to the speaker.
- **I2SManager**: Interacts with **SpeakerManager** for configuring and sending audio data.
- **SDCardManager**: Provides access to audio files for **WavFileReader** and **WavFileWriter**.
- **WavFileReader/WavFileWriter**: Read/write audio data to and from the SD card, providing data to **SpeakerManager**.
## Dependency Flow Chart

The following diagram illustrates the dependency flow among the various classes used in the project. Each class is structured to show its relationships and the direction of calls, making it easier to understand the architecture of the application.

```
                                 ┌───────────────────────┐
                                 │      Config.h         │
                                 └───────────────────────┘
                                           │
                                           ▼
                                 ┌───────────────────────┐
                                 │   Configuration.h     │
                                 └───────────────────────┘
                                           │
                                           ▼
                               ┌───────────┼──────────────────┐
                               │           │                  │
                               ▼           ▼                  ▼
                     ┌─────────────┐   ┌─────────────┐   ┌─────────────┐
                     │ GPIOManager │   │ PowerManager│   │ WiFiManager │
                     └─────────────┘   └─────────────┘   └─────────────┘
                                           │
                                           ▼
                                   ┌──────────────────┐
                                   │   OtaManager     │
                                   └──────────────────┘
                                           │
                                           ▼
                                   ┌──────────────────┐
                                   │   I2SManager     │
                                   └──────────────────┘
                                           │
                               ┌───────────┼──────────────────┐
                               │           │                  │
                               ▼           ▼                  ▼
                     ┌────────────┐   ┌──────────────┐   ┌──────────────┐
                     │ MicManager │   │WavFileReader │   │ WavFileWriter│
                     └────────────┘   └──────────────┘   └──────────────┘
                                           │
                                           ▼
                                   ┌──────────────────┐
                                   │  SpeakerManager   │
                                   └──────────────────┘
                                           │
                                           ▼
                                   ┌──────────────────┐
                                   │ SPIFlashManager  │
                                   └──────────────────┘
                                           │
                                           ▼
                                   ┌──────────────────┐
                                   │  DownloadStory   │
                                   └──────────────────┘
                                           │
                                           ▼
                                   ┌──────────────────┐
                                   │   StoryTelling   │
                                   └──────────────────┘
                                           │
                                           ▼
                               ┌───────────┼───────────┐
                               │           │           │
                               ▼           ▼           ▼
                     ┌─────────────────┐   ┌─────────────┐
                     │SpeechToText     │   │TextToSpeech │
                     │ Manager         │   │ Manager     │
                     └─────────────────┘   └─────────────┘
                               │                   │
                               │                   │
                               │                   │
                               ▼                   ▼
                      ┌──────────────────┐   ┌─────────────────┐
                      │ RandomBehavior   │   │   GameManager   │
                      │   Manager        │   └─────────────────┘
                      └──────────────────┘           │
                                                     ▼
                                          ┌───────────────────────┐
                                          │   VocalCommandManager │
                                          └───────────────────────┘

```

### Explanation
- **Classes Overview**: This flowchart displays the architecture of the project, illustrating how different components are interconnected.
- **Dependencies**: The arrows represent calls between classes, indicating which classes invoke methods of other classes. Understanding these relationships is crucial for navigating and maintaining the codebase.

# PowerManager Class

The `PowerManager` class is designed to manage battery operations for devices using the BQ2589x battery management IC. It provides functions to initialize the battery management system, read battery voltage, set charging parameters, and manage power modes.

## Features

- **Battery Management Initialization**: Configures the necessary pins and initializes the BQ2589x IC for operation.
- **Battery Voltage Reading**: Reads the current battery voltage and returns it in millivolts (mV).
- **Charging Parameters**: Set and adjust charging voltage and current to optimize battery charging.
- **Power Mode Control**: Enables features like ship mode and deep sleep for power savings.

## Functionality

### Initialization

- `void begin()`: Initializes power management settings by calling the battery management initialization function.

### Battery Management

- `void initBatteryManagement()`: Configures the pin modes for the BQ2589x and initializes I2C communication.

### Charging Control

- `void setChargingCurrent(uint16_t current)`: Sets the charging current for the battery.
- `void setChargingVoltage(uint16_t voltage)`: Sets the charging voltage for the battery.

### Power Management

- `void enterSleepMode(uint32_t duration)`: Puts the ESP32 into sleep mode for a specified duration to save power.
- `void wakeUp()`: Handles the logic for waking up the device from sleep mode.

### Battery Monitoring

- `uint8_t getBatteryLevel()`: Retrieves the battery level as a percentage based on the current battery voltage.
- `uint16_t readBatteryVoltage()`: Reads the battery voltage using the BQ2589x ADC and returns the value in millivolts (mV).

### Temperature Monitoring

- `float readThermistor()`: Reads the temperature from a thermistor to monitor battery temperature.

## Example Usage

```cpp
#include "PowerManager.h"

PowerManager powerManager;

void setup() {
    powerManager.begin(); // Initialize power management
    uint8_t batteryLevel = powerManager.getBatteryLevel(); // Get battery level
    Serial.print("Battery Level: ");
    Serial.println(batteryLevel);
}

void loop() {
    // Main loop logic
}
```

# ConfigManager Class

The `ConfigManager` class provides an efficient way to manage configuration settings for ESP32 applications using the Preferences library. This class allows for easy storage, retrieval, and management of various data types, including boolean, integer, float, and string values. It also includes functionality to reset the system and simulate power-down scenarios.

## Features

- **Initialization**: Automatically initializes configuration settings and checks if the device needs to be reset to factory defaults.
- **Read/Write Preferences**: Provides methods to open preferences in both read-write and read-only modes, allowing for flexible data management.
- **Data Management**: Supports storing and retrieving various data types:
  - Booleans
  - Unsigned integers
  - Integers
  - Floats
  - Strings
- **Reset Functionality**: Includes methods to reset the device with a countdown and simulate a power-down state through deep sleep.
- **Debugging Support**: Prints debug messages when enabled, allowing developers to trace execution and monitor configuration changes.

## Usage

To use the `ConfigManager` class, instantiate it with a reference to a `Preferences` object and call its methods to manage your configuration data. Ensure to check the `DEBUGMODE` macro for debugging outputs.

### Example

```cpp
#include <Preferences.h>
#include "ConfigManager.h"

Preferences preferences;
ConfigManager configManager(preferences);

void setup() {
    configManager.begin(); // Initialize the ConfigManager
    // Access preferences as needed
}

void loop() {
    // Main program logic
}
```

## Methods

- **Constructor**: Initializes the `ConfigManager` with a `Preferences` object.
- **Destructor**: Ensures proper cleanup of the preferences on destruction.
- **begin()**: Initializes configuration settings and checks for reset flags.
- **GetBool(), GetInt(), GetFloat(), GetString()**: Retrieve values from preferences.
- **PutBool(), PutInt(), PutFloat(), PutString()**: Store values in preferences.
- **RemoveKey()**: Remove a specific key from preferences.
- **ClearKey()**: Clear all stored preferences.
- **RestartSysDelay()**: Restarts the system after a specified delay.
- **simulatePowerDown()**: Simulates a power-down state by putting the ESP32 into deep sleep.

Here's an expanded GitHub description for the `WiFiManager` class, including detailed comments and a usage example:

# WiFiManager Class

The `WiFiManager` class provides an efficient and user-friendly way to manage Wi-Fi connectivity for ESP32 devices. It simplifies the process of connecting to a Wi-Fi network or initiating an access point (AP) mode for user configuration. Leveraging the ESPAsyncWebServer library, this class serves web pages for Wi-Fi setup and GPIO control, enabling seamless user interaction.

#### Key Features:
- **Automatic Mode Selection**: Connects to a specified Wi-Fi network or starts an access point based on stored settings.
- **Web Interface**: Serves HTML pages for configuring Wi-Fi credentials and controlling GPIO pins.
- **Asynchronous Operations**: Utilizes the ESPAsyncWebServer for non-blocking web requests.

#### Public Methods:
- **`WiFiManager()`**: Constructor that initializes the `WiFiManager` instance.
- **`void begin()`**: Starts the Wi-Fi manager and determines the operating mode (Wi-Fi or AP).
- **`void setAPCredentials(const char* ssid, const char* password)`**: Sets the SSID and password for the access point mode.
- **`void setServerCallback()`**: Configures server routes for handling web requests related to Wi-Fi and GPIO operations.

#### Private Methods:
- **`void connectToWiFi()`**: Connects to the configured Wi-Fi network.
- **`void startAccessPoint()`**: Initiates access point mode for user configuration.
- **`void handleRoot(AsyncWebServerRequest* request)`**: Handles the root request and serves the welcome page.
- **`void handleSetWiFi(AsyncWebServerRequest* request)`**: Serves the Wi-Fi credentials page.
- **`void handleSaveWiFi(AsyncWebServerRequest* request)`**: Saves the Wi-Fi credentials provided by the user.
- **`void handleGPIO(AsyncWebServerRequest* request)`**: Handles GPIO control requests.

#### Usage Example:
To integrate the `WiFiManager` class into your ESP32 project, follow these steps:

1. **Include the Necessary Libraries**:
   Make sure to include the necessary libraries in your main file:
   ```cpp
   #include <WiFi.h>
   #include <FS.h>
   #include <SPIFFS.h>
   #include <ESPAsyncWebServer.h>
   #include "WiFiManager.h"
   ```

2. **Create an Instance of `WiFiManager`**:
   In your setup function, create an instance of `WiFiManager` and call `begin()` to start the manager:
   ```cpp
   WiFiManager wifiManager;

   void setup() {
       Serial.begin(115200);
       wifiManager.begin();
   }
   ```

3. **Set Access Point Credentials (Optional)**:
   If you want to set custom credentials for the access point, use the `setAPCredentials` method:
   ```cpp
   wifiManager.setAPCredentials("MyAPSSID", "MyAPPassword");
   ```

4. **Access the Web Interface**:
   Once initialized, the device will either connect to the specified Wi-Fi network or start an access point. Users can connect to the access point and access the web interface through a browser to configure Wi-Fi settings or control GPIO pins.

#### Example HTML Structure:
To serve the HTML pages, ensure that you have the corresponding HTML files in the SPIFFS file system, such as `/welcome.html`, `/wifiCredentialsPage.html`, and `/gpiomanager.html`.

# OtaManager Class

The `OtaManager` class enables easy Over-the-Air (OTA) firmware updates for ESP32 devices by checking for new firmware versions from a remote server, downloading the latest firmware, and automatically applying the update if available. This functionality is particularly useful for remotely managing device updates without requiring physical access, making it an ideal solution for IoT applications.

### Features

- **Automatic Version Check**: Queries a remote server for the latest firmware version.
- **Secure Firmware Download**: Downloads firmware binaries from a specified server URL.
- **Firmware Update Process**: Validates the update process to ensure full downloads and complete update integrity.
- **ConfigManager Integration**: Utilizes a configuration manager for retrieving and storing the current firmware version and settings.
- **Debugging Output**: Includes Serial output for debugging to confirm status messages during the update process.

### Class Reference

#### Constructor

- **`OtaManager(ConfigManager* configManager,PowerManager* powerManager)`**
  - Constructs an `OtaManager` instance with an associated `ConfigManager`, which provides access to configuration data such as the current firmware version and update URL.

#### Public Methods

- **`void begin()`**
  - Initializes the OTA update manager, setting up the current firmware version and update URL. Prints debugging messages if `DEBUGMODE` is enabled.
  
- **`void checkForUpdate()`**
  - Checks if a new firmware version is available on the server. If a new version is detected, it triggers the download and update process.
  
- **`void downloadAndUpdateFirmware(const String& firmwareURL)`**
  - Downloads the firmware binary from the provided `firmwareURL`, writes it to flash memory, and completes the update. If successful, initiates a system reboot to apply the new firmware.

#### Private Methods

- **`bool isNewVersionAvailable()`**
  - Checks if the latest firmware version on the server differs from the current version stored in the device. Returns `true` if an update is available.

### Example Usage

Here's how you can integrate `OtaManager` into your ESP32 project:

```cpp
#include "OtaManager.h"
#include "PowerManager.h"

ConfigManager configManager;
PowerManager* powerManager;
OtaManager otaManager(&configManager,powerManager);

void setup() {
    otaManager.begin();        // Initialize OTA with current version
    otaManager.checkForUpdate(); // Check for available firmware updates
}

void loop() {
    // Application logic
}
```

### Requirements

- **Libraries**:
  - **Arduino.h**: Provides basic functions for ESP32.
  - **HTTPClient**: Enables HTTP communication for fetching firmware versions and binary files.
  - **Update**: Manages the OTA update process for ESP32.

### Notes

- The `ConfigManager` class must be configured to store and retrieve the firmware version.
- Debugging output is available if `DEBUGMODE` is enabled, allowing step-by-step tracing of the update process.

# I2SManager Class

The `I2SManager` class provides a streamlined interface for controlling I2S (Inter-IC Sound) audio playback on the ESP32, allowing developers to handle real-time audio applications. With easy-to-use methods for playback control and sample handling, this class simplifies the setup and management of I2S for audio streaming, such as music playback, sound effects, or other audio projects.

## Key Features
- **Simple Initialization:** Configure pins and sample rate, then start streaming audio effortlessly.
- **Playback Control:** Start, pause, resume, and stop audio playback with straightforward methods.
- **Real-time Audio Support:** Designed to handle and output audio samples in real-time.

## Prerequisites
This class is designed for use with the ESP32 microcontroller and requires the **ESP-IDF I2S driver**, available in the ESP32 Arduino core.

## Usage

### Quick Start Example

```cpp
#include "I2SManager.h"

// Define I2S pins
i2s_pin_config_t pins = {
    .bck_io_num = 26,          // Bit clock line
    .ws_io_num = 25,           // Word select (LRCK) line
    .data_out_num = 22,        // Data output line
    .data_in_num = I2S_PIN_NO_CHANGE  // No data input
};

int sampleRate = 44100;        // Set sample rate to 44.1 kHz (CD quality)
I2SManager i2sManager(pins, sampleRate);

void setup() {
    Serial.begin(115200);
    i2sManager.begin();        // Start the I2S audio stream
}

void loop() {
    int16_t sample = generateAudioSample();  // Replace with your audio sample source
    i2sManager.writeSample(sample);          // Send sample to I2S

    // Example playback control
    delay(1000);
    i2sManager.pause();         // Pause playback
    delay(1000);
    i2sManager.resume();        // Resume playback
    delay(1000);
    i2sManager.stop();          // Stop playback
}
```

### Class Methods

The `I2SManager` class includes several methods to control I2S playback and handle audio samples:

1. **Constructor**
   - `I2SManager(i2s_pin_config_t pins, int sample_rate);`
   - Configures the I2S driver with specified pin assignments and sample rate.

2. **begin()**
   - Initializes and begins I2S audio streaming.
   - Should be called in the setup phase after creating an `I2SManager` instance.

3. **writeSample(int16_t sample)**
   - Sends a 16-bit audio sample to the I2S peripheral for output.
   - Only writes if the playback state is active (`isPlaying()` returns `true`).

4. **pause()**
   - Pauses the audio stream without stopping or resetting the driver.
   - Call `resume()` to continue playback.

5. **resume()**
   - Resumes audio playback if it was paused.
   
6. **stop()**
   - Completely halts audio playback and stops the I2S driver.
   
7. **isPlaying()**
   - Returns `true` if the I2S stream is actively playing; otherwise, returns `false`.

## Dependencies
The following libraries are required:
- `Arduino.h` for basic ESP32 compatibility with the Arduino IDE.
- `driver/i2s.h` for ESP32-specific I2S driver functions.

## Installation

To use `I2SManager`, include the file in your ESP32 project and ensure you have the necessary libraries in your Arduino IDE or ESP-IDF setup.

1. Clone the repository to your project’s library folder or include the `.cpp` and `.h` files directly.
2. Ensure your ESP32 board library is installed and up-to-date.
3. Include `I2SManager.h` in your code and initialize the class with appropriate settings.

## Configuration

### Pin Configuration
Assign I2S-specific pins (such as bit clock, word select, and data out) when creating the `i2s_pin_config_t` structure. Ensure these pins are compatible with your ESP32 board’s configuration.

### Sample Rate
Set a sample rate that matches your audio requirements, such as `44100` for CD quality or `16000` for voice applications.

# SDCardManager Class

## Overview
The `SDCardManager` class provides a structured way to manage SD card initialization and recording file handling on embedded devices. Designed for applications requiring audio recording, it enables reliable storage and retrieval of recorded audio data on an SD card, ensuring efficient filename management and retrieval of the latest recorded files.

## Features
- **SD Card Initialization**: The `begin()` method initializes the SD card, setting up the SPI bus for communication and creating the required directory for recordings.
- **Sequential File Naming**: The `getNextRecordingFilename()` function generates unique filenames in sequential order, avoiding file overwrites.
- **Latest Recording Retrieval**: The `getLastRecordedFilename()` function retrieves the most recently modified file, making it easy to access the latest recording.

## Usage
### Setup
To use `SDCardManager`, call the `begin()` method to initialize the SD card and ensure the recording directory is ready.
```cpp
SDCardManager sdManager;
sdManager.begin();
```

### Creating a New Recording
To get the next available filename for a new recording, use `getNextRecordingFilename()`. This function returns a unique filename in sequential order.
```cpp
String nextFile = sdManager.getNextRecordingFilename();
```

### Retrieving the Latest Recording
To access the filename of the last recorded file, use `getLastRecordedFilename()`. This scans the recording directory for the latest file by modification time.
```cpp
String lastFile = sdManager.getLastRecordedFilename();
```

## Class Reference

### `SDCardManager` Constructor
```cpp
SDCardManager();
```
- Initializes the SD card manager instance.

### `void begin()`
- Initializes the SD card and sets up the SPI bus, creating the recording folder if it doesn’t exist. Logs SD card initialization status.

### `String getNextRecordingFilename()`
- Generates a unique, sequential filename for a new recording by checking for existing files.
- **Returns**: `String` - The next filename for recording, without path or extension.

### `String getLastRecordedFilename()`
- Retrieves the filename of the most recent recording based on file modification times.
- **Returns**: `String` - The filename of the latest recorded file.

## Dependencies
- **SD Library** (`SD.h`): Manages filesystem operations on the SD card.
- **SPI Library** (`SPI.h`): Enables SPI communication with the SD card.
- **I2SManager Integration**: Supports potential use with audio recording/playback through I2S.

## Example
```cpp
#include "SDCardManager.h"

SDCardManager sdManager;

void setup() {
    Serial.begin(115200);
    sdManager.begin();

    String nextFile = sdManager.getNextRecordingFilename();
    Serial.print("Next filename for recording: ");
    Serial.println(nextFile);

    String lastFile = sdManager.getLastRecordedFilename();
    Serial.print("Last recorded file: ");
    Serial.println(lastFile);
}

void loop() {
    // Record, playback, or perform other operations
}
```

## Summary
The `SDCardManager` class provides an easy-to-use interface for managing file operations on an SD card in embedded applications, particularly for audio recording. It ensures reliable recording management with minimal code overhead, supporting efficient file handling for continuous or sequential recordings.

# MicManager Class

The `MicManager` class is a comprehensive solution for managing I2S-based microphones in Arduino projects. It offers functionalities that facilitate microphone initialization, gain adjustment, and audio data processing, ensuring high-quality audio input and effective noise reduction.

## Features

### 1. Microphone Initialization
- **`begin()` Method**: This method initializes the microphone by configuring necessary parameters and preparing the I2S interface. It ensures that the microphone is ready to capture audio input immediately after instantiation.

### 2. Gain Control
- **`setGain(int gain)` Method**: Users can adjust the microphone's sensitivity by setting the gain level. This feature is critical for applications requiring different audio input levels, such as voice recording in noisy environments or capturing soft sounds. 
    - **Parameters**: 
      - `gain`: An integer value representing the desired gain level. Higher values increase sensitivity, while lower values decrease it.

### 3. Audio Data Processing
- **`readOutput()` Method**: This method retrieves the current output value from the microphone after applying a high-pass filter. The high-pass filter removes DC offset and low-frequency noise, resulting in cleaner audio signals, making this class suitable for applications like speech recognition or audio analysis.
    - **Returns**: An integer representing the processed microphone output value.

## Internal Implementation

### Private Member Variables
- **`float alpha`**: A filter coefficient (set to `0.98`) used in the high-pass filter algorithm to control the amount of filtering applied. This value can be adjusted to tune the filter's responsiveness.
- **`float lastMicValue`**: Stores the last microphone output value to facilitate filtering.
- **`float filteredMicValue`**: Holds the most recent filtered output from the microphone, ensuring that only clean audio data is processed.

## Dependencies

- **I2SManager**: The `MicManager` class relies on the `I2SManager` class to configure and manage I2S audio data streams. Ensure that you have an appropriate `I2SManager` instance created and initialized before using this class.
- **Arduino**: The standard Arduino library is utilized for basic functionalities and data types.

## Usage Example

```cpp
#include "I2SManager.h"
#include "MicManager.h"

// Create instances of I2SManager and MicManager
i2s_pin_config_t pinConfig = {/* configure your pins here */};
I2SManager i2sManager(pinConfig, 44100);  // Example sample rate
MicManager mic;

void setup() {
    i2sManager.begin();  // Initialize I2S manager
    mic.begin();         // Initialize microphone
    mic.setGain(10);     // Set desired microphone gain
}

void loop() {
    int micOutput = mic.readOutput();  // Read microphone output
    // Process micOutput as needed
}
```

## Notes

- Ensure that the `I2SManager` instance is properly configured before initializing the `MicManager` class to avoid runtime issues.
- The high-pass filter implemented in the `readOutput()` method helps mitigate low-frequency noise and DC offsets, making it suitable for audio applications where clarity and quality are paramount.
- This class is ideal for various applications, including voice-controlled systems, audio recording, and environmental sound analysis, where high-quality microphone input is essential.

## Conclusion

The `MicManager` class simplifies the process of working with I2S microphones in Arduino projects, providing a user-friendly interface to manage microphone settings and ensure high-quality audio input. Whether you're building a voice assistant, a sound recording device, or an interactive audio system, `MicManager` is a valuable tool in your development toolkit.

## WAVFileReader Class

The `WAVFileReader` class is designed to facilitate the reading and playback of WAV audio files from an SD card on the ESP32 platform. This class handles the complexities of parsing WAV file headers and efficiently streaming audio samples to an I2S output interface, enabling high-quality audio playback in various applications, such as music players or interactive audio experiences.

#### Key Features:
- **WAV File Parsing**: 
  - Automatically reads and parses the WAV file header to extract essential audio properties like sample rate, number of channels, and data length.
  
- **Playback Control**:
  - Provides methods to start, stop, pause, and resume playback, allowing for flexible audio management during runtime.
  
- **Playback State Management**:
  - Supports different playback states (STOPPED, PLAYING, PAUSED) to manage audio control flow effectively.
  
- **FreeRTOS Integration**:
  - Operates as a separate FreeRTOS task, enabling non-blocking audio playback while allowing other tasks to run concurrently.
  
- **I2S Output Support**:
  - Utilizes the I2S interface for high-fidelity audio output, making it compatible with various audio DACs and amplifiers.
  
- **End-of-File Detection**:
  - Provides functionality to check if the end of the audio data has been reached, facilitating seamless playback control.

#### Methods Overview:
- **Constructor**: Initializes the `WAVFileReader` with the specified file name and I2S pin configuration.
- **open()**: Opens the WAV file and prepares it for playback. Returns true if successful.
- **startPlayback()**: Begins audio playback from the current position in the WAV file.
- **stopPlayback()**: Stops playback and resets the current position to the beginning.
- **pausePlayback()**: Pauses playback, allowing for a temporary halt without losing progress.
- **resumePlayback()**: Resumes playback from the last paused position.
- **isEnd()**: Checks if the playback has reached the end of the audio data.
- **getSampleRate()**: Returns the sample rate of the audio data for compatibility checks.
- **readSample()**: Reads a single audio sample from the WAV file into the provided buffer.

#### Example Usage:
```cpp
#include "WAVFileReader.h"

// Initialize I2S pin configuration
i2s_pin_config_t i2sPins = {
    .bck_io_num = 26,   // Bit Clock Pin
    .ws_io_num = 25,    // Word Select Pin
    .data_out_num = 22, // Data Out Pin
    .data_in_num = I2S_PIN_NO_CHANGE
};

// Create a WAVFileReader instance
WAVFileReader wavReader("audio.wav", i2sPins);

void setup() {
    // Initialize SD card, I2S, etc.

    if (wavReader.open()) {
        wavReader.startPlayback(); // Start playback if the file opened successfully
    }
}

void loop() {
    // Other application logic
    if (wavReader.isEnd()) {
        wavReader.stopPlayback(); // Stop playback if end of file is reached
    }
}
```

### Conclusion
The `WAVFileReader` class is a powerful tool for integrating audio playback capabilities into ESP32-based projects. Its combination of WAV file handling, playback controls, and FreeRTOS task management makes it suitable for a variety of interactive audio applications, from simple sound effects to complex audio playback systems.

# WAVFileWriter Class

The `WAVFileWriter` class is a robust utility for creating and writing WAV audio files directly onto an SD card using the ESP32 platform. This class abstracts the complexities of handling WAV file structures, making it a crucial tool for any application involving audio recording or processing.

## Key Features:
- **WAV File Header Management**: Automatically constructs the WAV file header according to the WAV format specifications, including RIFF, WAVE, and fmt chunks.
- **Audio Sample Writing**: Capable of writing audio frames in both mono (1 channel) and stereo (2 channels) formats, allowing flexibility in audio data representation.
- **Dynamic Configuration**: Users can configure the number of audio channels, sampling rate, and expected duration at the time of object instantiation, accommodating various audio project requirements.
- **Automatic Data Length Calculation**: The class computes the data length based on the number of samples written, ensuring the WAV file header accurately reflects the file size upon closure.
- **File Management**: Efficiently handles file operations, including opening the file for writing and ensuring it is closed properly, preventing resource leaks and file corruption.

## Public Methods:
- **Constructor**: 
  - `WAVFileWriter(const char* file_name, short num_channels, int sample_rate, int duration_seconds, String Folder)`: Initializes the WAV file writer with the specified parameters and prepares the file for audio data writing.
  
- **writeFrame**: 
  - `void writeFrame(int16_t left_sample, int16_t right_sample)`: Writes a single frame of audio samples (left and right for stereo) to the WAV file. If in mono mode, the right sample can be set to zero.

- **close**: 
  - `void close()`: Finalizes the WAV file by writing the necessary header information and closing the file, ensuring all data is properly saved.

## Example Usage:

```cpp
#include <SD.h>
#include "WAVFileWriter.h"

// Create an instance of WAVFileWriter
WAVFileWriter wavWriter("audio.wav", 2, 44100, 60, "AudioFolder");

void setup() {
    // Initialize Serial for debugging
    Serial.begin(115200);

    // Initialize SD card
    if (!SD.begin()) {
        Serial.println("SD Card initialization failed!");
        return;
    }

    // Attempt to open the WAV file for writing
    if (wavWriter.open()) {
        Serial.println("WAV file opened successfully.");

        // Simulate writing audio frames
        for (int i = 0; i < 44100 * 60; ++i) { // 60 seconds of audio
            int16_t leftSample = random(-32768, 32767);  // Generate random sample data
            int16_t rightSample = random(-32768, 32767); // Generate random sample data
            wavWriter.writeFrame(leftSample, rightSample);
        }

        // Close the WAV file after writing
        wavWriter.close();
        Serial.println("WAV file written and closed successfully.");
    } else {
        Serial.println("Failed to open WAV file for writing.");
    }
}

void loop() {
    // Additional application logic can be implemented here
}
```

## Conclusion
The `WAVFileWriter` class is a vital component for developers working with audio recording on the ESP32 platform. By simplifying the process of WAV file management and audio data writing, it enables the rapid development of audio applications while ensuring compliance with standard WAV file formats. This class is especially useful in projects involving music playback, voice recording, or any other application where audio data needs to be stored in a WAV format.

# SpeakerManager Class

The `SpeakerManager` class is responsible for managing audio playback and recording functionalities on the ESP32 platform. It serves as a high-level interface that integrates various components such as the `WAVFileReader`, `WAVFileWriter`, and `MicManager` to facilitate audio operations. 

## Features
- **Audio Playback**: Control playback of WAV audio files with functions to start, stop, pause, and resume.
- **Volume Control**: Set and adjust the playback volume.
- **Audio Recording**: Record audio from a microphone and save it in WAV format.
- **Noise Reduction**: Apply basic noise reduction algorithms on recorded audio samples to enhance sound quality.

## Constructor
```cpp
SpeakerManager(
    WAVFileReader* wavfileReader, // Pointer to the WAVFileReader instance
    WAVFileWriter* wavfileWriter,   // Pointer to the WAVFileWriter instance
    MicManager* micManager,         // Pointer to the MicManager instance for audio input
    i2s_pin_config_t* i2sPins,     // I2S pin configuration structure
    I2SManager* i2SManager          // Pointer to the I2SManager for handling audio output
);
```

## Public Methods
- `void begin()`: Initializes the Speaker Manager.
- `void startPlayback(const char *file_name)`: Starts playback of the specified WAV file.
- `void stopPlayback()`: Stops the currently playing audio.
- `void pausePlayback()`: Pauses the audio playback.
- `void resumePlayback()`: Resumes the paused audio playback.
- `void setVolume(int volume)`: Sets the current playback volume.
- `void startRecording()`: Initiates audio recording.
- `void stopRecording()`: Stops the recording process.
- `void recordAudio(const int duration_seconds, const char *file_name, const int sample_rate, String Folder)`: Records audio for the specified duration and saves it as a WAV file.

## Dependencies
- **Preferences**: For storing configuration settings.
- **I2SManager**: For handling I2S audio output.
- **WAVFileReader**: For reading WAV file formats.
- **WAVFileWriter**: For writing audio data to WAV files.
- **MicManager**: For managing audio input from the microphone.

## Usage Example
```cpp
// Create instances of the required components
WAVFileReader wavReader("audio.wav");
WAVFileWriter wavWriter("output.wav", 2, 44100, 10, "/audio");
MicManager micManager;

// Initialize the Speaker Manager
SpeakerManager speakerManager(&wavReader, &wavWriter, &micManager, &i2sPins, &i2sManager);
speakerManager.begin();

// Start playback
speakerManager.startPlayback("audio.wav");

// Start recording
speakerManager.startRecording();
speakerManager.recordAudio(10, "recorded.wav", 44100, "/audio");
speakerManager.stopRecording();
```

## Notes
- Ensure proper configuration of I2S pins for audio playback and recording.
- The SD card must be initialized before reading/writing WAV files.

This `SpeakerManager` class streamlines audio management tasks, making it an essential component for any ESP32 audio application.


# SPIFlashManager Class

The `SPIFlashManager` class provides a robust interface for managing file operations on the SPIFFS (SPI Flash File System) of the ESP32 microcontroller. This class streamlines the process of file handling by enabling users to easily read, write, check for the existence of files, and obtain file sizes stored in flash memory. It is particularly useful in embedded applications where efficient storage and retrieval of data are crucial.

## Features
- **Initialization**: Use the `begin()` method to initialize the SPIFFS filesystem before performing any file operations.
- **File Writing**: The `writeFile()` method allows you to save data to files in the SPIFFS, enabling storage of arbitrary data types.
- **File Reading**: Retrieve stored data using the `readFile()` method, which reads the contents of a specified file into a buffer.
- **File Existence Check**: Quickly check if a file exists in the filesystem with the `fileExists()` method.
- **File Size Retrieval**: Get the size of a specific file with `getFileSize()`, useful for managing storage space.

## Usage Example
Here’s a basic example of how to use the `SPIFlashManager` class:

```cpp
SPIFlashManager flashManager;

// Initialize SPIFFS
flashManager.begin();

// Writing data to a file
const char* data = "Hello, World!";
if (flashManager.writeFile("/hello.txt", (const uint8_t*)data, strlen(data))) {
    Serial.println("File written successfully.");
}

// Reading data from a file
uint8_t buffer[50];
if (flashManager.readFile("/hello.txt", buffer, sizeof(buffer))) {
    Serial.println("File read successfully: " + String((char*)buffer));
}

// Checking if a file exists
if (flashManager.fileExists("/hello.txt")) {
    Serial.println("File exists.");
}

// Getting the size of a file
size_t fileSize = flashManager.getFileSize("/hello.txt");
Serial.print("File size: ");
Serial.println(fileSize);
```

## Error Handling
Each method in the `SPIFlashManager` class returns a boolean value indicating success or failure, allowing you to implement error handling as needed.

## Dependencies
- `Arduino.h`: Required for basic Arduino functionality.
- `SPIFFS.h`: Necessary for working with the SPIFFS filesystem.

The `SPIFlashManager` class is a vital tool for developers working on ESP32 projects, simplifying file management in embedded applications and enabling efficient resource usage.

## Getting Started

### Prerequisites

- **Hardware**: ESP32-S3, microphone, speaker, SD card, flash memory, LEDs, vibrating motors.
- **Software**: VS Code, PlatformIO for ESP32 development, Git for version control.

### Setup

1. Clone the repository:
   ```bash
   git clone https://github.com/SamuelCicuitStudio/PlushToyFirmware-V01.git
   cd SmartPlushToy
   ```
2. Open in VS Code and use PlatformIO to configure your build environment.
3. Upload firmware to your ESP32.

## Contributing

Feel free to open issues, suggest features, or contribute directly with pull requests.

## License

This project is licensed under the MIT License.

--- 
