#include <Arduino.h>

#include <max7219.h>

#include <Button.h>

#include <Encoder.h>

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

MotorDrive stepperMotor = MotorDrive(motorInterfaceType, stepPin, dirPin, enbPin);

// Defining all the interface objects
Encoder myEnc(encPinA, encPinB);
long oldPosition = 0;
Button encButton(10);
Button startButton(9);
Button menuButton(8);

// Variables for stepper operation
const float increments[] = {0.1, 0.01, 0.001};
const int incSize = sizeof(increments) / sizeof(float);
int incPos = 0;

// Variables for configuration menu
bool inMenu = false;
bool showOption = true;
bool editing = false;
char options[][12] = {"CONTROL", "CALIBRATE"};
const int optionSize = 2;
int optionPos = 0;
char controls[][12] = {"SPEED", "CALLIPER"};
int controlMode = 0;

// animation parameters
const int waitTime = 100;
char frames[] = {0b1000000, 0b0100000, 0b0010000, 0b0001000, 0b0000100, 0b0000010};
int frameLength = sizeof(frames) / sizeof(char);
int frameIndex = 0;
unsigned long animTimerOld;

MAX7219 max7219;

// This converts a float into the form displayed by the max7219 and displays it
void floatDisplay(float num)
{
    String temp = String(num, 4);
    char toDisplay[5];
    temp.toCharArray(toDisplay, temp.length());
    max7219.DisplayText(toDisplay, 1);
}

// Returns the direction the encoder moved (1 or  -1). 0 if it didn't move.
int readEncoder()
{
    long newPosition = myEnc.read() / 4;
    if (newPosition != oldPosition)
    {
        int direction = newPosition - oldPosition;
        oldPosition = newPosition;
        return direction;
    }
    else
    {
        return 0;
    }
}

// Shows the menu option on the max7219
void menuDisplay()
{
    if (editing && (optionPos == 0))
    {
        max7219.DisplayText(controls[controlMode], 1);
    }
    else if (editing && (optionPos == 1))
    {
        floatDisplay(stepperMotor.stepPerMilimetre);
    }
    else
    {
        max7219.DisplayText(options[optionPos], 1);
    }
}

// Allow user to choose and edit menu functions with encoder wheel
void menuMove()
{
    int encDir = readEncoder();

    if (encDir != 0)
    {
        if (editing)
        {
            if (optionPos == 0)
            {
                controlMode = 1 - controlMode; // Toggle between speed control and calliper control
                                               // Serial.println(controlMode);
            }
            else if (optionPos == 1)
            {
                stepperMotor.stepPerMilimetre += encDir * increments[incPos]; // Adjust the steps per millimeter used for speed calcs
                                                                              // Serial.println(stepPerMilimetre);
            }
        }
        else
        {
            // Cycle through menu elements (allows for more than 2 elements)
            optionPos += encDir;

            if (optionPos < 0)
            {
                optionPos = optionSize - 1;
            }
            else if (optionPos >= optionSize)
            {
                optionPos = 0;
            }
        }

        max7219.Clear();
    }
}

void menuMode()
{
    menuDisplay();
    menuMove();

    // The menu button chooses an option to edit and confirms the new value
    if (menuButton.pressed())
    {
        editing = !editing;
        max7219.Clear();
    }

    // Start/stop button exits out of the menu
    if (startButton.pressed())
    {
        editing = false;
        inMenu = false;
        max7219.Clear();

        floatDisplay(stepperMotor.speed);
        // Serial.println("Running mode");
    }
}

// Handles the animation when the extruder is running
void goAnimation()
{
    unsigned long animTimer = millis();

    if (animTimer - animTimerOld >= waitTime)
    {
        animTimerOld = animTimer;

        frameIndex++;

        if (frameIndex < 0)
        {
            frameIndex = frameLength - 1;
        }
        else if (frameIndex >= frameLength)
        {
            frameIndex = 0;
        }

        max7219.MAX7219_Write(8, frames[frameIndex]);
    }
}

// Changes the increments when turning the encoder wheel with numerical values
void incChange()
{
    if (encButton.pressed())
    {
        if (incPos < (incSize - 1))
            incPos++;
        else
        {
            incPos = 0;
        }
        // Serial.println("New increment is: ");
        // Serial.println(increments[incPos], 4);
    }
}

// Changes the speed of the steppers with the encoder wheel
void changeSpeed()
{
    int encDir = readEncoder();
    if (encDir != 0)
    {
        max7219.Clear();

        stepperMotor.setSpeed(stepperMotor.speed + encDir * increments[incPos]);
        // Serial.println(speed, 4);

        floatDisplay(stepperMotor.speed);
    }
}

void setup()
{
    Serial.begin(9600);

    // Start up the stepper, disable it at start
    stepperMotor.begin();

    // Initialise interface elements
    encButton.begin();
    startButton.begin();
    menuButton.begin();
    max7219.Begin();

    floatDisplay(stepperMotor.speed);
}

void loop()
{
    incChange();

    if (inMenu)
    {
        menuMode();
    }
    else if (startButton.pressed())
    {
        stepperMotor.startStop();
    }
    else
    {
        changeSpeed();
    }

    // menu button enters the menu
    if (menuButton.pressed())
    {
        showOption = true;
        inMenu = true;
        stepperMotor.stop();
        max7219.MAX7219_Write(8, 0b0000000);
    }

    if (stepperMotor.running)
    {
        goAnimation();
    }
    stepperMotor.run();
}