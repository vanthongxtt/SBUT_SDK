#include <SBut.h> // khai bao thu vien sbut

#define relay 4 // dinh nghia chan relay la chan so 4

SButSDK nSbut;  // dat ten cho thu vien sbut

const char *ssid = "YourNetworkName"; // ten wifi nha ban
const char *pass = "YourNetworkPassword"; // mat khau wifi nha ban

// Bạn sẽ nhận được Auth Token trong Ứng dụng SBut Platform.
// Vào phần Menu (Auth Token)
const char *token = "YourAuthToken"; // Auth Token

const int nodes = 1;   // khai bao co 1 luon node
const int sensors = 0; // khai bao luon cam bien la 0

int VIRTUAL_PIN;

void setup()
{
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
  pinMode(5, INPUT_PULLUP);
  nSbut.begin(ssid, pass, token, nodes, sensors); // bat dau goi thu vien
}

void loop()
{
  nSbut.loop();             // vong lap cho sbut
  int n = nSbut.getNode(0); // lay gia tri node cua luon so 1
  if (digitalRead(5) == 0)
  {
    delay(100);
    if (digitalRead(5) != 0)
    {
      VIRTUAL_PIN = !VIRTUAL_PIN;
      nSbut.setNode(0, VIRTUAL_PIN); // gui gia tri nhut nhan len luon node so 1
    }
  }

  if (n == 1)
  {
    digitalWrite(relay, HIGH);
  }
  else
  {
    digitalWrite(relay, LOW);
  }
}
