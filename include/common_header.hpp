#ifndef _COMMON_HEADER_H_
#define _COMMON_HEADER_H_

#include <Arduino.h>

namespace common
{
    enum class error_type_t
    {
        SUCCESS,
        FAILURE,
        INVALID_PIN,
        NULL_PARAMETER,
        COMMUNICATION_ERROR,
        INVALID_ID,
        INVALID_FUNCTION,
        INVALID_QUANTITY,
        FRAME_ERROR,
        CRC_ERROR,
        UNEXPECTED_BYTE_COUNT,
        INIT_FAIL,
        TX_ERROR,
        ERROR_TIMEOUT,
        RX_ERROR,
        ERROR_NONE,
        ERROR_NOT_INITIALIZED,
        ERROR_ALREADY_INITIALIZED
    };
}

#endif 
