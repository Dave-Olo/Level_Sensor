#include <Arduino.h>
#include "calibration.hpp"
#include "sdcard.hpp"
//#include <nlohmann/json.hpp>

namespace Calibration
{
    //using json = nlohmann::json;
    const constexpr int MAX_ROW_SIZE = 20;
    static char buffer[1024] = {};

    int lengthRead = 0;
    void CalibrationInterface::addVolumeCalibration(long height, float volume)
    {

        volumeCalibration[height] = volume;
    }

    void CalibrationInterface::printVolumeCalibration() const
    {
        for (const auto &point : volumeCalibration)
        {
            Serial.printf("Height: %ld, Volume: %.2f\n", point.first, point.second);
        }
    }

    float CalibrationInterface::getVolume(long height)
    {
        Serial.println("getting volume from calibration");
        Serial.printf("height passed to calibration is: %ld\n",height);
        if (auto search = volumeCalibration.find(height); search != volumeCalibration.end())
        {
            Serial.println("height found in table");
            return volumeCalibration[height];
        }
        else
        {
            Serial.println("height not found in table");
            auto upper = volumeCalibration.upper_bound(height);
            if (upper == volumeCalibration.end() || volumeCalibration.size() <= 1)
            {
                Serial.printf("table size: %d\n", volumeCalibration.size());
                if(upper == volumeCalibration.end())Serial.println("upper == end");
                Serial.println("Error in map size");
                return 0;
            }

            auto lower = upper--;
            float lower_bound = lower->second;
            float upper_bound = upper->second;
           
            long upper_key = upper->first;
            long lower_key = lower->first;
          
            Serial.printf("upper bound: %f, lower bound: %f\n", upper->second, lower->second);
          
            Serial.printf("upper key: %ld, lower key: %ld\n", upper->first, lower->first);
         
            float vol_interpol =((float)(height - upper_key)/(lower_key- upper_key)) * (lower_bound - upper_bound);
            Serial.printf("calculated interpol: %f\n", vol_interpol);
            float volume = upper_bound + vol_interpol;
            //float volume = lower_volume + ((float)(height - lower_height)/(upper_height-lower_height)) *(upper_volume - lower_volume);
            Serial.printf("calculated volume: %f\n", volume);
            return volume;
        }
    }

    common::error_type_t CalibrationWithCSV::load()
    {
        Serial.printf("Loading calibration data from CSV file: %s\n", filePath);

        const constexpr int MIN_FILE_LENGHT = 2;
        //char buffer[1024] = {};
        //char *a, b;

        int lengthRead = 0;

        common::error_type_t err = sdManager->readFile(SD, filePath, buffer,&lengthRead);
        
        if (err != common::error_type_t::SUCCESS)
        {
            Serial.println("readFile error returned not success");
            return common::error_type_t::ERROR_FILE_NOT_FOUND;
        }
        Serial.println("readFile error returned is success");
        char *current = buffer;
        int processedLenght = 0;
        while (lengthRead > MIN_FILE_LENGHT)
        {
            // int bytesRead = file.readBytesUntil('\n', buffer, sizeof(buffer) - 1);
            // buffer[bytesRead] = '\0';

            char row[MAX_ROW_SIZE];
            char *newlinePos = strchr(current, '\n');
            if (!newlinePos)
            {
                break;
            }
            int charNum = (newlinePos - current) / sizeof(char);
            lengthRead -= charNum;
            Serial.printf("char num is: %d\n", charNum);
            if (charNum >= MAX_ROW_SIZE)
            {
                return common::error_type_t::ERROR_FILE_PARSE_FAILED;
            }

            strncpy(row, current, charNum);
            row[charNum] = '\0';
            Serial.printf("row data is %s \n", row);
            char *commaPos = strchr(row, ',');
            if (commaPos)
            {
                *commaPos = '\0';
                const char *key = row;
                const char *value = commaPos + 1;
                Serial.printf("extracted key: %s \n", key);
                long height = atol(key);
                float volume = atof(value);
                Serial.printf("height: %ld : volume: %f\n", height, volume);
                volumeCalibration[height] = volume;
            }
            else
            {
                Serial.printf("Error: Parse error in line: %s\n", buffer);
                return common::error_type_t::ERROR_FILE_PARSE_FAILED;
            }
            current = newlinePos + 1;
        }

        return common::error_type_t::SUCCESS;
    }

    common::error_type_t CalibrationWithCSV::save()
    {
        Serial.printf("Saving calibration data to file: %s\n", filePath);
        char buff[MAX_ROW_SIZE];
        char mainBuffer[1024];
        char *ptr = mainBuffer;
        for (const auto &pair : volumeCalibration)
        {
            snprintf(buff, MAX_ROW_SIZE, "%ld,%.2f\n", pair.first, pair.second);
            int len = strlen(buff);
            strncat(ptr, buff, MAX_ROW_SIZE);
            ptr += len;
        };
        Serial.printf("main buffer is: %s\n", mainBuffer);
        sdManager->writeFile(SD, filePath, mainBuffer);

        return common::error_type_t::SUCCESS;
    }

  

    // common::error_type_t CalibrationWithJSON::load()
    // {
    //     common::error_type_t err = sdManager->readFile(SD, filePath, buffer, &lengthRead);
    //          if (err != common::error_type_t::SUCCESS){
    //                 Serial.printf("Failed to read file: %s, Error: %d\n", filePath, err);
    //               return err;
    //          }
    //
    //     Serial.printf("Loading calibration data from file: %s\n", filePath);
    //     int lengthRead = 0;
    //     //char buffer[2048] = {};
    //     common::error_type_t lengthRead = sdManager->readFile(SD, filePath, buffer);
    //     if (lengthRead <= 0)
    // 
    //     {
    //         Serial.printf("Failed to read file: %s\n", filePath);
    //         return common::error_type_t::ERROR_FILE_LOADING_FAILED;
    //     }

    //     StaticJsonDocument<1024> doc;
    //     DeserializationError error = deserializeJson(doc, buffer);
    //     if (error)
    //     {
    //         Serial.printf("Failed to parse JSON: %s\n", error.c_str());
    //         return common::error_type_t::ERROR_JSON_PARSE_FAILED;
    //     }
    //      
    //      JsonArray array = doc.as<JsonArray>();
    //      if (array.isNull())
    //      {
    //          Serial.println("Error: JSON data is not an array.");
    //          return common::error_type_t::ERROR_JSON_PARSE_FAILED;
    //      }
    //
    //
    //     for (JsonObject obj : doc.as<JsonArray>())
    //     {
    //         if (obj.containsKey("Height") && obj.containsKey("Volume"))
    //         {
    //             long height = obj["Height"];
    //             float volume = obj["Volume"];

    //             volumeCalibration[height] = volume;

    //             Serial.printf("Loaded height: %ld, volume: %f\n", height, volume);
    //         }
    //         else
    //         {
    //             Serial.println("Error: Missing keys in JSON object");
    //             return common::error_type_t::ERROR_JSON_PARSE_FAILED;
    //         }
    //     }

    //     return common::error_type_t::SUCCESS;
    // }

    // common::error_type_t CalibrationWithJSON::load()
    // {
    //     Serial.printf("Loading calibration data from file: %s\n", filePath);

    //     File file = SD.open(filePath);
    //     if (!file)
    //     {
    //         Serial.printf("Failed to open file: %s\n", filePath);
    //         return common::error_type_t::ERROR_FILE_NOT_FOUND;
    //     }

    //     int bytesRead = 0;
    //     common::error_type_t result = sdManager->readFile(SD, filePath, buffer);
    //     if (bytesRead <= 0)
    //     {
    //         Serial.printf("Failed to read file: %s\n", filePath);
    //         return common::error_type_t::ERROR_FILE_LOADING_FAILED;
    //     }

    //     String buffer;
    //     while (file.available())
    //     {
    //         buffer += (char)file.read();
    //     }
    //     file.close();

    //     StaticJsonDocument<1024> doc;
    //     DeserializationError error = deserializeJson(doc, buffer);
    //     if (error)
    //     {
    //         Serial.printf("Failed to parse JSON: %s\n", error.c_str());
    //         return common::error_type_t::ERROR_JSON_PARSE_FAILED;
    //     }

    //     for (JsonObject obj : doc.as<JsonArray>())
    //     {
    //         if (obj.containsKey("Height") && obj.containsKey("Volume"))
    //         {
    //             long height = obj["Height"];
    //             float volume = obj["Volume"];

    //             volumeCalibration[height] = volume;

    //             Serial.printf("Loaded height: %ld, volume: %f\n", height, volume);
    //         }
    //         else
    //         {
    //             Serial.println("Error: Missing keys in JSON object");
    //             return common::error_type_t::ERROR_JSON_PARSE_FAILED;
    //         }
    //     }

    //     return common::error_type_t::SUCCESS;
    // }
    // common::error_type_t CalibrationWithJSON::save()
    // {
    //     Serial.printf("Saving calibration data to file: %s\n", filePath);

    //     StaticJsonDocument<1024> doc;
    //     JsonArray jsonArray = doc.to<JsonArray>();

    //     for (const auto &pair : volumeCalibration)
    //     {
    //         JsonObject obj = jsonArray.createNestedObject();
    //         obj["Height"] = pair.first;  // Height
    //         obj["Volume"] = pair.second; // Volume
    //     }

    //     // Serialize JSON to a string
    //     char jsonString[1024]; // Adjust size if needed
    //     size_t jsonLength = serializeJson(doc, jsonString, sizeof(jsonString));

    //     if (jsonLength == 0)
    //     {
    //         Serial.println("Error: Failed to serialize JSON");
    //         return common::error_type_t::ERROR_FILE_WRITE_FAILED;
    //     }

    //     common::error_type_t err = sdManager->writeFile(SD, filePath, jsonString);
    //     if (err != common::error_type_t::SUCCESS){
    //          Serial.printf("Failed to write JSON file: %s, Error: %d\n", filePath, err);
    //          return err;
    //      }
    //     Serial.printf("Calibration data successfully saved to: %s\n", filePath);
    //     return common::error_type_t::SUCCESS;
    // }

    // common::error_type_t Calibration::CalibrationWithJSON::load()
    // {
    //     Serial.printf("Loading calibration data from file: %s\n", filePath);
    //     char buffer[2048] = {};

    //     common::error_type_t result = sdManager->readFile(SD, filePath, buffer);
    //     if (result != common::error_type_t::SUCCESS)
    //     {
    //         Serial.printf("Failed to read file: %s\n", filePath);
    //         //return common::error_type_t::ERROR_FILE_LOADING_FAILED;
    //         return result;
    //     }

    //     size_t bytesRead = strlen(buffer);
    //     if (bytesRead == 0)
    //     {
    //         Serial.printf("File is empty: %s\n", filePath);
    //         return common::error_type_t::ERROR_FILE_EMPTY;
    //     }

    //     try
    //     {
    //         buffer[bytesRead] = '\0';

    //         auto doc = json::parse(buffer);
    //         for (const auto &obj : doc)
    //         {
    //             if (obj.contains("Height") && obj.contains("Volume"))
    //             {
    //                 long height = obj["Height"].get<long>();
    //                 float volume = obj["Volume"].get<float>();

    //                 volumeCalibration[height] = volume;

    //                 Serial.printf("Loaded calibration data - Height: %ld, Volume: %f\n", height, volume);
    //             }
    //             // else
    //             // {
    //             //     Serial.printf("Skipping invalid object in JSON\n");
    //             // }
    //         }
    //     }
    //     catch (const json::parse_error &e)
    //     {
    //         Serial.printf("Failed to parse JSON: %s\n", e.what());
    //         return common::error_type_t::ERROR_JSON_PARSE_FAILED;
    //     }

    //     return common::error_type_t::SUCCESS;
    // }

    // common::error_type_t CalibrationWithJSON::save()
    // {
    //     Serial.printf("Saving calibration data to file: %s\n", filePath);
    //     json doc = json::array();
    //     for (const auto &pair : volumeCalibration)
    //     {
    //         doc.push_back({{"Height", pair.first},
    //                        {"Volume", pair.second}});
    //     }
    //     std::string jsonString = doc.dump();
    //     sdManager->writeFile(SD, filePath, jsonString.c_str());
    //     Serial.printf("Calibration data successfully saved to: %s\n", filePath);
    //     return common::error_type_t::SUCCESS;
    // }
}
