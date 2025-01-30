#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include <Arduino.h>
#include <cstdint>
#include "common_header.hpp"
#include <HardwareSerial.h>
#include "level_sensor.hpp"
#include "calibration.hpp"

namespace Container
{

    class ContainerInterface
    {
    public:
        ContainerInterface(Calibration::CalibrationInterface* calibration):calibration(calibration){

        }
        ContainerInterface():ContainerInterface(nullptr){

        }
        virtual float volume(float height) = 0;
        virtual ~ContainerInterface() = default;
    protected: 
        Calibration::CalibrationInterface* calibration;
    };

    class Cylinder : public ContainerInterface
    {
    public:
        Cylinder(float radius);
        Cylinder(Calibration::CalibrationInterface* calibration):ContainerInterface(calibration){
            Serial.printf("constructor with calibration called, calibaration object points to: %p\n",calibration);
        }
        float volume(float height) override;
    private:
        float radius=0;

    };

    class Cone : public ContainerInterface
    {
    public:
        Cone(float radius);
        Cone(Calibration::CalibrationInterface* calibration):ContainerInterface(calibration){

        }
        float volume(float height) override;

    private:
        float radius=0;

    };

    class Cuboid : public ContainerInterface
    {
    public:
        Cuboid(float length, float width); 
        Cuboid(Calibration::CalibrationInterface* calibration):ContainerInterface(calibration){

        }
        float volume(float height) override;

    private:
        float length=0;
        float width=0;
    };

    class Cube : public ContainerInterface
    {
    public:
         Cube(float length);
        Cube(Calibration::CalibrationInterface* calibration):ContainerInterface(calibration){

        }
        float volume(float height) override;


    private:
        float length=0;
    };


}

#endif
