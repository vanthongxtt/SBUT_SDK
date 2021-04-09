#ifndef SBut_h
#define SBut_h

#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ArduinoJson.h"
#include <EEPROM.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "Config.h"
#include "EEPRomService.h"

#define EEPROM_WIFI_SSID_START 0
#define EEPROM_WIFI_SSID_END 19

#define EEPROM_WIFI_PASS_START 20
#define EEPROM_WIFI_PASS_END 39

#define EEPROM_SBUT_TOKEN_START 40
#define EEPROM_SBUT_TOKEN_END 100

#define EEPROM_SBUT_ID_START 101
#define EEPROM_SBUT_ID_END 120

class SButSdk
{

public:
    void begin(int nodeCounts, int sensorCounts, bool smartconfig, const char *ssid = "", const char *pass = "", const char *token = "");
    void loop();
    String getVersion();
    String SButThingId();
    int getNode(int node);
    void setNode(int node, int value);
    void setSensor(int sensor, int value);
    void writeNotification(const String &message);

private:
    void beginSmartConfig();
    void startWebServer();
    bool checkConnection();
    bool restoreConfig();
    bool httpRequest(const String &method, const String &request, String &response);
    void httpInsertSButServer(bool smartconfig);
    void reconnect();
    void isOnline();
    void syncNode();
    void checkButtonResetClick();
};

#endif
