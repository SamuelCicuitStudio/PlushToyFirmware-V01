#include "OtaManager.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Update.h>

HTTPClient http;
/**
 * @brief Constructs the OtaManager class and initializes the OTA system.
 * 
 * @param configManager Pointer to a ConfigManager instance that handles configuration data.
 */
OtaManager::OtaManager(ConfigManager* configManager):
 configManager(configManager){
// Constructor
begin();
}
/**
 * @brief Initializes the OTA Manager, setting the current firmware version and update URL.
 * 
 * This method retrieves the current firmware version from the configuration manager and 
 * sets the URL used for checking updates. It is called in the constructor.
 */
void OtaManager::begin() {
             if (DEBUGMODE) {
        Serial.println("###########################################################");
        Serial.println("#                  Starting OTA Manager                   #");
        Serial.println("###########################################################");
    }
    this->currentVersion = configManager->GetString(FIRMWARE_VERSION,DEFAULT_FIRMWARE_VERSION);
    this->updateURL = OTA_UPDATE_URL;
    Serial.println("OTA Update Initialized");
}
/**
 * @brief Checks for firmware updates by querying a remote server.
 * 
 * This method sends an HTTP GET request to the `updateURL` to retrieve version information
 * stored on the server. If a new version is available, the OTA process is initiated.
 */
void OtaManager::checkForUpdate() {
    // Query the server for version metadata (assumed to be a JSON file)

    http.begin(updateURL);  // URL where version metadata is hosted
    int httpCode = http.GET();
    
    if (httpCode == 200) {
        String payload = http.getString();
        Serial.println("Received version information: " + payload);
        
        // Parse JSON payload (assumed to have a key "version" and "firmwareURL")
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        
        latestVersion = doc["version"].as<String>();
        String firmwareURL = doc["firmwareURL"].as<String>();

        Serial.println("Latest Version: " + latestVersion);
        
        if (isNewVersionAvailable()) {
            Serial.println("New version available! Downloading firmware...");
            downloadAndUpdateFirmware(firmwareURL);
        } else {
            Serial.println("Already on the latest version.");
        }
    } else {
        Serial.printf("Failed to check for update. HTTP error code: %d\n", httpCode);
    }

    http.end();
}
/**
 * @brief Compares the current firmware version with the latest version.
 * 
 * @return true if a new version is available, false otherwise.
 */
bool OtaManager::isNewVersionAvailable() {
    return currentVersion != latestVersion;
}
/**
 * @brief Downloads and installs the firmware update from a specified URL.
 * 
 * This method initiates the download of the firmware binary, writes it to flash memory, 
 * and applies the update. It also verifies the integrity of the update and reboots the 
 * system if successful.
 * 
 * @param firmwareURL The URL from which the firmware binary is downloaded.
 */
void OtaManager::downloadAndUpdateFirmware(const String& firmwareURL) {

    http.begin(firmwareURL);  // URL to the firmware binary
    int httpCode = http.GET();

    if (httpCode == 200) {
        int contentLength = http.getSize();
        WiFiClient* client = http.getStreamPtr();

        // Begin firmware update
        if (!Update.begin(contentLength)) {
            Serial.println("Not enough space to begin OTA");
            return;
        }

        // Download and write firmware to flash memory
        size_t written = Update.writeStream(*client);
        if (written == contentLength) {
            Serial.println("Firmware successfully downloaded");
            configManager->GetString("FIRVER",latestVersion);
        } else {
            Serial.printf("Firmware download incomplete, only %d/%d bytes written\n", written, contentLength);
        }

        // End the update and check for success
        if (Update.end()) {
            Serial.println("OTA update completed");
            if (Update.isFinished()) {
                Serial.println("Update successfully applied. Rebooting...");
                configManager->RestartSysDelay(2000);
            } else {
                Serial.println("Update not finished. Something went wrong.");
            }
        } else {
            Serial.printf("Error Occurred. Error #: %d\n", Update.getError());
        }
    } else {
        Serial.printf("Failed to download firmware. HTTP error code: %d\n", httpCode);
    }

    http.end();
}