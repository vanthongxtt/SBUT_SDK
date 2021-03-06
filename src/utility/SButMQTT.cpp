#include "Sbut/SButMQTT.h"

WiFiClient client;

PubSubClient sButMqttClient(client);

String idThing = "";

String msgMQTT = "";
String topicMQTT = "";

SButMQTTClass::SButMQTTClass(){

};
void callback(char *topic, byte *payload, unsigned int length)
{

    payload[length] = '\0';

    msgMQTT = (char *)payload;
    topicMQTT = String(topic);

    Serial.println(msgMQTT.c_str());
    Serial.println(topicMQTT.c_str());
}
void SButMQTTClass::beginMQTT(String thingId)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        idThing = thingId;
        sButMqttClient.setServer(SBUT_DEFAULT_DOMAIN_MQTT, SBUT_DEFAULT_PORT_MQTT);
        sButMqttClient.setCallback(callback);
    }
}
void SButMQTTClass::loopMQTT()
{
    if (!sButMqttClient.connected())
    {
        reconnect();
    }
    sButMqttClient.loop();
}

void SButMQTTClass::reconnect()
{
    while (!sButMqttClient.connected())
    {
        Serial.print("Connect MQTT...");
        String clientId = "SBUT_LIB_SDK_THING_";
        clientId += idThing;
        if (sButMqttClient.connect(clientId.c_str(), "", ""))
        {
            Serial.println("OK");

            String subUpdate = "v1/sdk/esp/";
            subUpdate += idThing;
            subUpdate += "/OTA";
            const char *subOTA = subUpdate.c_str();
            sButMqttClient.subscribe(subOTA);

            subCloudConnect("getNode");
            subCloudConnect("getSlider");
        }
        else
        {
            Serial.print("ERROR : ");
            Serial.print(sButMqttClient.state());
            Serial.println("5 secondes reconnect");
            delay(5000);
        }
    }
}
void SButMQTTClass::subCloudConnect(String type)
{
    String subCloud = "v1/sdk/esp/";
    subCloud += type.c_str();
    subCloud += "/";
    subCloud += idThing;
    subCloud += "/#";
    const char *s = subCloud.c_str();
    Serial.println(s);
    sButMqttClient.subscribe(s);
}
void SButMQTTClass::publish(String uri, String value)
{
    sButMqttClient.publish(uri.c_str(), value.c_str());
}
String SButMQTTClass::getTopic()
{
    return topicMQTT;
}
String SButMQTTClass::getMessage()
{
    return msgMQTT;
}

SButMQTTClass SButMQTT;