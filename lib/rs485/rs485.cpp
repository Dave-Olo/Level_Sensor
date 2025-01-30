#include <Arduino.h>
#include "rs485.hpp"
#include "common_header.hpp"

namespace RS485Comms
{
    Rs485::Rs485(HardwareSerial &serial, int dirPin)
        : serial(serial), dirPin(dirPin), isInitialized(false)
    {
    }

    common::error_type_t Rs485::init()
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
        isInitialized = true;

        return common::error_type_t::SUCCESS;
    }

    void Rs485::enableTransmit()
    {
        digitalWrite(dirPin, HIGH);
    }

    void Rs485::enableReceive()
    {
        digitalWrite(dirPin, LOW);
    }

    common::error_type_t Rs485::sendBytes(const uint8_t *data, size_t length)
    {
        if (!isInitialized)
        {
             //Serial.println("rs485 not initialized");
            return common::error_type_t::ERROR_NOT_INITIALIZED;
        }
        //Serial.println("rs485 initialized");
        enableTransmit();
        //Serial.println("rs485 tx enabled");
        serial.write(data, length);
        //Serial.println("wrote data.");
        serial.flush();

        return common::error_type_t::SUCCESS;
    }

    common::error_type_t Rs485::receiveBytes(uint8_t *buffer, size_t length, size_t *bytesRead)
    {
        if (!isInitialized)
        {
            // Serial.println("rs 485 not initialized");
            return common::error_type_t::ERROR_NOT_INITIALIZED;
        }

        enableReceive();
        // Serial.printf("expected lenght: %d\n",length);
        uint8_t localBytesRead = 0;
        #define MAX_DELAY_COUNTER 1000
        #define MAX_TIMEOUT_COUNTER 1000000
        int delayCounter = 0;
        while(!serial.available() && ++delayCounter < MAX_DELAY_COUNTER){
            delay(1);
        }
        if(delayCounter >= MAX_DELAY_COUNTER){
            // Serial.println("rs 485 timed out");
            return common::error_type_t::ERROR_TIMEOUT;
        }
        long timeoutCounter =0;
        while (localBytesRead < length)
        {
            if(serial.available()){
                buffer[localBytesRead++] = serial.read();
                // Serial.printf("received: %d\n",buffer[localBytesRead-1]);
                timeoutCounter = 0;
            }else{
                timeoutCounter++;
                if(timeoutCounter > MAX_TIMEOUT_COUNTER){
                    return common::error_type_t::ERROR_TIMEOUT;;
                }
            }
        }
        // Serial.printf("actual lenght: %d\n",localBytesRead);
        *bytesRead = localBytesRead;
       // serial.flush();
         return common::error_type_t::SUCCESS;
    }

    common::error_type_t Rs485::deinit()
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
