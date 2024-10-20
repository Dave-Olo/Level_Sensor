#ifndef CRC_HPP
#define CRC_HPP

#include <Arduino.h>
#include <cstdint>

class CRC16 {
public:
    
    static uint16_t CalculateCRC(const uint8_t *data, uint16_t length);
private:
    static const uint16_t wCRCTable[256]; 
};

#endif 
