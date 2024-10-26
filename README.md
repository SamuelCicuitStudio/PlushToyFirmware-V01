---

# SmartPlushToy Firmware

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
                     │ MicManager │   │SpeakerManager│   │ SDCardManager│
                     └────────────┘   └──────────────┘   └──────────────┘
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
                                   ┌──────────────────┐
                                   │   WavFileReader  │
                                   └──────────────────┘
                                           │
                                           ▼
                                   ┌──────────────────┐
                                   │   WavFileWriter  │
                                   └──────────────────┘
                                           │
                                           ▼
                               ┌───────────┼───────────┐
                               │                       │
                               ▼                       ▼
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

- **`OtaManager(ConfigManager* configManager)`**
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
#include "ConfigManager.h"

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



