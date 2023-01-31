#include <Arduino.h>

#include <max7219.h>

#include <Button.h>

#include <Encoder.h>

#include <MotorDrive.h>

#include <interface.h>

#include <cutter.h>
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
#define enablePin 13
#define dirPin 6
#define stepPin 7
#define motorInterfaceType 1
#define DMODE_0          12
#define DMODE_1          11
#define DMODE_2          10

// Define pins for interface
#define encPinA 2
#define encPinB 3

// Define button pins
#define encButt 8
#define startButt 9
#define startButtLED 5

// Define cutter parameters
#define servoPin 4
#define CHECKING_INTERVAL 500
#define FILAMENT_LENGTH 1000 // mm


MotorDrive stepperMotor = MotorDrive(motorInterfaceType, stepPin, dirPin, enablePin);
Interface interface = Interface(encPinA, encPinB, encButt, startButt);
Cutter cutter = Cutter(servoPin, CHECKING_INTERVAL, FILAMENT_LENGTH);

void setup()
{
  // Serial.begin(9600);

  stepperMotor.begin();
  interface.begin();
  cutter.begin();

  cutter.setSpeed(stepperMotor.speed);

  // Show SPEED at start
  // interface.max7219.DisplayText("SPEED", 0);
  // delay(3000);
  interface.max7219.Clear();
  interface.floatDisplay(stepperMotor.speed);

  // These ensure the correct stepping mode of the drivers
  pinMode(DMODE_0, OUTPUT);
  pinMode(DMODE_1, OUTPUT);
  pinMode(DMODE_2, OUTPUT);
  digitalWrite(DMODE_0, LOW);
  digitalWrite(DMODE_1, LOW);
  digitalWrite(DMODE_2, HIGH);

  // Dylan Trying this
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // Button LED is on while the device is stopped
  pinMode(startButtLED, OUTPUT);
  digitalWrite(startButtLED, HIGH);
  // Serial.println("testing");
  // Serial.println(1234);
  //   cutter.start(); // Testing without the extruder on hand
}

void loop()
{
    interface.runControls();

    // If the interface has "signalled" if a change is needed, do the change
    if (interface.startSig)
    {
        stepperMotor.startStop();
        //Serial.println("beep");

        // Toggle the LED of the button to show if started or stopped
        if(stepperMotor.running)
        {
            digitalWrite(startButtLED, LOW);
            cutter.start();
        }
        else
        {
            digitalWrite(startButtLED, HIGH);
            cutter.stop();
        }
    }
    else if (interface.speedSig)
    {
        stepperMotor.incSpeed(interface.increment);
        interface.floatDisplay(stepperMotor.speed);
        cutter.setSpeed(stepperMotor.speed);
    }
    else if (interface.stopSig)
    {
        stepperMotor.stop();
        cutter.stop();
    }
    
   
    stepperMotor.run();
    cutter.run();

    // Run the animation if the motor is moving
    if(stepperMotor.running)
    {
        interface.goAnimation();
    }

}