#include <Arduino.h>
#include "protocol.hpp"

namespace Protocol
{
    Glt500Protocol::Glt500Protocol(uint8_t slave_address):slave_address(slave_address){

    }
    common::error_type_t Glt500Protocol::encodePayload(uint8_t* payload, const size_t length,uint8_t* response, size_t* response_length)
    {
        if(payload == NULL || response == NULL){
            common::error_type_t::NULL_PARAMETER;
        }
        uint8_t tempBuffer[8];
        tempBuffer[0] = slave_address;
        tempBuffer[1] = FunctionCode::readRegister;
        tempBuffer[2] = 0x00;
        tempBuffer[3] = payload[0];
        tempBuffer[4] = 0x00;
        tempBuffer[5] = payload[1];

        uint16_t crc = ProtocolInterface::calculateCRC(tempBuffer, 6);
        tempBuffer[6] = crc & 0xFF;
        tempBuffer[7] = (crc >> 8) & 0xFF;
        Serial.printf("CRC 6 %.02X CRC 7 %.02X\n",tempBuffer[6],tempBuffer[7]);
        memcpy(response,tempBuffer,8);
        *response_length = 8;
        return common::error_type_t::SUCCESS;
    }

    common::error_type_t Glt500Protocol::decodePayload(uint8_t* payload, const size_t length,uint8_t* response, size_t* response_length)
    {
        constexpr const int DATA_START_INDEX=3;
        if(payload == NULL || response == NULL){
            common::error_type_t::NULL_PARAMETER;
        }
        if (length < 5)
        {
            return common::error_type_t::FRAME_ERROR;
        }

        uint16_t receivedCRC = (payload[length - 1] << 8) | payload[length - 2];
        uint16_t calculatedCRC = ProtocolInterface::calculateCRC(payload, length - 2);
         Serial.printf("calculatedCRC %.04X receivedCRC %.04X\n",calculatedCRC,receivedCRC);
        if (receivedCRC != calculatedCRC)
        {
            return common::error_type_t::CRC_ERROR;
        }

        uint8_t functionCode = response[1];
        // if (functionCode != FunctionCode::readRegister)
        // {
        //     return common::error_type_t::INVALID_FUNCTION;
        // }

        uint8_t byteCount = payload[2];
        if (length != 5 + byteCount)
        {
            return common::error_type_t::FRAME_ERROR;
        }

        // for (size_t i = 0; i < dataCount; ++i) {
        //     data[i] = (response[3 + (i * 2)] << 8) | response[4 + (i * 2)];
        // }
        memcpy(response,payload+DATA_START_INDEX,byteCount);
        *response_length = byteCount;
        return common::error_type_t::SUCCESS;
    }

    

    uint16_t ProtocolInterface::calculateCRC(const uint8_t *data, uint16_t length)
    {
        return CRC16::CalculateCRC(data, length);
    }

}