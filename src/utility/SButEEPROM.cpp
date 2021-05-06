#include "Sbut/SButEEPROM.h"

/**
 * constructor
 */
SButEEPROMClass::SButEEPROMClass()
{
    EEPROM.begin(EEPROM_SIZE);
}

/**
 * destructor
 */
SButEEPROMClass::~SButEEPROMClass()
{
}

void SButEEPROMClass::write(const String &data, int begin, int endMax)
{
    int end = data.length() + begin;
    if (end - 1 > endMax) {
        ECHOLN("[EEPRomService][write] Size too large");
        return;
    }
    clear(begin, endMax);
    ECHO("[EEPRomService][write] Wrote EEPROM: ");
    for (int i = begin; i < end; i++) {
        EEPROM.write(i, data[i - begin]);
        // ECHO("[EEPRomService][write] Wrote EEPROM: ");
        // ECHOLN(data[i - begin]);
    }
}

String SButEEPROMClass::read(int begin, int end)
{
    ECHO("[EEPRomService][read] Read eeprom: ");
    ECHO(begin);
    ECHO(" ");
    ECHOLN(end);
    String data;
    char c;
    for (int i = begin; i <= end; ++i) {
        c = (char) EEPROM.read(i);
        if (c != 0) {
            data += c;
        }
    }

    return data;
}

void SButEEPROMClass::clear(int start, int end)
{
    ECHO("[EEPRomService][clear] Clearing eeprom: ");
    ECHO(start);
    ECHO(" ");
    ECHOLN(end);
    for (int i = start; i < end; i++) {
        EEPROM.write(i, 0);
    }
}

void SButEEPROMClass::commit()
{
    ECHOLN("[EEPRomService][commit] Commit eeprom");
    EEPROM.commit();
}

void SButEEPROMClass::end(){
    ECHOLN("[EEPRomService][end]");
    EEPROM.end();
}

SButEEPROMClass SButEEPROM;
