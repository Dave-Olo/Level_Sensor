#include <Arduino.h>
#include "rs485.hpp"
#include "protocol.hpp"
#include "level_sensor.hpp"

#define fluidDensity 1000  
#define gravity 9.81 
//#define PI 3.14159 
#define radius 0.1
#define baudRate 9600

const int TX_PIN = 17;  
const int RX_PIN = 16;  
const int DIR_PIN = 15; 

float calculateVolume(float height) {
    return PI * radius * radius * height; 
}

float pressureToHeight(float pressure) {
    return pressure / (fluidDensity * gravity); 
}


void setup() {
    Serial.begin(115200);
    Serial1.begin(baudRate, SERIAL_8N1, RX_PIN, TX_PIN); 
    pinMode(DIR_PIN, OUTPUT); 
    rs485Comms::rs485Setup rs485(Serial, DIR_PIN);
    Protocol::ProtocolImplementation protocol;
    sensor::LevelSensor levelSensor(protocol, rs485);

    float pressure = 0.0;
    common::error_type_t result = levelSensor.readPressure(pressure,baudRate);  
    if (result != common::error_type_t::SUCCESS) {
        Serial.println("Failed to read pressure.");
        return;
    }

    float height = pressureToHeight(pressure);
    Serial.print("Height: ");
    Serial.println(height);

    float volume = calculateVolume(height);
    Serial.print("Volume: ");
    Serial.print(volume);
    Serial.println(" cubic meters");
   
    

 }

void loop() {
    
}