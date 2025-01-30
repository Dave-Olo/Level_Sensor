#ifndef __LEVEL_SENSOR_H__
#define __LEVEL_SENSOR_H__

#include "protocol.hpp"
#include "rs485.hpp"
#include "common_header.hpp"

namespace Sensor
{
    class LevelSensorInterface{
     public:
       
        virtual common::error_type_t init() = 0;
        virtual common::error_type_t getHeight(float& height) = 0;
        virtual ~LevelSensorInterface() = default;

    };


    class GLT500Level : public LevelSensorInterface {
    public:
        GLT500Level(Protocol::ProtocolInterface& protocolHandler, RS485Comms::CommInterface& rs485Handler);
        ~GLT500Level() override; 

        common::error_type_t init() override;
        common::error_type_t getHeight(float& height) override;
        //common::error_type_t test();

    private:
        common::error_type_t readPressure(float& pressure);
        Protocol::ProtocolInterface& protocol;
        RS485Comms::CommInterface& rs485;
    };
}

#endif
