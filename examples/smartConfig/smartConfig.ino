#include <SBut.h>

SButSDK nSbut;

SButTimer timer;

const char *ssid = "YourNetworkName";     // ten wifi nha ban
const char *pass = "YourNetworkPassword"; // mat khau wifi nha ban

// Bạn sẽ nhận được Auth Token trong Ứng dụng SBut Platform.
// Vào phần Menu (Auth Token)
const char *token = "YourAuthToken"; // Auth Token

const int nodes = 0;   // khai bao co 0 luon node
const int sensors = 2; // khai bao luon cam bien la 2

void setup()
{
    Serial.begin(115200);
    nSbut.beginSmartConfig(nodes, sensors); // bat dau goi thu vien
    timer.setInterval(setValueServer, 5000); // moi 5s gui data len cloud 1 lan
}
void setValueServer()
{
    nSbut.setSensor(0, random(100)); // gui du lieu random len cloud
    nSbut.setSensor(1, random(100)); // gui du lieu random len cloud
}

void loop()
{
    nSbut.loop();
    timer.loop();
}
