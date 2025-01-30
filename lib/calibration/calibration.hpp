#ifndef __CALIBRATION_H__
#define __CALIBRATION_H__

#include <map>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
// #include <ArduinoJson.h>
//#include <nlohmann/json.hpp>
#include <common_header.hpp>
#include"sdcard.hpp"
namespace Calibration {

    class CalibrationInterface {
    public:
        virtual void addVolumeCalibration(long height, float volume);
        virtual float getVolume(long height);
        virtual void printVolumeCalibration() const;  
        virtual ~CalibrationInterface() = default;

        virtual common::error_type_t load() {
            return common::error_type_t::SUCCESS;
        }


        virtual common::error_type_t save() {
            return common::error_type_t::SUCCESS;
        }
    protected:
        std::map<long, float> volumeCalibration;
    };

    class ContainerCalibration : public CalibrationInterface {
    private:
        // std::map<long, float> calibrationData;


    public:
    //    void addVolumeCalibration(long height, float volume) override;
    //    float getVolume(float height) override;
    //    void printVolumeCalibration()const override;
       

    };
    

    class CalibrationWithCSV : public CalibrationInterface {
    private:
        const char * filePath;
        Sdcard::SDCardManager *sdManager;
        
    public:
        CalibrationWithCSV(const char *filePath,Sdcard::SDCardManager *sdManager): filePath(filePath),sdManager(sdManager),CalibrationInterface() {}

        common::error_type_t load() override;
        common::error_type_t save() override;
    };

    class CalibrationWithJSON: public CalibrationInterface {
    private:
        const char * filePath;
        Sdcard::SDCardManager *sdManager;
    public:
        CalibrationWithJSON(const char *filePath,Sdcard::SDCardManager *sdManager): filePath(filePath),sdManager(sdManager),CalibrationInterface(){}
        
        common::error_type_t load();
        common::error_type_t save();

    
    };


} 

#endif 
