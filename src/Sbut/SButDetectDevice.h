#ifndef SButDetectDevice_h
#define SButDetectDevice_h

    #ifndef SBUT_INFO_DEVICE

            #if defined(ARDUINO_ESP8266_NODEMCU)
            #define SBUT_INFO_DEVICE  "NodeMCU"
            #elif defined(ARDUINO_ARCH_ESP8266)
            #define SBUT_INFO_DEVICE  "ESP8266"

            /* ESP32 */
            #elif defined(ARDUINO_ARCH_ESP32)
            #define SBUT_INFO_DEVICE  "ESP32"
            #endif
    #endif

#endif