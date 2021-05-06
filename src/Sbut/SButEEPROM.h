#ifndef SBUT_EEPROM_H
#define SBUT_EEPROM_H

#include <Arduino.h>

#include <EEPROM.h>

#include "SButDebug.h"

#define EEPROM_SIZE 512

class SButEEPROMClass
{
public:
    SButEEPROMClass();
    ~SButEEPROMClass();
    void write(const String &data, int begin, int endMax);
    String read(int begin, int end);
    void clear(int start, int end);
    void commit();
    void end();
};

extern SButEEPROMClass SButEEPROM;

#endif 
