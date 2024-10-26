#ifndef SPEAKER_MANAGER_H
#define SPEAKER_MANAGER_H

#include <Preferences.h>
#include "Config.h"
#include <driver/i2s.h>
#include <driver/adc.h>
#include "I2SManager.h"
#include "WAVFileReader.h"
#include "WAVFileWriter.h"
#include "MicManager.h"

/**
 * @class SpeakerManager
 * @brief A class to manage audio playback and recording using I2S on the ESP32 platform.
 *
 * The `SpeakerManager` class provides functionality for playing audio files and recording audio input. 
 * It integrates with the `WAVFileReader` and `WAVFileWriter` classes to handle WAV file formats, 
 * and utilizes the `MicManager` for capturing audio input. The class is designed to simplify 
 * audio management tasks, making it easier to implement audio playback and recording 
 * features in your ESP32 applications.
 *
 * Key functionalities include:
 * - Audio Playback: Start, stop, pause, and resume playback of WAV audio files.
 * - Volume Control: Set and manage playback volume levels.
 * - Audio Recording: Record audio from a microphone and save it in WAV format.
 * - Noise Reduction: Implement basic noise reduction algorithms on recorded audio samples.
 *
 * ## Example Usage:
 *
 * ```cpp
 * SpeakerManager speakerManager(&wavReader, &wavWriter, &micManager, &i2sPins, &i2sManager);
 * speakerManager.begin();
 * speakerManager.startPlayback("audio.wav");
 * speakerManager.startRecording();
 * ```
 *
 * ## Dependencies:
 * - The class relies on various dependencies such as `Preferences`, `I2SManager`, `WAVFileReader`, 
 *   `WAVFileWriter`, and `MicManager` to function correctly. Ensure these classes are correctly implemented 
 *   and included in your project.
 *
 * ## Note:
 * - Proper configuration of I2S pins is required for audio playback and recording.
 * - Ensure the SD card is initialized before attempting to read/write WAV files.
 */

class SpeakerManager {
public:
    // Constructor
    SpeakerManager(
        WAVFileReader* wavfileReader,
        WAVFileWriter* wavfileWriter,
        MicManager* micManager,
        i2s_pin_config_t* i2sPins,
        I2SManager* i2SManager);
    
    // Initialize the Speaker Manager
    void begin();

    // Playback control
    void startPlayback(const char *file_name);
    void stopPlayback();
    void pausePlayback();
    void resumePlayback();
    void setVolume(int volume);

    // Recording control
    void startRecording();
    void stopRecording();
    void recordAudio(const int duration_seconds, const char *file_name, const int sample_rate, String Folder);

private:
    int currentVolume;                  // Current volume level
    I2SManager* i2SManager;             // Pointer to I2S output object
    WAVFileReader* wavfileReader;       // Pointer to WAV file reader object
    WAVFileWriter* wavfileWriter;       // Pointer to WAV file writer object
    bool isPaused;                      // Playback pause state
    i2s_pin_config_t* i2sPins;          // I2S pin configuration structure
    MicManager* micManager;             // Pointer to the MicManager for audio input
    short int* buffer;                  // Buffer for audio samples
    short int applyNoiseReduction(short int sample, short int* noiseBuffer, int noiseSize); // Noise reduction function
};

#endif // SPEAKER_MANAGER_H
