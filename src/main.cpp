#include <Arduino.h>

#include <max7219.h>

#include <Button.h>

#include <Encoder.h>

#include <AccelStepper.h>
#include <MultiStepper.h>

#include <MotorDrive.h>
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