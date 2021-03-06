#ifndef SButMQTT_H
#define SButMQTT_H

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "lib/PubSubClient/PubSubClient.h"

#include "SButConfig.h"
#include "SButDebug.h"
#include "SButHTTP.h"



class SButMQTTClass
{
public:
    SButMQTTClass();
    void beginMQTT(String thingId);
    void loopMQTT();
    void reconnect();
    void publish(String id, String value);
    String getTopic();
    String getMessage();

private:
    void subCloudConnect(String type);
};
extern SButMQTTClass SButMQTT;
#endif