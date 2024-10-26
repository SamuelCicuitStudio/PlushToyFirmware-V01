---

# SmartPlushToy Firmware

Welcome to the **SmartPlushToy** firmware repository! This project provides firmware for an ESP32-based plush toy designed for interaction with children through audio feedback, LED displays, games, and more. This document describes the overall architecture, class structure, and basic usage information for developers working on or contributing to this project.

## Project Structure

The firmware for SmartPlushToy is modular, with each key system component encapsulated in separate classes for better code organization, modularity, and maintainability. Below is an overview of each primary class, its purpose, and interconnections within the system.

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
