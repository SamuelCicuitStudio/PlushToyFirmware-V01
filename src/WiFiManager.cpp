/**
 * @file WiFiManager.h
 * @brief This file contains the implementation of the WiFiManager class for managing 
 *        Wi-Fi connections and access point functionality on ESP32 devices.
 *
 * The WiFiManager class handles the initialization and connection of Wi-Fi, as well as
 * the creation of an access point (AP) for configuration purposes. It provides methods to 
 * set AP credentials, connect to Wi-Fi, and handle web server requests for managing 
 * Wi-Fi settings and GPIO controls.
 */
#include "WiFiManager.h"

/**
 * @brief Constructor for the WiFiManager class.
 *
 * Initializes the WiFiManager object, setting default values for the access point 
 * credentials and other configurations.
 */
WiFiManager::WiFiManager(ConfigManager* configManager):configManager(configManager),server(80),isAPMode(false), apSSID(DEFAULT_AP_SSID),apPassword(DEFAULT_AP_PASSWORD){}
/**
 * @brief Begins the WiFiManager initialization process.
 *
 * This method mounts the SPIFFS filesystem, checks the configuration for 
 * the connection mode (AP or Wi-Fi), and starts the appropriate connection process.
 */
void WiFiManager::begin() {
    if (DEBUGMODE) {
        Serial.begin(115200);
        Serial.println("###########################################################");
        Serial.println("#                 Starting WIFI Manager                   #");
        Serial.println("###########################################################");
        
        if (!SPIFFS.begin(true)) {
            Serial.println("An error has occurred while mounting SPIFFS");
            return;
        }
        Serial.println("SPIFFS mounted successfully");
        Serial.println("WiFiManager: Begin initialization");
    }

    // Determine the mode to start in (AP or WiFi)
    bool startAP = configManager->GetAPFLag();
    if (DEBUGMODE) {
        Serial.printf("WiFiManager: Start mode - %s\n", startAP ? "AP" : "WiFi");
    }

    // Start in access point mode or connect to WiFi based on the flag
    startAP ? startAccessPoint() : connectToWiFi();
}

/**
 * @brief Sets the credentials for the access point.
 *
 * @param ssid The SSID of the access point.
 * @param password The password for the access point.
 */
void WiFiManager::setAPCredentials(const char* ssid, const char* password) {
    apSSID = ssid;
    apPassword = password;

    if (DEBUGMODE) {
        Serial.print("\n WiFiManager: AP credentials set - SSID: ");
        Serial.print(ssid);
        Serial.print(", Password: ");
        Serial.println(password);
    }
}

/**
 * @brief Connects to the specified Wi-Fi network.
 *
 * Attempts to connect to the Wi-Fi using stored credentials. If the connection fails, 
 * it defaults to starting the access point.
 */
void WiFiManager::connectToWiFi() {
    String ssid = configManager->GetString(WIFISSID, "");
    String password = configManager->GetString(WIFIPASS, "");

    if (DEBUGMODE) {
        Serial.print("WiFiManager: Attempting to connect to WiFi\n - SSID: ");
        Serial.print(ssid);
        Serial.print(", Password: ");
        Serial.println(password);
    }

    if (ssid == "" || password == "") {
        startAccessPoint();
    } else {
        WiFi.begin(ssid.c_str(), password.c_str());
        unsigned long startAttemptTime = millis();

        if (DEBUGMODE) {
            Serial.println("WiFiManager: Connecting to WiFi...");
        }

        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
            delay(500);
            if (DEBUGMODE) {
                Serial.print(".");
            }
        }

        if (WiFi.status() == WL_CONNECTED) {
            if (DEBUGMODE) {
                Serial.print("\nWiFiManager: Connected to WiFi,\nIP Address: ");
                Serial.println(WiFi.localIP());
                setServerCallback();
            }
            server.begin(); // Start web server
        } else {
            if (DEBUGMODE) {
                Serial.println("WiFiManager: Failed to connect to WiFi.\nSwitching to AP mode.");
                configManager->SetAPFLag(); // Set flag to start in AP mode next time
                configManager->RestartSysDelay(3000);
            }
        }
    }
}


/**
 * @brief Starts the access point mode.
 *
 * Sets the ESP32 to access point mode, allowing devices to connect and configure the 
 * Wi-Fi settings.
 */
void WiFiManager::startAccessPoint() {
    if (DEBUGMODE) {
        Serial.println("WiFiManager: Starting Access Point");
    }

    WiFi.disconnect();
    delay(100);

    WiFi.softAP(apSSID.c_str(), apPassword.c_str());

    if (DEBUGMODE) {
        Serial.print("WiFiManager: AP Started - IP Address: ");
        Serial.println(WiFi.softAPIP());
    }

    isAPMode = true;

    setServerCallback();//set the server Callbacks
}

/**
 * @brief Sets up the server callbacks for handling web requests.
 *
 * Configures the web server endpoints for handling root requests, saving Wi-Fi credentials,
 * serving static files, and controlling GPIO.
 */
void WiFiManager:: setServerCallback(){
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest* request) { handleRoot(request); });
    server.on("/saveWiFi", HTTP_POST, [this](AsyncWebServerRequest* request) { handleSaveWiFi(request); });
    server.on("/wifiCredentialsPage", HTTP_GET, [this](AsyncWebServerRequest* request) { handleSetWiFi(request); });
    server.on("/gpioctrl", HTTP_GET, [this](AsyncWebServerRequest* request) { handleGPIO(request); });
    
    // Endpoint to toggle LED
    server.on("/set_led", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (DEBUGMODE) {
        Serial.println("WiFiManager: Handling set led request");
    };
        if (request->hasParam("led")) {
            String led = request->getParam("led")->value();
            if (led == "1") {
                led1State = !led1State; // Toggle LED 1 state
                digitalWrite(LED_PIN, led1State ? LOW : HIGH);
            } else if (led == "2") {
                led2State = !led2State; // Toggle LED 2 state
                digitalWrite(LED_PIN, led2State ? LOW : HIGH);
            }
        }
        request->send(200, "text/plain", "OK");
    });


    // Endpoint to get status of LEDs and buttons
    server.on("/status", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (DEBUGMODE) {
        Serial.println("WiFiManager: Handling gpio status request");
    };
        String json = "{";
        json += "\"led1\":" + String(led1State ? "true" : "false") + ",";
        json += "\"led2\":" + String(led2State ? "true" : "false") + ",";
        json += "\"button1\":" + String(digitalRead(BUTTON_01_PIN) == LOW ? "true" : "false") + ",";
        json += "\"button2\":" + String(digitalRead(BUTTON_02_PIN) == LOW ? "true" : "false");
        json += "}";
        request->send(200, "application/json", json);
    });


    // Serve static files like icons, CSS, JS, etc.
    server.serveStatic("/icons/", SPIFFS, "/icons/").setCacheControl("max-age=86400");
    server.begin();
}
/**
 * @brief Handles requests to the root endpoint.
 *
 * @param request The incoming web request.
 */
void WiFiManager::handleRoot(AsyncWebServerRequest* request) {
    if (DEBUGMODE) {
        Serial.println("WiFiManager: Handling welcome root request");
    }

    request->send(SPIFFS, "/welcome.html", "text/html");
}

/**
 * @brief Handles requests for the Wi-Fi credentials page.
 *
 * @param request The incoming web request.
 */
void WiFiManager::handleSetWiFi(AsyncWebServerRequest* request) {
    if (DEBUGMODE) {
        Serial.println("WiFiManager: Handling set wifi request");
    }

    request->send(SPIFFS, "/wifiCredentialsPage.html", "text/html");
}
/**
 * @brief Handles requests for the GPIO management page.
 *
 * @param request The incoming web request.
 */
void WiFiManager::handleGPIO(AsyncWebServerRequest* request) {
    if (DEBUGMODE) {
        Serial.println("WiFiManager: Handling set GPIO request");
    }

    request->send(SPIFFS, "/gpiomanager.html", "text/html");
}
/**
 * @brief Handles saving the Wi-Fi credentials.
 *
 * @param request The incoming web request containing the SSID and password.
 */
void WiFiManager::handleSaveWiFi(AsyncWebServerRequest* request) {
    if (DEBUGMODE) {
        Serial.println("WiFiManager: Handling save WiFi request");
    }

    if (request->hasParam("ssid", true) && request->hasParam("password", true)) {
        String ssid = request->getParam("ssid", true)->value();
        String password = request->getParam("password", true)->value();

        if (DEBUGMODE) {
            Serial.print("WiFiManager: Received credentials - SSID: ");
            Serial.print(ssid);
            Serial.print(", Password: ");
            Serial.println(password);
        }

        if (ssid != "" && password != "") {
            configManager->PutString(WIFISSID, ssid);
            configManager->PutString(WIFIPASS, password);
            configManager->ResetAPFLag();
            request->send(SPIFFS, "/thankyou_page.html", "text/html");
            configManager->RestartSysDelay(3000);
        } else {
            request->send(400, "text/plain", "Invalid SSID or Password.");
        }
    } else {
        request->send(400, "text/plain", "Missing parameters.");
    }
}



