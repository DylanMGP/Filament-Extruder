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
the user to select either a pulling speed or intended diameter (Currently Unimplemented) to control the speed of
the extruder steppers.
*/ 

// TODO: Change the pins used for the interface: 
//      Encoder -> D2,D3 
//      Encoder Button -> D8
//      Display -> A0,A1,A2 Changing these is painful, you need to do it within the library
//      Stop Button -> D9
//      Driver Pins?
// TODO: Remove the options feature, just have speed control
// TODO: Check licences, rewrite anything that we cant use
// TODO: Fix the floating pont integer issue

// Define pins and values for driving the stepper motors:
#define dirPin 4
#define stepPin 5
#define motorInterfaceType 1

// Define pins for interface
#define encPinA 2
#define encPinB 3

// Define button pins
#define encButt 8
#define startButt 9
#define menuButt 8 // Soon to be depreciated


MotorDrive stepperMotor = MotorDrive(motorInterfaceType, stepPin, dirPin);
Interface interface = Interface(encPinA, encPinB, encButt, startButt, menuButt);

void setup()
{
    // Serial.begin(9600);

    stepperMotor.begin();
    interface.begin();
    interface.floatDisplay(stepperMotor.speed);
}

void loop()
{
    interface.runControls();

    // If the interface has "signalled" if a change is needed, do the change
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
    else if (interface.stopSig)
    {
        stepperMotor.stop();
    }
    
   
    stepperMotor.run();

    // Run the animation if the motor is moving
    if(stepperMotor.running)
    {
        interface.goAnimation();
    }

}