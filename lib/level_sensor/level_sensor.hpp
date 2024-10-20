#ifndef __LEVEL_SENSOR_H__
#define __LEVEL_SENSOR_H__

#include "protocol.hpp"
#include "rs485.hpp"
#include "common_header.hpp"

namespace sensor
{
    class LevelSensor
    {
    public:
        LevelSensor(Protocol::ProtocolInterface& protocolHandler, rs485Comms::RS485Interface& rs485Handler);
         common::error_type_t readRegister(uint16_t regAddress, uint16_t& data);
        common::error_type_t readPressure(float& pressure, unsigned long baudRate);
        
    private:
        Protocol::ProtocolInterface& protocol;
        rs485Comms::RS485Interface& rs485;
        //common::error_type_t readRegister(uint16_t regAddress, uint16_t& data);
    };
}

#endif
