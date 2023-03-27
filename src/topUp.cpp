#include <Arduino.h>
#include <Servo.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <topUp.h>

TopUp::TopUp(int stepPin, int dirPin, int servoDoorPin, int servoDumpPin, TwoWire &WirePort){

}
void TopUp::run(){
    if(running){
        unsigned long currCheckTime = millis();

        if((currCheckTime - prevCheckTime) > checkingInterval){
            runningMass -= massExtruded(extrudedDiameter, extrudeSpeed, checkingInterval, density);

            if((-runningMass) > dumpSize){
                dumping = true;
                runningMass += dumpSize;
            }
        }
    }
}
float TopUp::massExtruded(float extrudedDiameter, float extrudeSpeed, unsigned long checkingInterval, float density){
    float length = (extrudeSpeed * (checkingInterval / 1000)) / 1000;
    float area = PI * sq(extrudedDiameter / 2);
    return area * length * density;
}
