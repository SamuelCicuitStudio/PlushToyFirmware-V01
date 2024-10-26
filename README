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
2. **Power Management**: **PowerManager** checks battery status and sets constraints for system operation (e.g., disables games when battery is low).
3. **I/O Management**:
   - **MicManager** collects audio input data.
   - **SpeakerManager** provides audio output for responses and game sounds.
4. **Data Handling**:
   - **SDCardManager** and **SPIFlashManager** manage audio and configuration data.
   - **WavFileReader** and **WavFileWriter** provide data interfaces for reading and saving `.wav` files.
5. **Interactive Features**:
   - **VocalCommandManager** processes user commands and initiates interactions.
   - **RandomBehaviorManager** and **GameManager** provide feedback and engage users through LED animations, audio, and vibrations.
6. **Text and Audio Processing**:
   - **TextToSpeechManager** generates responses, while **SpeechToTextManager** converts audio to text for command recognition.

### Example Flow: "Play Sound" Command

1. **MicManager** captures audio, and **SpeechToTextManager** identifies the "play sound" command.
2. **VocalCommandManager** triggers **SpeakerManager** to play an audio file, using **WavFileReader** to load data from the SD card.
3. **RandomBehaviorManager** may trigger LED animations or vibration patterns to accompany the sound.

## Getting Started

### Prerequisites

- **Hardware**: ESP32-S3, microphone, speaker, SD card, flash memory, LEDs, vibrating motors.
- **Software**: VS Code, PlatformIO for ESP32 development, Git for version control.

### Setup

1. Clone the repository:
   ```bash
   git clone https://github.com/YourUsername/SmartPlushToy.git
   cd SmartPlushToy
   ```
2. Open in VS Code and use PlatformIO to configure your build environment.
3. Upload firmware to your ESP32.

## Contributing

Feel free to open issues, suggest features, or contribute directly with pull requests.

## License

This project is licensed under the MIT License.

--- 
