#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <cstdint>
#include "common_header.hpp"
#include "crc.hpp"

namespace Protocol 
{
    struct ProtocolConfig
    {
        uint8_t slave_addr;
        uint16_t starting_addr;
        uint16_t qty_register;
    };

    enum FunctionCode
    {
        readRegister = 0x03,
        writeRegister = 0x06
    };

    class ProtocolInterface
    {
    public:
        virtual ~ProtocolInterface() = default;
        virtual common::error_type_t encodePayload(const ProtocolConfig &config, uint8_t* payload, size_t& length) = 0;
        virtual common::error_type_t decodePayload(const uint8_t* response, size_t length, uint16_t* data, size_t& dataCount) = 0;
        virtual common::error_type_t writePayload(const ProtocolConfig &config, uint8_t* payload, size_t& length) = 0;
    
     protected:
        static uint16_t calculateCRC(const uint8_t *data, uint16_t length);
   
   
    };

    class ProtocolImplementation : public ProtocolInterface
    {
    public:
        common::error_type_t encodePayload(const ProtocolConfig &config, uint8_t* payload, size_t& length) override;
        common::error_type_t decodePayload(const uint8_t* response, size_t length, uint16_t* data, size_t& dataCount) override;
        common::error_type_t writePayload(const ProtocolConfig &config, uint8_t* payload, size_t& length) override;
        
    
   
    };
}

#endif
