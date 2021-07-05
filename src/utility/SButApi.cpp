#include "Sbut/SButApi.h"

ESP8266WebServer webServer(80);
DNSServer dnsServer;
const IPAddress apIP(192, 168, 1, 5);
const char *alphabeth = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

String IdThing = "";
String authToken = "";
boolean settingMode = false;

const char *apSSID = "SBUT_";
const char *apPassword = "sbut12345";

unsigned long settingTimeout = 0;
unsigned long valSync = 0;
unsigned long valNotify = 0;

int appMode = NORMAL_MODE;
int tempSync = 0;

String ver = ""; 



SButApiClass::SButApiClass()
{
}
SButApiClass::~SButApiClass()
{
}

String SButApiClass::thingId()
{
    String thingIdEEPROM = SButEEPROM.read(EEPROM_SBUT_ID_START, EEPROM_SBUT_ID_END);
    String a = "";
    if (thingIdEEPROM.length() > 0)
    {
        return thingIdEEPROM;
    }
    else
    {
        for (int n = 0; n < 12; n++)
        {
            a = a + alphabeth[random(0, 62)];
        }
        SButEEPROM.write(a, EEPROM_SBUT_ID_START, EEPROM_SBUT_ID_END);
        SButEEPROM.commit();
        return a;
    }
}

void SButApiClass::begin(const char *ssid, const char *pass, const char *token, int nodeCounts, int sensorCounts)
{

    if (ssid == NULL || pass == NULL || token == NULL)
    {
        ECHOLN("Can't begin'");
        return;
    }
    if (nodeCounts == 0 && sensorCounts == 0)
    {
        ECHOLN(" NODE AND SENSOR NULL ");
    }

    WiFi.begin(ssid, pass);
    String tokenUser = String(token);
    authToken = tokenUser;
    IdThing = thingId();
    if (checkConnection())
    {
        SButHTTP.httpCreateThing(tokenUser, IdThing, WiFi.localIP().toString(), SBUT_INFO_DEVICE, "SDEV_V1");
        SButHTTP.httpCreateNodeAndSensor(IdThing, nodeCounts, sensorCounts);

        SButMQTT.beginMQTT(IdThing);
    }
    else
    {
        ECHOLN("Ket Noi Mang That Bai!");
        return;
    }
}
void SButApiClass::beginSmartConfig(const int nodeCounts, const int sensorCounts)
{
    if (nodeCounts == 0 && sensorCounts == 0)
    {
        ECHOLN(" NODE AND SENSOR NULL ");
    }
    IdThing = thingId();
    if (restoreSmartConfig())
    {
        if (checkConnection())
        {
            settingMode = false;
            String token = SButEEPROM.read(EEPROM_SBUT_TOKEN_START, EEPROM_SBUT_TOKEN_END);
            authToken = token;
            if (token.length() > 0)
            {
                SButHTTP.httpCreateThing(token, IdThing, WiFi.localIP().toString(), SBUT_INFO_DEVICE, "SDEV_V1");
                SButHTTP.httpCreateNodeAndSensor(IdThing, nodeCounts, sensorCounts);
                SButMQTT.beginMQTT(IdThing);
            }
            else
            {
                ECHOLN("Token không tồn tại!");
            }
            return;
        }
        else
        {
            ECHOLN("Ket Noi Mang That Bai!");
        }
    }
    settingMode = true;
    createSmartConfig();
}
bool SButApiClass::restoreSmartConfig()
{
    String ssid = SButEEPROM.read(EEPROM_WIFI_SSID_START, EEPROM_WIFI_SSID_END);
    String pass = SButEEPROM.read(EEPROM_WIFI_PASS_START, EEPROM_WIFI_PASS_END);
    int ssid_len = ssid.length() + 1;
    char char_ssid[ssid_len];
    ssid.toCharArray(char_ssid, ssid_len);
    int pass_len = pass.length() + 1;
    char char_pass[pass_len];
    pass.toCharArray(char_pass, pass_len);
    if (ssid.length() > 0 && pass.length() > 0)
    {
        WiFi.begin(char_ssid, char_pass);
        ECHOLN(char_ssid);
        ECHOLN(char_pass);
        return true;
    }
    return false;
}
void SButApiClass::createSmartConfig()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(apSSID + IdThing.substring(0, 5), apPassword);
    dnsServer.start(53, "*", apIP);
    Serial.print("Starting Access Point at \"");
    Serial.print(apSSID + IdThing.substring(0, 5));
    Serial.println("\"");
    startWebServer();
}
void SButApiClass::startWebServer()
{
    if (settingMode)
    {
        Serial.print("Starting Web Server at ");
        Serial.println(WiFi.softAPIP());
        webServer.on("/setap", []() {
            String ssid = webServer.arg("ssid");
            String passwifi = webServer.arg("passwifi");
            String token = webServer.arg("token");

            SButEEPROM.write(ssid, EEPROM_WIFI_SSID_START, EEPROM_WIFI_SSID_END);
            SButEEPROM.write(passwifi, EEPROM_WIFI_PASS_START, EEPROM_WIFI_PASS_END);
            SButEEPROM.write(token, EEPROM_SBUT_TOKEN_START, EEPROM_SBUT_TOKEN_END);
            SButEEPROM.commit();
            String message = "{\"status\": 1}";
            webServer.send(200, "application/json", message);
            delay(3000);
            ESP.restart();
        });
      
        webServer.onNotFound([]() {
            String s = "<h1>SBUT Not Found";
            webServer.send(200, "text/html", s);
        });
    }
    else
    {
        webServer.on("/reset", []() {
            String s = "<h1>Wi-Fi settings was reset.</h1><p>Please reset device.</p>";
            // webServer.send(200, "text/html", makePage("Reset Wi-Fi Settings", s));
            ESP.restart();
        });
    }
    webServer.begin();
}
void SButApiClass::loop()
{
    webServer.handleClient();
    if (WiFi.status() != WL_CONNECTED)
    {
        return;
    }
    if (settingMode)
    {
        dnsServer.processNextRequest();
    }
    else
    {
        SButMQTT.loopMQTT();
        syncNode();
        checkButtonResetClick();

        String topic = SButMQTT.getTopic();
        String uri = "v1/sdk/esp/";
        uri += IdThing;
        uri += "/OTA";

        if (topic == uri.c_str())
        {
            String msg = SButMQTT.getMessage();
            SButHTTP.updateOTA(msg, ver);
        }
    }
}
bool SButApiClass::checkConnection()
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

void SButApiClass::setSensor(int id, int value)
{
    String sId = String(id);
    String v = String(value);
    String uri = "v1/sdk/esp/setSensor/";
    uri += IdThing;
    uri += "/";
    uri += sId;
    uri += "/update";
    SButMQTT.publish(uri, v);
}

int SButApiClass::getNode(int id)
{
    String topic = SButMQTT.getTopic();
    String msg = SButMQTT.getMessage();
    String uri = "v1/sdk/esp/getNode/";
    uri += IdThing;
    uri += "/";
    uri += id;

    if (topic == uri.c_str())
    {
        return msg.toInt();
    }
    else
    {
        return -1;
    }
}

void SButApiClass::setNode(int id, int value)
{
    String n = String(id);
    String v = String(value);
    String uri = "v1/sdk/esp/setNode/";
    uri += IdThing;
    uri += "/";
    uri += n;
    uri += "/update";
    SButMQTT.publish(uri.c_str(), v.c_str());
}

bool SButApiClass::isConnected()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        return false;
    }
    return true;
}

void SButApiClass::setVersion(const String version)
{
    if (version.length() > 0)
    {
        ver = version;
        SButHTTP.updateThingVersion(IdThing, version);
    }
}

void SButApiClass::syncNode()
{
    if (tempSync == 0)
    {
        if (((unsigned long)(millis() - valSync)) > 2000)
        {
            String s = "v1/sdk/esp/nodeOnl/";
            s += IdThing;
            s += "/sync";
            SButMQTT.publish(s.c_str(), "ok");
            tempSync = 1;
            valSync = millis();
        }
    }
}
void SButApiClass::notify(const String &message)
{
    if (((unsigned long)(millis() - valNotify)) > 1000)
    {
        String uri = "v1/sdk/esp/notify/";
        uri += authToken;

        String mess = IdThing;
        mess += "/";
        mess += message;

        SButMQTT.publish(uri.c_str(), mess.c_str());

        valNotify = millis();
    }
}
void SButApiClass::checkButtonResetClick()
{
    if (digitalRead(0) == LOW && (settingTimeout + SETTING_HOLD_TIME) <= millis() && appMode != SERVER_MODE)
    {
        settingTimeout = millis();
        if (appMode != SERVER_MODE)
        {
            appMode = SERVER_MODE;
            ECHOLN("[WifiService][SBUT RESET] SBUT RESET....");
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
            SButEEPROM.clear(20, 512);
            SButEEPROM.end();
            delay(3000);
            ESP.restart();
        }
    }
    else if (digitalRead(0) == HIGH)
    {
        settingTimeout = millis();
    }
}