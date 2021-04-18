#include "SBut.h"
const char *apSSID = "SBUT_SETUP_";
const char *apPassword = "12345678";
const IPAddress apIP(192, 168, 1, 5);
boolean settingMode;
DNSServer dnsServer;
WiFiClient client;
String ssidList;
String letters[10] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
String lettersSButRandom[66] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
char message_buff[100];
String randStringSButThing = "";
bool debug = true;
long lastMsg = 0;
long lastRecu = 0;
int nodesCount = 0;
int sensorsCount = 0;
String SBUT_THING_ID = "";
String TOKEN_ID = "";
String topicCB = "";
String messageCB = "";
unsigned long valSBUt = 0;
unsigned long valSync = 0;
unsigned long settingTimeout = 0;
int appMode = NORMAL_MODE;
int tempSync = 0;
ESP8266WebServer webServer(80);
PubSubClient mqttClient(client);
void callback(char *topic, byte *payload, unsigned int length)
{

    int i = 0;
    if (debug)
    {
        Serial.println("Message recu =>  topic: " + String(topic));
        Serial.print(" | longueur: " + String(length, DEC));
    }

    for (i = 0; i < length; i++)
    {
        message_buff[i] = payload[i];
    }
    message_buff[i] = '\0';

    String msgString = String(message_buff);
    topicCB = String(topic);

    messageCB = msgString;
    if (debug)
    {
        Serial.println("Payload: " + msgString);
    }
}
void SButSdk::begin(int nodeCounts, int sensorCounts, bool smartconfig, const char *ssid, const char *pass, const char *token)
{
    Serial.begin(115200);
    Serial.println("");
    Serial.println("");
    Serial.println("Welcome to SBUT Platform");
    Serial.println("");
    Serial.println("");
    pinMode(0, INPUT_PULLUP);
    nodesCount = nodeCounts;
    sensorsCount = sensorCounts;
    for (int i = 0; i < 13; i++)
    {
        randStringSButThing = randStringSButThing + lettersSButRandom[random(0, 66)];
    }
    if (smartconfig)
    {
        if (restoreConfig())
        {
            if (checkConnection())
            {
                settingMode = false;
                startWebServer();
                mqttClient.setServer(SBUT_DEFAULT_DOMAIN_MQTT, SBUT_DEFAULT_PORT_MQTT);
                mqttClient.setCallback(callback);
                return;
            }
        }
        else
        {
            settingMode = true;
        }
        beginSmartConfig();
    }
    else
    {

        WiFi.begin(ssid, pass);
        String tokens = String(token);
        if (checkConnection())
        {
            delay(2000);
            TOKEN_ID = tokens;
            httpInsertSButServer(false);
            settingMode = false;
            mqttClient.setServer(SBUT_DEFAULT_DOMAIN_MQTT, SBUT_DEFAULT_PORT_MQTT);
            mqttClient.setCallback(callback);
            return;
        }
    }
}
String SButSdk::SButThingId()
{
    return randStringSButThing;
}
void SButSdk::loop()
{
    if (settingMode)
    {
        dnsServer.processNextRequest();
    }
    else
    {
        if (!mqttClient.connected())
        {
            reconnect();
        }
        mqttClient.loop();
    }
    checkButtonResetClick();
    isOnline();
    syncNode(); // ham dong bo node
    webServer.handleClient();
}
bool SButSdk::checkConnection()
{
    int count = 0;
    Serial.println("Waiting for Wi-Fi connection");
    while (count < 30)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println();
            Serial.println("Connected!");
            WiFi.mode(WIFI_STA);
            return (true);
        }
        delay(500);
        Serial.print(".");
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        count++;
    }
    Serial.println("Timed out.");
    return false;
}
bool SButSdk::restoreConfig()
{
    String ssid = EEPROMH.read(EEPROM_WIFI_SSID_START, EEPROM_WIFI_SSID_END);
    String pass = EEPROMH.read(EEPROM_WIFI_PASS_START, EEPROM_WIFI_PASS_END);
    if (ssid.length() > 0 && pass.length() > 0)
    {
        WiFi.begin(ssid.c_str(), pass.c_str());
        return true;
    }
    else
    {
        return false;
    }
}
void SButSdk::beginSmartConfig()
{
    String randomString = "";
    for (int i = 0; i < 10; i++)
    {
        randomString += letters[random(0, 10)];
    }
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(apSSID + randomString, apPassword);
    dnsServer.start(53, "*", apIP);
    Serial.print("Starting Access Point at \"");
    Serial.print(apSSID + randomString);
    Serial.println("\"");
    startWebServer();
}
void SButSdk::startWebServer()
{
    if (settingMode)
    {
        Serial.print("Starting Web Server at ");
        Serial.println(WiFi.softAPIP());
        webServer.on("/setap", []() {
            String ssid = webServer.arg("ssid");
            String passwifi = webServer.arg("passwifi");
            String token = webServer.arg("token");

            EEPROMH.write(ssid, EEPROM_WIFI_SSID_START, EEPROM_WIFI_SSID_END);
            EEPROMH.write(passwifi, EEPROM_WIFI_PASS_START, EEPROM_WIFI_PASS_END);
            EEPROMH.write(randStringSButThing, EEPROM_SBUT_ID_START, EEPROM_SBUT_ID_END);
            EEPROMH.write(token, EEPROM_SBUT_TOKEN_START, EEPROM_SBUT_TOKEN_END);
            EEPROMH.commit();

            String message = "{\"status\": 1}";
            webServer.send(200, "application/json", message);
            delay(5000);
            ESP.restart();
        });
        webServer.onNotFound([]() {
            String s = "<h1>SBUT AP mode</h1><p><a href=\"/settings\">Wi-Fi Settings</a></p>";
            webServer.send(200, "text/html", s);
        });
    }
    else
    {
        httpInsertSButServer(true);

        webServer.on("/reset", []() {
            for (int i = 0; i < 512; ++i)
            {
                EEPROM.write(i, 0);
            }
            EEPROM.commit();
            String s = "<h1>Wi-Fi settings was reset.</h1><p>Please reset device.</p>";
            // webServer.send(200, "text/html", makePage("Reset Wi-Fi Settings", s));
            ESP.restart();
        });
    }
    webServer.begin();
}
void SButSdk::httpInsertSButServer(bool smartconfig)
{
    String response;
    String pStr = "";
    String token = EEPROMH.read(EEPROM_SBUT_TOKEN_START, EEPROM_SBUT_TOKEN_END);
    String SBUT_ID = EEPROMH.read(EEPROM_SBUT_ID_START, EEPROM_SBUT_ID_END);
    digitalWrite(LED_BUILTIN, HIGH);
    if (smartconfig == false)
    {
        if (SBUT_ID.length() == 0)
        {
            SBUT_THING_ID = randStringSButThing;
            EEPROMH.write(SBUT_THING_ID, EEPROM_SBUT_ID_START, EEPROM_SBUT_ID_END);
            EEPROMH.write(TOKEN_ID, EEPROM_SBUT_TOKEN_START, EEPROM_SBUT_TOKEN_END);
            EEPROMH.commit();
            String httpRequestData = "nodes=" + String(nodesCount) + "&sensors=" + String(sensorsCount) + "&thingId=" + SBUT_THING_ID + "&ipAddress=" + WiFi.localIP().toString();
            if (httpRequest(String("POST /api/v1/thing/") + TOKEN_ID.c_str() + "/addThing", httpRequestData, response))
            {
                if (response.length() != 0)
                {
                    digitalWrite(LED_BUILTIN, HIGH);
                    Serial.print("WARNING: ");
                    Serial.println(response);
                }
            }
        }
        else
        {
            SBUT_THING_ID = SBUT_ID;
        }
    }
    else
    {
        if (SBUT_ID.length() > 0)
        {
            SBUT_THING_ID = SBUT_ID;
            TOKEN_ID = token;
            String httpRequestData = "nodes=" + String(nodesCount) + "&sensors=" + String(sensorsCount) + "&thingId=" + SBUT_ID + "&ipAddress=" + WiFi.localIP().toString();
            if (httpRequest(String("POST /api/v1/thing/") + TOKEN_ID.c_str() + "/addThing", httpRequestData, response))
            {
                if (response.length() != 0)
                {
                    digitalWrite(LED_BUILTIN, HIGH);
                    Serial.print("WARNING: ");
                    Serial.println(response);
                }
            }
        }
        else
        {
            EEPROMH.write(randStringSButThing, EEPROM_SBUT_ID_START, EEPROM_SBUT_ID_END);
            EEPROMH.write(TOKEN_ID, EEPROM_SBUT_TOKEN_START, EEPROM_SBUT_TOKEN_END);
            EEPROMH.commit();
            String httpRequestData = "nodes=" + String(nodesCount) + "&sensors=" + String(sensorsCount) + "&thingId=" + SBUT_ID + "&ipAddress=" + WiFi.localIP().toString();
            if (httpRequest(String("POST /api/v1/thing/") + TOKEN_ID.c_str() + "/addThing", httpRequestData, response))
            {
                if (response.length() != 0)
                {
                    digitalWrite(LED_BUILTIN, HIGH);
                    Serial.print("WARNING: ");
                    Serial.println(response);
                }
            }
        }
    }

    delay(3000L);
}
bool SButSdk::httpRequest(const String &method, const String &request, String &response)
{
    if (client.connect(SBUT_DEFAULT_DOMAIN_CLOUD, 3000))
    {
        // Serial.println("OK");
    }
    else
    {
        Serial.println("failed");
        return false;
    }

    client.print(method);
    client.println(F(" HTTP/1.1"));
    client.print(F("Host: "));
    client.println(SBUT_DEFAULT_DOMAIN_CLOUD);
    client.println(F("Connection: close"));
    if (request.length())
    {
        client.println(F("Content-Type: application/x-www-form-urlencoded"));
        client.print(F("Content-Length: "));
        client.println(request.length());
        client.println();
        client.print(request);
    }
    else
    {
        client.println();
    }

    //Serial.println("Waiting response");
    int timeout = millis() + 5000;
    while (client.available() == 0)
    {
        if (timeout - millis() < 0)
        {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return false;
        }
    }

    //Serial.println("Reading response");
    int contentLength = -1;
    while (client.available())
    {
        String line = client.readStringUntil('\n');
        line.trim();
        line.toLowerCase();
        if (line.startsWith("content-length:"))
        {
            contentLength = line.substring(line.lastIndexOf(':') + 1).toInt();
        }
        else if (line.length() == 0)
        {
            break;
        }
    }
    response = "";
    response.reserve(contentLength + 1);
    while (response.length() < contentLength && client.connected())
    {
        while (client.available())
        {
            char c = client.read();
            response += c;
        }
    }
    client.stop();
    return true;
}
// int SButSdk::getNode(int node)
// {

//     JSONVar myObject = JSON.parse(messageCB);
//     if (JSON.typeof(myObject) == "undefined")
//     {
//         // Serial.println("Parsing input failed!");
//         return false;
//     }

  
//     String pinMode = String((const char *)myObject[node]["pinMode"]);
//     // Serial.println((int)myObject[node]["state"]);
//     String nod = String(node);
//     // String sub = "esp/node/";
//     // sub += SBUT_THING_ID;
//     // sub += "/";
//     // sub += nod;
//     if (nod == pinMode)
//     {
//         String state = String((const char *)myObject[node]["state"]);
//         int convertedstring = atoi(state.c_str());
//         return convertedstring;
//     }
//     // return false;
// }
int SButSdk::getNode(int node)
{

    String nod = String(node);
    String sub = "esp/node/";
    sub += SBUT_THING_ID;
    sub += "/";
    sub += nod;

    if (topicCB == sub.c_str())
    {
        return messageCB.toInt();
    }
    // return false;
}
void SButSdk::setNode(int node, int value)
{
    String n = String(node);
    String v = String(value);
    String s = "esp/node/";
    s += SBUT_THING_ID;
    s += "/";
    s += n;
    s += "/update";
    mqttClient.publish(s.c_str(), v.c_str());
}
void SButSdk::setSensor(int sensor, int value)
{
    String sen = String(sensor);
    String v = String(value);
    String s = "esp/sensor/";
    s += SBUT_THING_ID;
    s += "/";
    s += sen;
    s += "/update";
    mqttClient.publish(s.c_str(), v.c_str());
}
void SButSdk::reconnect()
{
    while (!mqttClient.connected())
    {
        Serial.print("Connexion au serveur MQTT...");
        String clientId = "SButClient_";
        clientId += SBUT_THING_ID;
        if (mqttClient.connect(clientId.c_str(), "", ""))
        {
            Serial.println("OK");
            for (int i = 0; i < nodesCount; i++)
            {
                String node = String(i);
                String sub = "esp/node/";
                sub += SBUT_THING_ID;
                sub += "/";
                sub += node;

                Serial.println(sub);
                const char *s = sub.c_str();
                mqttClient.subscribe(s);
            }
            // String sub = "esp/node/";
            // sub += SBUT_THING_ID;

            // Serial.println(sub);
            // const char *s = sub.c_str();
            // mqttClient.subscribe(s);
        }
        else
        {
            Serial.print("ERROR : ");
            Serial.print(mqttClient.state());
            Serial.println(" On attend 5 secondes avant de recommencer");
            delay(5000);
        }
    }
}
void SButSdk::isOnline()
{
    if (((unsigned long)(millis() - valSBUt)) > 8000)
    {
        String s = "esp/registerOnline/";
        s += SBUT_THING_ID;
        mqttClient.publish(s.c_str(), "online");
        valSBUt = millis();
    }
}
void SButSdk::syncNode()
{
    if (((unsigned long)(millis() - valSync)) > 10000)
    {
        if (tempSync == 0)
        {
            String s = "esp/nodeOnl/";
            s += SBUT_THING_ID;
            s += "/sync";
            mqttClient.publish(s.c_str(), "ok");
            tempSync = 1;
        }

        valSync = millis();
    }
}
void SButSdk::writeNotification(const String &message)
{
    String s = "esp/notify/";
    s += TOKEN_ID;
    // String mes = "{'thingId': '"+SBUT_THING_ID+"','message': '"+message+"'}";
    String mess = SBUT_THING_ID;
    mess += "/";
    mess += message;
    mqttClient.publish(s.c_str(), mess.c_str());
}
String SButSdk::getVersion()
{
    return SBUT_VERSION;
}

void SButSdk::checkButtonResetClick()
{
    if (digitalRead(0) == LOW && (settingTimeout + SETTING_HOLD_TIME) <= millis() && appMode != SERVER_MODE)
    {
        settingTimeout = millis();
        if (appMode != SERVER_MODE)
        {
            appMode = SERVER_MODE;
            ECHOLN("[WifiService][SBUT RESET] SBUT RESET....");
            EEPROMH.clear(0, 512);
            EEPROM.end();
            delay(3000);
            ESP.restart();
        }
    }
    else if (digitalRead(0) == HIGH)
    {
        settingTimeout = millis();
    }
}