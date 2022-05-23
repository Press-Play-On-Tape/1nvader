#pragma once

#include "Arduboy2Ext.h"

class EEPROM_Utils {

    public: 

        EEPROM_Utils(){};
            
        static void initEEPROM(bool force);
        static uint16_t getScore(GameMode mode);
        static void saveScore(GameMode mode, uint16_t score);
        static GameMode getMode();
        static void saveMode(GameMode mode);
        static GameRotation getRotation();
        static void saveRotation(GameRotation rotation);
        static uint8_t getLevel(uint8_t level);
        static void saveLevel(uint8_t level, uint8_t value);
        static int16_t checkSum(bool update);

};


/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised?
 *
 *   Looks for the characters '1' and 'V' in the first two bytes of the EEPROM
 *   memory range starting from byte EEPROM_START.  
 */

const uint8_t letter1 = 'O'; 
const uint8_t letter2 = 'V'; 

void EEPROM_Utils::initEEPROM(bool force) {

    byte c1 = EEPROM.read(Constants::EEPROM_Start_C1);
    byte c2 = EEPROM.read(Constants::EEPROM_Start_C2);

    if (c1 != letter1 || c2 != letter2 || force) { 

        uint16_t hs = 0;
        uint8_t mode = static_cast<uint8_t>(GameMode::Single);
        uint8_t rotation = static_cast<uint8_t>(GameRotation::Portrait);

        EEPROM.put(Constants::EEPROM_Start_C1, letter1);
        EEPROM.put(Constants::EEPROM_Start_C2, letter2);
        EEPROM.put(Constants::EEPROM_Single, hs);
        EEPROM.put(Constants::EEPROM_Double, hs);
        EEPROM.put(Constants::EEPROM_Tug_Of_War, hs);
        EEPROM.put(Constants::EEPROM_Mode, mode);
        EEPROM.put(Constants::EEPROM_Rotation, rotation);
        EEPROM.put(Constants::EEPROM_Lock_LVL_0, 0);
        EEPROM.put(Constants::EEPROM_Lock_LVL_1, 0);

        EEPROM_Utils::checkSum(true);

    }
    else {

        int16_t checkSumOld = 0;
        int16_t checkSumNow = EEPROM_Utils::checkSum(false);
        EEPROM.get(Constants::EEPROM_Checksum, checkSumOld);

        if (checkSumNow != checkSumOld) {

            EEPROM_Utils::initEEPROM(true);

        }
        
    }    

}

/* -----------------------------------------------------------------------------
 *   Get score. 
 */
uint16_t EEPROM_Utils::getScore(GameMode mode) {

    uint16_t score = 0;
    EEPROM.get(Constants::EEPROM_Single + (static_cast<uint8_t>(mode) * 2), score);
    return score;

}


/* -----------------------------------------------------------------------------
 *   Save score ..
 */
void EEPROM_Utils::saveScore(GameMode mode, uint16_t score) {

    uint16_t oldScore = 0;
    EEPROM.get(Constants::EEPROM_Single + (static_cast<uint8_t>(mode) * 2), oldScore);

    if (oldScore < score) {

        EEPROM.update(Constants::EEPROM_Single + (static_cast<uint8_t>(mode) * 2), score);

    }

    EEPROM_Utils::checkSum(true);

}


/* -----------------------------------------------------------------------------
 *   Get mode. 
 */
GameMode EEPROM_Utils::getMode() {

    uint8_t mode = 0;
    EEPROM.get(Constants::EEPROM_Mode, mode);
    return static_cast<GameMode>(mode);

}


/* -----------------------------------------------------------------------------
 *   Save mode ..
 */
void EEPROM_Utils::saveMode(GameMode mode) {

    EEPROM.update(Constants::EEPROM_Mode, static_cast<uint8_t>(mode));
    EEPROM_Utils::checkSum(true);

}


/* -----------------------------------------------------------------------------
 *   Get rotation. 
 */
GameRotation EEPROM_Utils::getRotation() {

    uint8_t rotation = 0;
    EEPROM.get(Constants::EEPROM_Rotation, rotation);
    return static_cast<GameRotation>(rotation);

}


/* -----------------------------------------------------------------------------
 *   Save rotation ..
 */
void EEPROM_Utils::saveRotation(GameRotation rotation) {

    EEPROM.update(Constants::EEPROM_Rotation, static_cast<uint8_t>(rotation));
    EEPROM_Utils::checkSum(true);

}

/* -----------------------------------------------------------------------------
 *   Get level. 
 */
uint8_t EEPROM_Utils::getLevel(uint8_t level) {

    uint8_t rotation = 0;
    EEPROM.get(Constants::EEPROM_Lock_LVL_0 + level, rotation);
    return rotation;

}


/* -----------------------------------------------------------------------------
 *   Save level ..
 */
void EEPROM_Utils::saveLevel(uint8_t level, uint8_t value) {

    EEPROM.update(Constants::EEPROM_Lock_LVL_0 + level, value);
    EEPROM_Utils::checkSum(true);

}


/* -----------------------------------------------------------------------------
 *   Generate and optionally save a check sum .. 
 */
int16_t EEPROM_Utils::checkSum(bool update) {

    int16_t checksum = 0;

    for (uint8_t i = 0; i < (Constants::EEPROM_End - Constants::EEPROM_Start); i++) {

        checksum = checksum + ((i % 2 == 0 ? 1 : -1) * eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::EEPROM_Start + i)));

    }

    if (update) {
        EEPROM.put(Constants::EEPROM_Checksum, static_cast<uint16_t>(checksum));
    }

    return checksum;

}