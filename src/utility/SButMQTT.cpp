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
        String clientId = "SButClient_";
        clientId += idThing;
        if (sButMqttClient.connect(clientId.c_str(), "", ""))
        {
            Serial.println("OK");
            int nodes = SButHTTP.getNodeCloud(idThing);
            if (nodes > 0)
            {
                for (int i = 0; i < nodes; i++)
                {
                    String node = String(i);
                    String sub = "esp/node/";
                    sub += idThing;
                    sub += "/";
                    sub += node;
                    Serial.println(sub);
                    const char *s = sub.c_str();
                    sButMqttClient.subscribe(s);
                }
            }
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