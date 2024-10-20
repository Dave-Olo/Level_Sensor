#include <Arduino.h>
#include "rs485.hpp"
#include "common_header.hpp"

namespace rs485Comms
{
    rs485Setup::rs485Setup(HardwareSerial &serial, int dirPin)
        : serial(serial), dirPin(dirPin), isInitialized(false)
    {
    }

    common::error_type_t rs485Setup::init(unsigned long baudRate)
    {
        if (dirPin <= 0)
        {
            return common::error_type_t::INVALID_PIN;
        }

        if (isInitialized)
        {
            return common::error_type_t::ERROR_ALREADY_INITIALIZED;
        }

        pinMode(dirPin, OUTPUT);
        serial.begin(baudRate);
        isInitialized = true;

        return common::error_type_t::SUCCESS;
    }

    void rs485Setup::enableTransmit()
    {
        digitalWrite(dirPin, HIGH);
    }

    void rs485Setup::enableReceive()
    {
        digitalWrite(dirPin, LOW);
    }

    common::error_type_t rs485Setup::sendBytes(const uint8_t *data, size_t length)
    {
        if (!isInitialized)
        {
            return common::error_type_t::ERROR_NOT_INITIALIZED;
        }

        enableTransmit();
        serial.write(data, length);
        serial.flush();
        enableReceive();

        return common::error_type_t::SUCCESS;
    }

    common::error_type_t rs485Setup::receiveBytes(uint8_t *buffer, size_t length, size_t &bytesRead)
    {
        if (!isInitialized)
        {
            return common::error_type_t::ERROR_NOT_INITIALIZED;
        }

        enableReceive();

        bytesRead = 0;
        while (serial.available() && bytesRead < length)
        {
            buffer[bytesRead++] = serial.read();
        }

        if (bytesRead == 0)
        {
            return common::error_type_t::ERROR_TIMEOUT;
        }

        return common::error_type_t::SUCCESS;
    }

    common::error_type_t rs485Setup::deinit()
    {
        if (!isInitialized)
        {
            return common::error_type_t::ERROR_NOT_INITIALIZED;
        }

        serial.end();
        isInitialized = false;

        return common::error_type_t::SUCCESS;
    }
}
