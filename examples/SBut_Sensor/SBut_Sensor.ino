#include <SBut.h> // goi thu vien SBut SDK
#include "DHTesp.h" // goi thu vien DHT

SButSdk nSbut; // khai bao thu vien SBut
DHTesp dht; // khai bao thu vien DHT

#define relay 4 // dinh nghia chan relay la chan so 4

int nodes = 2; // khai bao sbut co 2 luong node
int sensors = 2; // khai bao sbut co 2 luong cam bien

unsigned long val = 0; 

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relay, OUTPUT);
  nSbut.begin(nodes, sensors, true); // bat dau setup thu vien sbut
  dht.setup(5, DHTesp::DHT11);
}

void loop() {
  nSbut.loop(); // goi vong lap sbut
  int n = nSbut.getNode(1); // goi luong node thu 2
  int n2 = nSbut.getNode(0); // goi luong node thu 1

  if (((unsigned long)(millis() - val)) > 10000) {
    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    nSbut.setSensor(0, humidity); // gui do am len luong cam bien thu 1
    nSbut.setSensor(1, temperature); // gui nhiet do len luong cam bien thu 2
    //      nSbut.writeNotification("Thong Bao Ne hihi");
    val = millis();
  }
  if (n == 1) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (n2 == 1) {
    digitalWrite(relay, HIGH);
  } else {
    digitalWrite(relay, LOW);
  }
}