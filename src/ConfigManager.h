#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "Config.h"  // Include Config.h for default values
#include <Preferences.h>
#include <esp_task_wdt.h>

class ConfigManager {
public:
    ConfigManager(Preferences preferences);
    ~ConfigManager();

    void begin();  // Initialize the configuration
    void end();    // End access to preferences

    // Public interface for saving and retrieving data
    void PutBool(const char* key, bool value);      // Save a boolean value
    void PutInt(const char* key, int value);        // Save an integer value
    void PutFloat(const char* key, float value);    // Save a float value
    void PutString(const char* key, const String& value);  // Save a string value
    void PutUInt(const char* key, int value);       // Save an unsigned integer value

    bool GetBool(const char* key, bool defaultValue);    // Retrieve a boolean value
    int GetInt(const char* key, int defaultValue);       // Retrieve an integer value
    float GetFloat(const char* key, float defaultValue); // Retrieve a float value
    String GetString(const char* key, const String& defaultValue);  // Retrieve a string value

    void RemoveKey(const char* key);  // Remove a specific key
    void ClearKey(); 
    bool GetAPFLag();                 // return ap flag
    void ResetAPFLag();//reset AP flag

    // System control methods
    void RestartSysDelay(unsigned long delayTime);  // Restart system with delay
    void simulatePowerDown();  // Simulate power down for testing

    // Preferences management
    void startPreferencesReadWrite();  // Open preferences in read-write mode
    void startPreferencesRead();       // Open preferences in read-only mode

    void SetAPFLag();  // Set the AP flag

private:
    // Private utility methods for internal use only
    void initializeDefaults();   // Initialize default values
    void initializeVariables();  // Initialize internal variables
    bool getResetFlag();         // Get system reset flag

    Preferences preferences;     // Preferences object to store configuration
    const char* namespaceName;   // Namespace for the preferences storage
};

#endif // CONFIG_MANAGER_H