#ifndef __RS485_H__
#define __RS485_H__

#include <Arduino.h>
#include <cstdint>
#include "common_header.hpp"
#include <HardwareSerial.h>

namespace rs485Comms
{

    class RS485Interface
    {
    public:
        virtual common::error_type_t init(unsigned long baudRate) = 0;
        virtual common::error_type_t sendBytes(const uint8_t *data, size_t length) = 0;
        virtual common::error_type_t receiveBytes(uint8_t *buffer, size_t length, size_t &bytesRead) = 0;
        virtual common::error_type_t deinit() = 0;
        virtual ~RS485Interface() = default;
    };

    
    class rs485Setup : public RS485Interface
    {
    public:
        rs485Setup(HardwareSerial &serial, int dirPin);
        common::error_type_t init(unsigned long baudRate) override;
        common::error_type_t sendBytes(const uint8_t *data, size_t length) override;
        common::error_type_t receiveBytes(uint8_t *buffer, size_t length, size_t &bytesRead) override;
        common::error_type_t deinit() override;

    protected:
        HardwareSerial &serial;
        int dirPin;
        bool isInitialized = false;
        void enableTransmit();
        void enableReceive();
    };
}

#endif
