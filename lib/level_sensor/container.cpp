#include "container.hpp"
#include <cmath>

namespace Container
{
    const int cubicMeters_To_Litres = 1000; 
    const int mmConverter = 10000;
    float Cylinder::volume(float height){
        float result_in_litres = 0;
        Serial.printf("height passed in is: %f\n",height);
        Serial.printf("radius is: %f\n",radius);
        if(height < 0 || radius < 0){
            Serial.println("Invalid height or radius");
            return result_in_litres;
        }
        if(calibration){
            Serial.println("Using calibration");
            long converted_height = mmConverter * height;
            Serial.printf("converted height is: %ld\n",converted_height);
            result_in_litres= calibration->getVolume(converted_height);
            //result_in_litres= calibration->getVolume(16500);
        }else{
        float result = M_PI * std::pow(radius, 2) * height;
        Serial.println("Using formula");
        result_in_litres = result * cubicMeters_To_Litres; 

        }
        return result_in_litres;
    }

    float Cone::volume(float height){
        float result_in_litres = 0;
        if(height < 0 || radius < 0){
            return  result_in_litres;
        }
         if(calibration){
            Serial.println("Using calibration");
            long converted_height = mmConverter * height;
            result_in_litres= calibration->getVolume(converted_height);
            //result_in_litres= calibration->getVolume(16500);
        }else{
            float result = (1.0f / 3.0f) * M_PI * std::pow(radius, 2) * height;
             result_in_litres = result * cubicMeters_To_Litres;
        }      
        return result_in_litres;       
    }
    
    float Cuboid::volume(float height){
        float result_in_litres = 0;
        if(height < 0 || length < 0 || width < 0){
            return result_in_litres;
        }
         if(calibration){
            Serial.println("Using calibration");
            long converted_height = mmConverter * height;
            Serial.printf("converted height: %ld\n",converted_height);
            result_in_litres= calibration->getVolume(converted_height);
            //result_in_litres= calibration->getVolume(16500);
        }else{
            float result = length * width * height;
            result_in_litres = result * cubicMeters_To_Litres;

        }
        return result_in_litres;
    }

    float Cube::volume(float height){
        float result_in_litres = 0;
        if(height < 0){
            return  result_in_litres;
        }
         if(calibration){
            Serial.println("Using calibration");
            long converted_height = mmConverter * height;
            result_in_litres= calibration->getVolume(converted_height);
            //result_in_litres= calibration->getVolume(16500);
        } else{
            float result = std::pow(height, 3);
            result_in_litres = result * cubicMeters_To_Litres;

        }
        return result_in_litres;
    }
    
}
