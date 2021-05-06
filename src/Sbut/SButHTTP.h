#ifndef SBUT_HTTP_H
#define SBUT_HTTP_H

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>


#include "SButConfig.h"

#include "lib/ArduinoJson.h"

#include "SButDebug.h"

class SButHTTPClass
{
public:
    SButHTTPClass();
    void httpCreateThing(const String token, const String thingId, const String Ip);
    void httpCreateNodeAndSensor(const String thingId, const int nodes, const int sensors);
    bool httpRequest(const String &method, const String &request, String &response);
    int getNodeCloud(const String thingId);
};

extern SButHTTPClass SButHTTP;

#endif 
