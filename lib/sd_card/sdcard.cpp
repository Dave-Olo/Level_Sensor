#include "sdcard.hpp"
namespace Sdcard
{
    SDCardManager::SDCardManager()
    {
        initialized = false;
    }

    common::error_type_t SDCardManager::initSDCard()
    {
        if (!initialized)
        {
            SPI.begin(18, 19, 23, 5);
            pinMode(5, OUTPUT);
            if (!SD.begin())
            {
                Serial.println("Card Mount Failed");
                return common::error_type_t::CARD_MOUNT_FAILED;
            }

            uint8_t cardType = SD.cardType();

            if (cardType == CARD_NONE)
            {
                Serial.println("No SD card attached");
                 return common::error_type_t::NO_CARD_ATTACHED;
            }

            Serial.print("SD Card Type: ");
            if (cardType == CARD_MMC)
            {
                Serial.println("MMC");
            }
            else if (cardType == CARD_SD)
            {
                Serial.println("SDSC");
            }
            else if (cardType == CARD_SDHC)
            {
                Serial.println("SDHC");
            }
            else
            {
                Serial.println("UNKNOWN");
            }

            uint64_t cardSize = SD.cardSize() / (1024 * 1024);
            Serial.printf("SD Card Size: %lluMB\n", cardSize);

            initialized = true;
        }
        Serial.println("SD card successfully initialised");
        return common::error_type_t::CARD_SUCCESSFUL;
    }
    
    common::error_type_t SDCardManager::readFile(fs::FS &fs, const char *path,char* buffer, int * lengthRead)
    {
        int counter = 0;
        Serial.println("Entered the read file");
        Serial.printf("Reading file: %s\n", path);

        File file = fs.open(path);
        if (!file)
        {
            Serial.println("Failed to open file for reading");
            file.close();
            return common::error_type_t::ERROR_FILE_LOADING_FAILED;
        }

        Serial.print("Read from file: ");
        

        while (file.available())
        {
            char data = file.read();
            Serial.write(data);
            buffer[counter++] = data;
        }
        *lengthRead = counter;
        Serial.println("Successfully read the file");
        file.close();
        return common::error_type_t::SUCCESS;
    }

    common::error_type_t SDCardManager::writeFile(fs::FS &fs, const char *path, const char *message)
    {
        Serial.printf("Writing file: %s\n", path);

        File file = fs.open(path, FILE_WRITE);
        if (!file)
        {
            Serial.println("Failed to open file for writing");
            file.close();
            return common::error_type_t::ERROR_FILE_OPEN_FAILED;
        }
        if (file.print(message))
        {
            Serial.println("File written");
        }
        else
        {
            Serial.println("Write failed");
            file.close();
            return common::error_type_t::ERROR_FILE_WRITE_FAILED;
        }
        file.close();
        return common::error_type_t::SUCCESS;
    }

    common::error_type_t SDCardManager::appendFile(fs::FS &fs, const char *path, const char *message)
    {
        Serial.printf("Appending to file: %s\n", path);

        File file = fs.open(path, FILE_APPEND);
        if (!file)
        {
            Serial.println("Failed to open file for appending");
              return common::error_type_t::ERROR_FILE_OPEN_FAILED;
        }
        if (!file.print(message))
        {
            Serial.println("Message appended failed");
             file.close();
            return common::error_type_t::ERROR_FILE_OPEN_FAILED;
        }
        file.close();
        return common::error_type_t::SUCCESS;
    }

}
