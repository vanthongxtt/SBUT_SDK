#include <SBut.h> // khai bao thu vien sbut

#define relay 4 // dinh nghia chan relay la chan so 4
SButSdk nSbut; // dat ten cho thu vien sbut
int nodes = 1; // khai bao co 1 luong node
int sensors = 0; // khai bao luong cam bien la 0

int VIRTUAL_PIN;
void setup() {
  pinMode(relay, OUTPUT);
  pinMode(5, INPUT_PULLUP);
  nSbut.begin(nodes, sensors, true); // bat dau goi thu vien
}

void loop() {
  nSbut.loop(); // vong lap cho sbut
  int n = nSbut.getNode(0); // lay gia tri node cua luong so 1
  if (digitalRead(5) == 0)
  {
    delay(100);
    if (digitalRead(5) != 0)
    {
      VIRTUAL_PIN = !VIRTUAL_PIN;
      nSbut.setNode(0,VIRTUAL_PIN); // gui gia tri nhut nhan len luong node so 1
    }
  }

  if (n == 1) {
    digitalWrite(relay, HIGH);
  } else {
    digitalWrite(relay, LOW);
  }

}
