#ifndef SBUT_HTTP_H
#define SBUT_HTTP_H

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>


#include "SButConfig.h"

#include "lib/ArduinoJson.h"

#include "SButDebug.h"

class SButHTTPClass
{
public:
    SButHTTPClass();
    void httpCreateThing(const String token, const String thingId, const String Ip, const String board, const String version);
    void httpUpdateWidgetInforThing(const String token, const String thingId, const String Ip, const String board);
    void httpCreateNodeAndSensor(const String thingId, const int nodes, const int sensors);
    bool httpRequest(const String &method, const String &request, String &response);
    void updateThingVersion(const String thingId, const String version);
    int getNodeCloud(const String thingId);
    void updateOTA(const String serverUrl, const String version);
};

extern SButHTTPClass SButHTTP;

#endif 
