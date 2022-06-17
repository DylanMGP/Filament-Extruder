#include <Arduino.h>
#include <Servo.h>
#include <cutter.h>

Cutter::Cutter(int selectedPin, unsigned long interval, int length){
    Servo myservo;
    servoPin = selectedPin;
    checkingInterval = interval;
    filamentLength = length;

}

void Cutter::begin(){
    myservo.attach(servoPin);
    myservo.write(160);
    extrudeSpeed = 4.00f;
    filamentRemaining = (float)filamentLength;
    prevCheckTime = millis();
}

void Cutter::setSpeed(float speed){
    extrudeSpeed = speed;
}

void Cutter::setLength(int length){
    filamentRemaining += (float)(length - filamentLength); // Update the remaining extrusion distance to the new length
    filamentLength = length;
}

void Cutter::start(){
    running = true;
    filamentRemaining = (float)filamentLength;
    prevCheckTime = millis();
}

void Cutter::stop(){
    running = false;
}

void Cutter::cutFilament(){
    cut = true;
}

void Cutter::runCut(){
    if(cut){
        unsigned long currCheckTime = millis();
        
        if(!cutterOpen){
            myservo.write(180);
            prevCutTime = currCheckTime;
            cutterOpen = true;
        }
        else if(currCheckTime - prevCutTime > 50){ // return the cutter after a small delay
            myservo.write(160);
            cutterOpen = false;
            cut = false;
        }
    }
}

void Cutter::run(){
    if(running){
        unsigned long currCheckTime = millis();

        // Check and update extruded length on the interval we set
        if((currCheckTime - prevCheckTime) > checkingInterval){
            filamentRemaining -= extrudeSpeed * (checkingInterval/1000);

            prevCheckTime = currCheckTime;
            
            // When it's long enough, cut it
            if(filamentRemaining < 0){
                Cutter::cutFilament();
                filamentRemaining = (float)filamentLength;
            }
        }
    }

    Cutter::runCut();
}
