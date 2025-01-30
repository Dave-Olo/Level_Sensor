#ifndef SDCARD_H
#define SDCARD_H
#include <common_header.hpp>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
namespace Sdcard
{
    class SDCardManager
    {
    public:
        SDCardManager();
        common::error_type_t initSDCard();
        common::error_type_t readFile(fs::FS &fs, const char *path,char* buffer, int * lengthRead);
        common::error_type_t writeFile(fs::FS &fs, const char *path, const char *message);
        common::error_type_t appendFile(fs::FS &fs, const char *path, const char *message);
    private:
        bool initialized;
    };
}
#endif
