
#ifndef SBUT_API_H
#define SBUT_API_H

#include "Arduino.h"

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>

#include "SButConfig.h"
#include "SButDetectDevice.h"
#include "SButEEPROM.h"

#include "SButMQTT.h"
#include "SButHTTP.h"

#define SButApiClass SButSDK

class SButApiClass
{
public:
    SButApiClass();
    ~SButApiClass();
    String thingId();
    void begin(const char *ssid, const char *pass, const char *token, const int nodeCounts = 0, const int sensorCounts = 0);
    void beginSmartConfig(const int nodeCounts = 0, const int sensorCounts = 0);
    void loop();
    void setSensor(int id, int value);
    int getNode(int id);
    void setNode(int id, int value);
    bool isConnected();
    void notify(const String &message);
    void setVersion(const String version);

private:
    bool checkConnection();
    bool restoreSmartConfig();
    void createSmartConfig();
    void startWebServer();
    void syncNode();
    void checkButtonResetClick();
};

#endif