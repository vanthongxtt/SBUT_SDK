#include <SBut.h> // khai bao thu vien sbut

SButSDK nSbut; // dat ten cho thu vien sbut

#define relay1 4
#define relay2 14
#define relay3 13
#define relay4 12

const char *ssid = "YourNetworkName"; // ten wifi nha ban
const char *pass = "YourNetworkPassword"; // mat khau wifi nha ban

// Bạn sẽ nhận được Auth Token trong Ứng dụng SBut Platform.
// Vào phần Menu (Auth Token)
const char *token = "YourAuthToken"; // Auth Token

const int nodes = 4;   // khai bao co 4 luon node
const int sensors = 0; // khai bao luon cam bien la 0

void setup()
{
    Serial.begin(115200);
    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);
    pinMode(relay3, OUTPUT);
    pinMode(relay4, OUTPUT);
    nSbut.begin(ssid, pass, token, nodes, sensors); // bat dau goi thu vien
}

void loop()
{
    nSbut.loop();              // vong lap cho sbut
    int n1 = nSbut.getNode(0); // lay gia tri node cua luon so 1
    int n2 = nSbut.getNode(1); // lay gia tri node cua luon so 2
    int n3 = nSbut.getNode(2); // lay gia tri node cua luon so 3
    int n4 = nSbut.getNode(3); // lay gia tri node cua luon so 4

    // luu y phai dung if else if moi co the dieu khien duoc thiet bi chinh xac nhat
    if (n1 == 1)
    {
        digitalWrite(relay1, HIGH);
    }
    else if (n1 == 0)
    {
        digitalWrite(relay1, LOW);
    }
    else if (n2 == 1)
    {
        digitalWrite(relay2, HIGH);
    }
    else if (n2 == 0)
    {
        digitalWrite(relay2, LOW);
    }
    else if (n3 == 1)
    {
        digitalWrite(relay3, HIGH);
    }
    else if (n3 == 0)
    {
        digitalWrite(relay3, LOW);
    }
    else if (n4 == 1)
    {
        digitalWrite(relay4, HIGH);
    }
    else if (n4 == 0)
    {
        digitalWrite(relay4, LOW);
    }
    //
}
