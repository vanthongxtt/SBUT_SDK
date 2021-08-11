# SBUT PLATFORM
- SBut Platform là nền tảng IoT giúp kết nối các thiết bị của bạn lên Internet. Bạn có thể kiểm soát thiết bị dễ dàng bằng cách sử dụng Dashboard hoặc thông qua API dành cho nhà phát triển.
- Download: 
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
#include <SBut.h> // Gọi thư viện sbut

SButSDK nSbut; // đặt tên cho thư viện Sbut

const int nodes = 0; // khai báo luồn node
const int sensors = 0; // khai báo luồn sensor

void setup() {
  Serial.begin(115200);
  nSbut.beginSmartConfig(nodes, sensors); // bắt đầu gọi thư viện smart config
}

void loop() {
  nSbut.loop(); // vòng lập cho sbut
}
```
- Config Thủ Công

```cpp
#include <SBut.h>

SButSDK nSbut;


const int nodes = 0; // khai báo luồn node
const int sensors = 0; //khai báo luồn sensor


// Thông tin đăng nhập WiFi của bạn.
// Đặt mật khẩu thành "" cho các mạng mở.
const char *ssid = "YourNetworkName"; // Tên wifi nhà bạn
const char *pass = "YourNetworkPassword"; // Mật khẩu wifi nhà bạn

// Bạn sẽ nhận được Auth Token trong Ứng dụng SBut Platform.
// Vào phần Menu (Auth Token)
const char *token = "YourAuthToken"; // Auth Token

void setup()
{
    Serial.begin(115200);
    nSbut.begin(ssid, pass, token, nodes, sensors); //dùng cho phần config thủ công
}

void loop()
{
    nSbut.loop();
}
```


- Config với sbut widget cloud

```cpp
#include <SBut.h> // Gọi thư viện sbut
SButSDK nSbut; // đặt tên cho thư viện Sbut
  
// Thông tin đăng nhập WiFi của bạn.
// Đặt mật khẩu thành "" cho các mạng mở.

const char *ssid = "YourNetworkName"; // Tên wifi nhà bạn
const char *pass = "YourNetworkPassword"; // Mật khẩu wifi nhà bạn

// Bạn sẽ nhận được Auth Token trong Ứng dụng SBut Platform.
// Vào phần Menu (Auth Token)

const char *token = "YourAuthToken"; // Auth Token
const char *thingId = "YourThingId"; // Mã thiết bị của bạn

void setup()
{
    Serial.begin(115200);
    nSbut.beginWidgets(ssid, pass, token, thingId); //dùng cho phần sbut widget
}
 
void loop() {
    nSbut.loop(); // vòng lập cho sbut      
}
```

