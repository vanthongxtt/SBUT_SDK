#include "Sbut/SButHTTP.h"

WiFiClient clientHttp;

String tokenUser = "";

SButHTTPClass::SButHTTPClass()
{
}
void SButHTTPClass::httpCreateThing(const String token, const String thingId, const String Ip, const String board, const String version)
{
    uint32_t getSketchSize = ESP.getSketchSize();
    uint32_t getFreeSketchSpace = ESP.getFreeSketchSpace();
    String getSketchMD5 = ESP.getSketchMD5();

    String response;
    tokenUser = String(token);
    String httpRequestData = "thingId=" + thingId + "&ipAddress=" + Ip + "&board=" + board + "&version=" + version + "&sketchHash=" + getSketchMD5 + "&sketchSize=" + String(getSketchSize) + "&freeSketchSpace=" + String(getFreeSketchSpace);
    if (httpRequest(String("POST /api/v1/thing/") + token + "/addThing", httpRequestData, response))
    {
        if (response.length() != 0)
        {
            Serial.print("WARNING: ");
            Serial.println(response);
        }
    }
}
void SButHTTPClass::httpUpdateWidgetInforThing(const String token, const String thingId, const String Ip, const String board){
    uint32_t getSketchSize = ESP.getSketchSize();
    uint32_t getFreeSketchSpace = ESP.getFreeSketchSpace();
    String getSketchMD5 = ESP.getSketchMD5();

    String response;
    tokenUser = String(token);
    String httpRequestData = "&ipAddress=" + Ip + "&board=" + board +  "&sketchHash=" + getSketchMD5 + "&sketchSize=" + String(getSketchSize) + "&freeSketchSpace=" + String(getFreeSketchSpace);
    if (httpRequest(String("POST /api/v1/thing/") + token + "/" + thingId + "/updateWidgetThingCloud", httpRequestData, response))
    {
        if (response.length() != 0)
        {
            Serial.print("WARNING: ");
            Serial.println(response);
        }
    }
}
void SButHTTPClass::httpCreateNodeAndSensor(const String thingId, const int nodes, const int sensors)
{
    String response;
    String httpRequestData = "nodeCount=" + String(nodes) + "&sensorCount=" + String(sensors);
    if (httpRequest(String("POST /api/v1/thing/") + tokenUser + "/" + thingId + "/add", httpRequestData, response))
    {
        if (response.length() != 0)
        {
            Serial.print("WARNING: ");
            Serial.println(response);
        }
    }
}
void SButHTTPClass::updateThingVersion(const String thingId, const String version)
{
    String response;
    String httpRequestData = "version=" + String(version);
    if (httpRequest(String("POST /api/v1/thing/") + tokenUser + "/" + thingId + "/updateVersion", httpRequestData, response))
    {
        if (response.length() != 0)
        {
            Serial.print("WARNING: ");
            Serial.println(response);
        }
    }
}
void SButHTTPClass::updateOTA(const String serverUrl, const String version)
{

    int serverUrlSize = serverUrl.length() + 1;
    char _serverUrl[serverUrlSize];
    serverUrl.toCharArray(_serverUrl, serverUrlSize);

    int versionSize = version.length() + 1;
    char _version[versionSize];
    version.toCharArray(_version, versionSize);

    t_httpUpdate_return ret = ESPhttpUpdate.update(serverUrl, version);
    Serial.println(String(ret));
    switch (ret)
    {
    case HTTP_UPDATE_FAILED:
        Serial.printf("SBUT_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;
    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("SBUT_UPDATE_NO_UPDATES");
        break;
    case HTTP_UPDATE_OK:
        Serial.println("SBUT_UPDATE_OK");
        break;
    }
}
int SButHTTPClass::getNodeCloud(const String thingId)
{
    String response;
    String httpRequestData = "";
    if (httpRequest(String("GET /api/v1/thing/") + tokenUser + "/" + thingId + "/asynNode", httpRequestData, response))
    {
        if (response.length() != 0)
        {
            DynamicJsonDocument doc(2000);
            DeserializationError error = deserializeJson(doc, response);
            if (error)
            {
                return 0;
            }
            String dt = doc["data"];
            return dt.toInt();
        }
    }
    return -1;
}
bool SButHTTPClass::httpRequest(const String &method, const String &request, String &response)
{
    if (clientHttp.connect(SBUT_DEFAULT_DOMAIN_CLOUD, 3000))
    {
        // Serial.println("OK");
    }
    else
    {
        Serial.println("failed");
        return false;
    }

    clientHttp.print(method);
    clientHttp.println(F(" HTTP/1.1"));
    clientHttp.print(F("Host: "));
    clientHttp.println(SBUT_DEFAULT_DOMAIN_CLOUD);
    clientHttp.println(F("Connection: close"));
    if (request.length())
    {
        clientHttp.println(F("Content-Type: application/x-www-form-urlencoded"));
        clientHttp.print(F("Content-Length: "));
        clientHttp.println(request.length());
        clientHttp.println();
        clientHttp.print(request);
    }
    else
    {
        clientHttp.println();
    }

    int timeout = millis() + 5000;
    while (clientHttp.available() == 0)
    {
        if (timeout - millis() < 0)
        {
            Serial.println(">>> Client Timeout !");
            clientHttp.stop();
            return false;
        }
    }

    int contentLength = -1;
    while (clientHttp.available())
    {
        String line = clientHttp.readStringUntil('\n');
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
    while (response.length() < contentLength && clientHttp.connected())
    {
        while (clientHttp.available())
        {
            char c = clientHttp.read();
            response += c;
        }
    }
    clientHttp.stop();
    return true;
}
SButHTTPClass SButHTTP;