#ifndef OTAUPDATE_H
#define OTAUPDATE_H
/**
 * @file OTAUpdate.h
 * @brief Header file for the OtaManager class, which manages Over-the-Air (OTA) firmware updates.
 *
 * This class allows an ESP32 device to check for firmware updates from a remote server, download the latest version, 
 * and apply the update, if available. It leverages a configuration manager to retrieve settings and maintains version control 
 * for efficient update checks.
 *
 * @details
 * The OtaManager class is responsible for:
 * - Managing the firmware update process, including retrieving version metadata and downloading new firmware versions.
 * - Checking for a newer firmware version by comparing the current and latest version values.
 * - Using ConfigManager to store and access configuration data, such as the firmware version and update URL.
 *
 * @dependencies
 * - Arduino.h: Provides basic Arduino functions.
 * - ConfigManager: Manages configuration data for accessing firmware version and settings.
 *
 * Example Usage:
 * @code
 * #include "OTAUpdate.h"
 * ConfigManager configManager;
 * PowerManager* powerManager;
 * OtaManager otaManager(&configManager,powerManager);
 * 
 * void setup() {
 *     otaManager.begin();  // Initialize OTA with current version
 *     otaManager.checkForUpdate();  // Check for a new firmware version
 * }
 * 
 * void loop() {
 *     // Regular application code
 * }
 * @endcode
 */
#include <Arduino.h>
#include "PowerManager.h"
class OtaManager {
public:
    OtaManager(ConfigManager* configManager,PowerManager* powerManager);  // Constructor

    void begin();  // Initialize OTA with current version and update URL
    void checkForUpdate();  // Check if a new version is available
    void downloadAndUpdateFirmware(const String& firmwareURL);  // Download and update firmware

private:
    String currentVersion;
    String updateURL;
    String latestVersion;
    ConfigManager* configManager;
    PowerManager* powerManager;
    bool isNewVersionAvailable();
};

#endif  // OTAUPDATE_H