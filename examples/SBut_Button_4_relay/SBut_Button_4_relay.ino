#include <SBut.h> // khai bao thu vien sbut

#define relay1 4
#define relay2 14
#define relay3 13
#define relay4 12
SButSdk nSbut;   // dat ten cho thu vien sbut
int nodes = 4;   // khai bao co 1 luong node
int sensors = 0; // khai bao luong cam bien la 0

int VIRTUAL_PIN;
void setup()
{
    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);
    pinMode(relay3, OUTPUT);
    pinMode(relay4, OUTPUT);
    nSbut.begin(nodes, sensors, true); // bat dau goi thu vien
}

void loop()
{
    nSbut.loop();              // vong lap cho sbut
    int n1 = nSbut.getNode(0); // lay gia tri node cua luong so 1
    int n2 = nSbut.getNode(1); // lay gia tri node cua luong so 2
    int n3 = nSbut.getNode(2); // lay gia tri node cua luong so 3
    int n4 = nSbut.getNode(3); // lay gia tri node cua luong so 4

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
