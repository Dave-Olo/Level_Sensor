#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <cstdint>
#include "common_header.hpp"
#include "crc.hpp"

namespace Protocol 
{
    

    enum FunctionCode
    {
        readRegister = 0x03,
        writeRegister = 0x06
    };

    class ProtocolInterface
    {
    public:
        virtual ~ProtocolInterface() = default;
        virtual common::error_type_t encodePayload(uint8_t* payload, const size_t length,uint8_t* response, size_t* response_length) = 0;
        virtual common::error_type_t decodePayload(uint8_t* payload, const size_t length,uint8_t* response, size_t* response_length) = 0;
    
     protected:
        static uint16_t calculateCRC(const uint8_t *data, uint16_t length);
   
   
    };

    class Glt500Protocol : public ProtocolInterface
    {
    private:
        uint8_t slave_address;
    public:
        Glt500Protocol(uint8_t slave_address);
        common::error_type_t encodePayload(uint8_t* payload, const size_t length,uint8_t* response, size_t* response_length) override;
        common::error_type_t decodePayload(uint8_t* payload, const size_t length,uint8_t* response, size_t* response_length) override;
        
    };
}

#endif
