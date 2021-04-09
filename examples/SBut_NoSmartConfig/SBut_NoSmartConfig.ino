#include <SBut.h>

SButSdk nSbut;
int nodes = 1; // khai báo ta có 1 luồng node
int sensors = 0;

int VIRTUAL_PIN;

// Thông tin đăng nhập WiFi của bạn.
// Đặt mật khẩu thành "" cho các mạng mở.
const char *ssid = "Van Thong"; //dùng cho phần config thủ công
const char *pass = "123456789"; //dùng cho phần config thủ công

// Bạn sẽ nhận được Auth Token trong Ứng dụng SBut Platform.
// Vào phần Menu (Auth Token)
const char *token = "JiHw2L5hTvmEFLB1GdONNAvKbPApbzVxPV7lXvu8hWTvlTiJv5"; //dùng cho phần config thủ công

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    nSbut.begin(nodes, sensors, false, ssid, pass, token); //dùng cho phần config thủ công
    //   nSbut.begin(nodes, sensors, true);   // nếu bạn dùng Smart Config thì để giá trị thành true
}

void loop()
{
    nSbut.loop();
    int n = nSbut.getNode(0); // lấy giá trị luồng node số 1

    if (n == 1)
    {
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
        digitalWrite(LED_BUILTIN, LOW);
    }
}