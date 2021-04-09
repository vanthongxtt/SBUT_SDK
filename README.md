# SBUT PLATFORM
- Nền tảng kết nối các thiết bị của bạn với đám mây, thiết kế, phân tích dữ liệu từ xa và quản  các sản phẩm được triển khai của bạn theo yêu cầu.
- Dowload: 
 [<img src="https://svgsilh.com/svg/1873373.svg" width="18" height="18" /> Website](https://sbut.vn) | 
 [<img src="https://cdn.rawgit.com/simple-icons/simple-icons/develop/icons/googleplay.svg" width="18" height="18" /> Google Play](https://play.google.com/store/apps/details?id=com.sbut.sbutapp) 

### ỨNG DỤNG
- Điều khiển, giám sát các thiết bị đầu vào/ đầu ra từ xa qua Internet.
- Dùng trong nghiên cứu IoT để ra ứng dụng nhanh chóng mà không cần biết quá nhiều về lĩnh vực IoT.
- Làm bộ báo động (cháy, nhiệt,... bất cứ dòng cảm biến nào mà Arduino hỗ trợ).
- Làm SCADA.

### CÁCH SỬ DỤNG
- Smart Config
```cpp
#include <SBut.h> // khai bao thu vien sbut

SButSdk nSbut; // dat ten cho thu vien sbut
int nodes = 0; // khai bao luong node
int sensors = 0; // khai bao luong cam bien la 0

int VIRTUAL_PIN;
void setup() {
  nSbut.begin(nodes, sensors, true); // bat dau goi thu vien
}

void loop() {
  nSbut.loop(); // vong lap cho sbut
}
```
- Config Thủ Công

```cpp
#include <SBut.h>

SButSdk nSbut;
int nodes = 0; // khai báo  luồng node
int sensors = 0;


// Thông tin đăng nhập WiFi của bạn.
// Đặt mật khẩu thành "" cho các mạng mở.
const char *ssid = "Van Thong"; //dùng cho phần config thủ công
const char *pass = "123456789"; //dùng cho phần config thủ công

// Bạn sẽ nhận được Auth Token trong Ứng dụng SBut Platform.
// Vào phần Menu (Auth Token)
const char *token = "JiHw2L5hTvmEFLB1GdONNAvKbPApbzVxPV7lXvu8hWTvlTiJv5"; //dùng cho phần config thủ công

void setup()
{
    nSbut.begin(nodes, sensors, false, ssid, pass, token); //dùng cho phần config thủ công
}

void loop()
{
    nSbut.loop();
}
```


