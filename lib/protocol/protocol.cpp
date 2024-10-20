#include <Arduino.h>
#include "protocol.hpp"

namespace Protocol 
{
    
        
        common::error_type_t ProtocolImplementation::encodePayload(const ProtocolConfig &config, uint8_t* payload, size_t& length)
        {
            payload[0] = config.slave_addr;
            payload[1] = FunctionCode::readRegister;
            payload[2] = (config.starting_addr >> 8) & 0xFF;
            payload[3] = config.starting_addr & 0xFF;
            payload[4] = (config.qty_register >> 8) & 0xFF;
            payload[5] = config.qty_register & 0xFF;

            uint16_t crc = ProtocolInterface::calculateCRC(payload, 6); 
            payload[6] = crc & 0xFF;
            payload[7] = (crc >> 8) & 0xFF;

            length = 8;
            return common::error_type_t::SUCCESS;
        }

        // Implement the decoding for reading
        common::error_type_t ProtocolImplementation::decodePayload(const uint8_t* response, size_t length, uint16_t* data, size_t& dataCount)
        {
            if (length < 5) {
                return common::error_type_t::FRAME_ERROR;
            }

            uint16_t receivedCRC = (response[length - 1] << 8) | response[length - 2];
            uint16_t calculatedCRC = ProtocolInterface::calculateCRC(response, length - 2);  // CRC calculation
            if (receivedCRC != calculatedCRC) {
                return common::error_type_t::CRC_ERROR;
            }

            uint8_t functionCode = response[1];
            if (functionCode != FunctionCode::readRegister) {
                return common::error_type_t::INVALID_FUNCTION;
            }

            uint8_t byteCount = response[2];
            if (length != 5 + byteCount) {
                return common::error_type_t::FRAME_ERROR;
            }

            dataCount = byteCount / 2;
            for (size_t i = 0; i < dataCount; ++i) {
                data[i] = (response[3 + (i * 2)] << 8) | response[4 + (i * 2)];
            }

            return common::error_type_t::SUCCESS;
        }


        common::error_type_t ProtocolImplementation::writePayload(const ProtocolConfig &config, uint8_t* payload, size_t& length)
        {
            payload[0] = config.slave_addr;
            payload[1] = FunctionCode::writeRegister;
            payload[2] = (config.starting_addr >> 8) & 0xFF;
            payload[3] = config.starting_addr & 0xFF;
            payload[4] = (config.qty_register >> 8) & 0xFF;  
            payload[5] = config.qty_register & 0xFF;

            uint16_t crc = ProtocolInterface::calculateCRC(payload, 6);  
            payload[6] = crc & 0xFF;
            payload[7] = (crc >> 8) & 0xFF;

            length = 8;
            return common::error_type_t::SUCCESS;
        }
    

    
        uint16_t ProtocolInterface::calculateCRC(const uint8_t *data, uint16_t length)
        {
            return CRC16::CalculateCRC(data, length);  
        }
   
}