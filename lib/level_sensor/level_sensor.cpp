#include <Arduino.h>
#include <cmath>
#include "level_sensor.hpp"

namespace Sensor
{
     GLT500Level::~GLT500Level() {
    
    }
    GLT500Level::GLT500Level(Protocol::ProtocolInterface& protocolHandler, RS485Comms::CommInterface& rs485Handler)
        : protocol(protocolHandler), rs485(rs485Handler) {}

    common::error_type_t GLT500Level::init(){
        common::error_type_t result = rs485.init();
        if (result != common::error_type_t::SUCCESS) {
            return result;
        }
         Serial.println("Level_Sensor initialized");
        return common::error_type_t::SUCCESS;
       
    }

    // common::error_type_t LevelSensor::test(){
    //     uint8_t test_packet[]={0x01,0x03,0x00,0x00,0x00,0x01,0x84,0x0A};
    //      Serial.println("sending packet");
    //     common::error_type_t err = rs485.sendBytes(test_packet,8);
    //     if (err != common::error_type_t::SUCCESS) {
    //         Serial.println("sending failed");
    //         return err;
    //     }
    //     //delay(1);
    //     uint8_t payload[7];
    //     size_t bytes_read;
    //     err = rs485.receiveBytes(payload,7,&bytes_read);
    //     if (err != common::error_type_t::SUCCESS) {
    //         Serial.println("rx failed");
    //         return err;
    //     }
    //     Serial.println("lenght read is: "+(String)bytes_read);
    //     char buff[100];
    //     for(int x=0;x<7;x++){
    //         Serial.printf("0x%.2X, ",payload[x]);
    //         //Serial.println("byte "+(String)x +"is: "+(String)payload[x]);
    //     }
    //     Serial.println();
    //     return common::error_type_t::SUCCESS;
    // }
    common::error_type_t GLT500Level::readPressure(float& pressure)
    {
        const uint16_t startingRegister = 0x0002; 
        const uint16_t numberOfRegisters = 3;     
        uint8_t response[50];
        uint8_t payload[2] = {startingRegister,numberOfRegisters}; 
        size_t payloadSize = 2;

        uint8_t encodeResponse[20]; 
        size_t encodeResponseLength = 0;

        uint8_t decodeResponse[20]; 
        size_t decodeResponseLength = 0;

        common::error_type_t encodeResult = protocol.encodePayload(payload, payloadSize,encodeResponse,&encodeResponseLength);
        if (encodeResult != common::error_type_t::SUCCESS) {
            return encodeResult;
        }
        Serial.println("encode successful");
        for(int k = 0; k <encodeResponseLength;k++){
            Serial.printf("%.02X , ",encodeResponse[k]);
        }
        Serial.println("");
        common::error_type_t sendResult = rs485.sendBytes(encodeResponse, encodeResponseLength);
        if (sendResult != common::error_type_t::SUCCESS) {
            return sendResult;
        }

        constexpr const size_t expectedBytesRead = 11;
        size_t actualBytesRead = 0;

        common::error_type_t receiveResult = rs485.receiveBytes(response, expectedBytesRead, &actualBytesRead);
        if (receiveResult != common::error_type_t::SUCCESS) {
            return receiveResult;
        }
        Serial.println("rx  485 response received");
        if (expectedBytesRead != actualBytesRead) {
            return common::error_type_t::UNEXPECTED_BYTE_COUNT;
        }
        Serial.println("actual response is same as expected");
        common::error_type_t decodeResult = protocol.decodePayload(response, actualBytesRead, decodeResponse, &decodeResponseLength);
        if (decodeResult != common::error_type_t::SUCCESS) {
            return decodeResult;
        }
        Serial.println("decode successful");
       
        uint16_t pressureUnit = decodeResponse[0] << 8 | decodeResponse[1];
        uint16_t decimalOffset = decodeResponse[2] << 8 | decodeResponse[3];
        uint16_t rawPressure = decodeResponse[4] << 8 | decodeResponse[5];

       
        float offsetFactor = 1.0;
        switch (decimalOffset)
        {
            case 0x0001:
                offsetFactor = 0.1;
                break;
            case 0x0002:
                offsetFactor = 0.01;
                break;
            case 0x0003:
                offsetFactor = 0.001;
                break;
            default:
                offsetFactor = 1.0;
                break;
        }

       
        pressure = rawPressure * offsetFactor;
        String pressureArray[] = {"MPa", "KPa", "Pa", "bar", "mbar", "kg/cm2", "psi", "mH2O", "mmH20", "0C", "cmH2O"};
        // Serial.printf("pressure value is: %f",pressure);
        Serial.printf("Pressure value is: %.4f %s\n", pressure, pressureArray[3].c_str());
       //Serial.println
       // rs485.deinit();

        return common::error_type_t::SUCCESS;
    }

    common::error_type_t GLT500Level::getHeight(float& height){
        const float gravity = 9.81;
        const float fluidDensity = 1000;
        float pressure;
        if (readPressure(pressure) != common::error_type_t::SUCCESS) {
        Serial.println("Failed to read pressure");
        return common::error_type_t::PRESSURE_READ_ERROR;
         }
        height = pressure/(fluidDensity * gravity);
        //height = round((pressure / (fluidDensity * gravity)) * 10000);
        Serial.printf("The value of height is: %.4f m\n", height);

        return common::error_type_t::SUCCESS;
    }

}