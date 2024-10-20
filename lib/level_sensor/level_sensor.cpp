// #include <Arduino.h>
// #include "level_sensor.hpp"

// namespace sensor
// {
//     LevelSensor::LevelSensor(Protocol::ProtocolInterface& protocolHandler, rs485Comms::RS485Interface& rs485Handler)
//         : protocol(protocolHandler), rs485(rs485Handler)
//     {}

//     common::error_type_t LevelSensor::readRegister(uint16_t regAddress, uint16_t& data)
//     {
//         uint8_t payload[8]; 
//         size_t payloadLength = 0;

//         Protocol::ProtocolConfig config;
//         config.slave_addr = 1;                 
//         config.starting_addr = regAddress;     
//         config.qty_register = 1;               

//         common::error_type_t encodeResult = protocol.encodePayload(config, payload, payloadLength);
//         if (encodeResult != common::error_type_t::SUCCESS) {
//             return encodeResult; 
//         }

      
//         common::error_type_t sendResult = rs485.sendBytes(payload, payloadLength);
//         if (sendResult != common::error_type_t::SUCCESS) {
//             return sendResult; 
//         }

//         uint8_t response[256];
//         size_t bytesRead = 0;

       
//         common::error_type_t receiveResult = rs485.receiveBytes(response, sizeof(response), bytesRead);
//         if (receiveResult != common::error_type_t::SUCCESS) {
//             return receiveResult; 
//         }

//         uint16_t dataArray[10];
//         size_t dataCount = 0;
//         common::error_type_t decodeResult = protocol.decodePayload(response, bytesRead, dataArray, dataCount);
//         if (decodeResult != common::error_type_t::SUCCESS) {
//             return decodeResult; 
//         }

        
//         if (dataCount > 0) {
//             data = dataArray[0];
//         } else {
//             return common::error_type_t::UNEXPECTED_BYTE_COUNT;
//         }

//         return common::error_type_t::SUCCESS;
//     }

   
//     common::error_type_t LevelSensor::readPressure(float& pressure, unsigned long baudRate)
    
//     {
//         common::error_type_t result = rs485.init(baudRate);
//         if (result != common::error_type_t::SUCCESS) {
//             return result;
//         }

        
//         uint16_t pressureUnit;
//         result = readRegister(0x0002, pressureUnit);
//         if (result != common::error_type_t::SUCCESS) {
//             return result;
//         }

        
//         uint16_t decimalOffset;
//         result = readRegister(0x0003, decimalOffset);
//         if (result != common::error_type_t::SUCCESS) {
//             return result;
//         }

        
//         uint16_t rawPressure;
//         result = readRegister(0x0004, rawPressure);
//         if (result != common::error_type_t::SUCCESS) {

//             return result;
//         }

        
//         float offsetFactor = 1.0;
//         if (decimalOffset == 0x0001) {
//             offsetFactor = 0.1;
//         } else if (decimalOffset == 0x0002) {
//             offsetFactor = 0.01;
//         } else if (decimalOffset == 0x0003) {
//             offsetFactor = 0.001;
//         }

//         pressure = rawPressure * offsetFactor;

//         return common::error_type_t::SUCCESS;
//     }
// }

#include <Arduino.h>
#include "level_sensor.hpp"

namespace sensor
{
    LevelSensor::LevelSensor(Protocol::ProtocolInterface& protocolHandler, rs485Comms::RS485Interface& rs485Handler)
        : protocol(protocolHandler), rs485(rs485Handler)
    {}

    common::error_type_t LevelSensor::readRegister(uint16_t regAddress, uint16_t& data)
    {
        uint8_t payload[8]; 
        size_t payloadLength = 0;

        Protocol::ProtocolConfig config;
        config.slave_addr = 1;                  // Example slave address
        config.starting_addr = regAddress;      // Start reading from the register
        config.qty_register = 1;                // Read one register

        // Encode the request
        //common::error_type_t encodeResult = protocol.encodePayload(config, payload, payloadLength);
        // common::error_type_t ProtocolInterface::encodePayload(const ProtocolConfig &config, uint8_t* payload, size_t& length);
        common::error_type_t encodeResult = protocol.encodePayload(config, payload, payloadLength);
        if (encodeResult != common::error_type_t::SUCCESS) {
            return encodeResult;
        }

        // Send the request to the sensor
        common::error_type_t sendResult = rs485.sendBytes(payload, payloadLength);
        if (sendResult != common::error_type_t::SUCCESS) {
            return sendResult;
        }

        // Prepare to receive the response
        uint8_t response[256];
        size_t bytesRead = 0;

        // Receive the response from the sensor
        common::error_type_t receiveResult = rs485.receiveBytes(response, sizeof(response), bytesRead);
        if (receiveResult != common::error_type_t::SUCCESS) {
            return receiveResult;
        }

        // Decode the received data
        uint16_t dataArray[10];
        size_t dataCount = 0;
        common::error_type_t decodeResult = protocol.decodePayload(response, bytesRead, dataArray, dataCount);
        if (decodeResult != common::error_type_t::SUCCESS) {
            return decodeResult;
        }

        // Ensure data is received
        if (dataCount > 0) {
            data = dataArray[0];
        } else {
            return common::error_type_t::UNEXPECTED_BYTE_COUNT;
        }

        return common::error_type_t::SUCCESS;
    }

    common::error_type_t LevelSensor::readPressure(float& pressure, unsigned long baudRate)
    {
        // Initialize RS485 communication with the given baud rate
        common::error_type_t result = rs485.init(baudRate);
        if (result != common::error_type_t::SUCCESS) {
            return result;
        }

        // Read multiple registers in a single request
        const uint16_t startingRegister = 0x0002; // Starting address for pressure registers
        const uint16_t numberOfRegisters = 3;     // Read pressureUnit, decimalOffset, rawPressure

        uint16_t dataArray[3]; // Array to store multiple register values
        size_t dataCount = 0;

        // Use protocol to read multiple registers in one go
        Protocol::ProtocolConfig config;
        config.slave_addr = 1;
        config.starting_addr = startingRegister;
        config.qty_register = numberOfRegisters;

        uint8_t payload[8]; 
        size_t payloadLength = 0;

        // Encode the payload for reading multiple registers
        common::error_type_t encodeResult = protocol.encodePayload(config, payload, payloadLength);
        if (encodeResult != common::error_type_t::SUCCESS) {
            return encodeResult;
        }

        // Send the request
        common::error_type_t sendResult = rs485.sendBytes(payload, payloadLength);
        if (sendResult != common::error_type_t::SUCCESS) {
            return sendResult;
        }

        // Prepare the response buffer
        uint8_t response[256];
        size_t bytesRead = 0;

        // Receive the response
        common::error_type_t receiveResult = rs485.receiveBytes(response, sizeof(response), bytesRead);
        if (receiveResult != common::error_type_t::SUCCESS) {
            return receiveResult;
        }

        // Decode the received response
        common::error_type_t decodeResult = protocol.decodePayload(response, bytesRead, dataArray, dataCount);
        if (decodeResult != common::error_type_t::SUCCESS || dataCount < numberOfRegisters) {
            return common::error_type_t::UNEXPECTED_BYTE_COUNT;
        }

        // Extract the register values from the dataArray
        uint16_t pressureUnit = dataArray[0];
        uint16_t decimalOffset = dataArray[1];
        uint16_t rawPressure = dataArray[2];

        // Convert raw pressure using the decimal offset
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

        // Calculate the final pressure value
        pressure = rawPressure * offsetFactor;

        // Deinitialize RS485 communication after reading
        rs485.deinit();

        return common::error_type_t::SUCCESS;
    }
}