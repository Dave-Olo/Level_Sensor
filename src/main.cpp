#include <Arduino.h>
#include "calibration.hpp"
#include "sdcard.hpp"
#include "rs485.hpp"
#include "protocol.hpp"
#include "level_sensor.hpp"
#include "container.hpp"

#define baudRate 9600
#define FILENAME "/calibration.csv"
const int TX_PIN = 17;
const int RX_PIN = 16;
const int DIR_PIN = 4;

Sensor::LevelSensorInterface *levelSensor;
RS485Comms::CommInterface *rs485;
Protocol::ProtocolInterface *protocol;
Calibration::CalibrationInterface *calibration;
//Calibration::CalibrationInterface *csvcalibration;
Sdcard::SDCardManager sdManager;
Container::ContainerInterface* container;
//Calibration::CalibrationWithJSON Calibrator("/Calib.json", &sdManager);
const uint8_t slave_address = 1;

//float radius = 0.3;
// float length = 6.5;
// float width = 6.3;
//Container::Cylinder *cylinder;
// Container::Cone *cone;
// Container::Cuboid *cuboid;

void setup()
{
    
    Serial.begin(115200);
    Serial1.begin(baudRate, SERIAL_8N1, RX_PIN, TX_PIN);
    Serial.println("delaying for 10 seconds");
    delay(10000);
    Serial.println("Serial Monitor Started");
    pinMode(DIR_PIN, OUTPUT);
    rs485 = new RS485Comms::Rs485(Serial1, DIR_PIN);
    protocol = new Protocol::Glt500Protocol(slave_address);
    Serial.println("Protocol Setup");
    levelSensor = new Sensor::GLT500Level(*protocol, *rs485);
    Serial.println("got here.");
    common::error_type_t result = levelSensor->init();
    Serial.println("got here.");
    if (result != common::error_type_t::SUCCESS)
    {
        Serial.println("Failed to init level sensor.");
        // return;
        while (true);
    }
    Serial.println("Setup Complete");
    sdManager.initSDCard();
    calibration = new Calibration::CalibrationWithCSV(FILENAME,&sdManager);
    // if (Calibrator.load() == common::error_type_t::SUCCESS)
    if (calibration->load() == common::error_type_t::SUCCESS)
    {
        Serial.println("Calibration data loaded successfully.");
    }
    else
    {
        Serial.println("Failed to load calibration data.");
    }

    // Serial.println("<<<<<<<<<< Calibration after loading begin >>>>>>>>>>>>>>>");
    // calibration->printVolumeCalibration();
    // //calibration->printVolumeCalibration();
    // Serial.println("<<<<<<<<<< Calibration after loading end >>>>>>>>>>>>>>>");
    // // Calibrator.addVolumeCalibration(7050, 0.73);
    // // Calibrator.addVolumeCalibration(7070, 0.74);


    // // if (calibration->save() == common::error_type_t::SUCCESS) {
    // //     Serial.println("Calibration data saved successfully.");
    // // } else {
    // //     Serial.println("Failed to save calibration data.");
    // // }

    // // if (Calibrator.save() == common::error_type_t::SUCCESS)
    //  if (calibration->save() == common::error_type_t::SUCCESS)
    // {
    //     Serial.println("Calibration data saved successfully.");
    // }
    // else
    // {
    //     Serial.println("Failed to save calibration data.");
    // }

    // Serial.println("<<<<<<<<<< Calibration after adding begin >>>>>>>>>>>>>>>");
    // // Calibrator.printVolumeCalibration();
    // calibration->printVolumeCalibration();
    // Serial.println("<<<<<<<<<< Calibration after adding end >>>>>>>>>>>>>>>");

    // std::string filename = "calibration.csv";
    // calibration = new Calibration::ContainerCalibration();
    // csvcalibration = new Calibration::CalibrationWithCSV(filename);
    // calibration->addVolumeCalibration(16000, 10.0f);
    // calibration->addVolumeCalibration(17000, 12.0f);
    // calibration->addVolumeCalibration(18000, 14.0f);
    // calibration->addVolumeCalibration(5, 8.0f);
    // calibration->addVolumeCalibration(6, 9.0f);

    // // csvcalibration->load();
    // // csvcalibration->printVolumeCalibration();

    // cylinder = new Container::Cylinder(radius);
    //  cone = new container::Cone(radius);
    //  cuboid = new container::Cuboid(length, width);
    container = new Container::Cylinder(calibration);
}

void loop()
{

    float height;
    float volume;

    common::error_type_t result = levelSensor->getHeight(height);
    if (result != common::error_type_t::SUCCESS)
    {
        Serial.println("Failed to calculate height.");
        delay(1000);
        return;
    }

    // float cylinder_volume = cylinder->volume(volume, height);
    // if (cylinder_volume != 0)
    // {
    //     Serial.printf("Cylinder Volume: %.4f Litres\n", volume);
    // }
    // else
    // {
    //     Serial.println("Failed to calculate cylinder volume.");
    // }

    // float cone_volume = cone->volume(volume, height);
    // if (cone_volume != 0) {
    //     Serial.printf("Cone Volume: %.4f Litres\n", volume);
    // } else {
    //     Serial.println("Failed to calculate cone volume.");
    // }

    // float cuboid_volume = cuboid->volume(volume, height);
    // if (cuboid_volume != 0) {
    //     Serial.printf("Cuboid Volume: %.2f Litres\n", volume);
    // } else {
    //     Serial.println("Failed to calculate cuboid volume.");
    // }

    
    Serial.printf("Retrieved Height: %f\n", height);
    // volume = container->volume(height);
    // Serial.printf("volume is: %f \n",volume);
    
    // // // volume = Calibrator.getVolume(height);
    // volume = calibration->getVolume(height);
    // if (volume == 0)
    // {
    //     Serial.println("Failed to calculate volume (either height not found or error in interpolation).");
    // }
    // else
    // {
    //     Serial.printf("Calculated Volume: %.2f Litres\n", volume);
    // }

    delay(1000);
}
