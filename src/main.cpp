#include <Arduino.h>

#include <max7219.h>

#include <Button.h>

#include <Encoder.h>

#include <MotorDrive.h>

#include <interface.h>
/*
Name: Glass filament Extruder program
Author: Mackinley Shaw
Email: mackshaw@iinet.net.au
Purpose: Includes all the programming for the glass filament extruder device. Allows
the user to select either a pulling speed or intended diameter to control the speed of
the extrider steppers.
Libraries used:
- TBD
*/

// Define pins and values for driving the stepper motors:
#define enbPin 3
#define dirPin 4
#define stepPin 5
#define motorInterfaceType 1

// Define pins for interface
#define encPinA 11
#define encPinB 12

// Define button pins
#define encButt 10
#define startButt 9
#define menuButt 8

MotorDrive stepperMotor = MotorDrive(motorInterfaceType, stepPin, dirPin, enbPin);
Interface interface = Interface(encPinA, encPinB, encButt, startButt, menuButt);

// animation parameters
// const int waitTime = 100;
// char frames[] = {0b1000000, 0b0100000, 0b0010000, 0b0001000, 0b0000100, 0b0000010};
// int frameLength = sizeof(frames) / sizeof(char);
// int frameIndex = 0;
// unsigned long animTimerOld;

MAX7219 max7219;

// Handles the animation when the extruder is running
// void goAnimation()
// {
//     unsigned long animTimer = millis();

//     if (animTimer - animTimerOld >= waitTime)
//     {
//         animTimerOld = animTimer;

//         frameIndex++;

//         if (frameIndex < 0)
//         {
//             frameIndex = frameLength - 1;
//         }
//         else if (frameIndex >= frameLength)
//         {
//             frameIndex = 0;
//         }

//         max7219.MAX7219_Write(8, frames[frameIndex]);
//     }
// }



void setup()
{
    Serial.begin(9600);

    stepperMotor.begin();
    interface.begin();
    interface.floatDisplay(stepperMotor.speed);
}

void loop()
{
    interface.runControls();
    if (interface.startSig)
    {
        stepperMotor.startStop();
    }
    else if (interface.speedSig)
    {
        stepperMotor.incSpeed(interface.increment);
        interface.floatDisplay(stepperMotor.speed);
    }
    else if (interface.stepSig)
    {
        stepperMotor.incSteps(interface.increment);
        interface.floatDisplay(stepperMotor.stepPerMilimetre);
    }
   
    stepperMotor.run();

    if(stepperMotor.running)
    {
        interface.goAnimation();
    }

}