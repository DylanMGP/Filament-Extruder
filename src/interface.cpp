#include <Arduino.h>
#include <interface.h>
#include <Button.h>
#include <max7219.h>
#include <Encoder.h>

Interface::Interface(uint8_t encAPin, uint8_t encBPin, uint8_t encButtonPin, uint8_t startPin, uint8_t menuPin)
    : myEnc(encAPin, encBPin)
    , encButton(encButtonPin)
    , startButton(startPin)
    , menuButton(menuPin)
    , increments({0.1F, 0.01F, 0.001F})
    , incSize(sizeof(increments) / sizeof(float))
    , incPos(0)
    , oldPosition(0)
    , inMenu(false)
    , editing(false)
    , options({"CONTROL", "CALIBRATE"})
    , controls({"SPEED", "CALLIPER"})
    , optionPos(0)
    , optionSize(2)
    , controlMode(0)
    , waitTime(100)
    , frames({0b1000000, 0b0100000, 0b0010000, 0b0001000, 0b0000100, 0b0000010})
    , frameLength(sizeof(frames) / sizeof(char))
    , frameIndex(0)
{};
void Interface::begin()
{
    encButton.begin();
    startButton.begin();
    menuButton.begin();
    max7219.Begin();
}
void Interface::incChange()
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
int Interface::readEncoder()
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
void Interface::floatDisplay(float num)
{
    String temp = String(num, 4);
    char toDisplay[5];
    temp.toCharArray(toDisplay, temp.length());
    max7219.DisplayText(toDisplay, 1);
}
void Interface::goAnimation()
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
void Interface::runControls()
{
    startSig = false;
    speedSig = false;
    stepSig = false;
    increment = 0;
    Interface::incChange();
    int encDir = Interface::readEncoder();
    if (encDir != 0)
    {
        if (inMenu)
        {
            if (editing)
            {
                if (optionPos == 0)
                {
                    controlMode = 1 - controlMode; // Toggle between speed control and calliper control
                    max7219.Clear();
                    max7219.DisplayText(controls[controlMode], 1);
                }
                else if (optionPos == 1)
                {
                    stepSig = true;
                    increment = increments[incPos] * encDir; // Adjust the steps per millimeter used for speed calcs
                    max7219.Clear();
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
                max7219.Clear();
                max7219.DisplayText(options[optionPos], 1);
            }
        }
        else
        {
            speedSig = true;
            increment = increments[incPos] * encDir;
            max7219.Clear();
        }
    }
    else
    {
        if (inMenu)
        {
            if (menuButton.pressed())
            {
                editing = !editing;
                max7219.Clear();
                if(editing && (optionPos == 0))
                {
                    max7219.DisplayText(controls[controlMode], 1);
                }
                else if (editing && (optionPos == 1))
                {
                    stepSig = true;
                }
                else
                {
                    max7219.DisplayText(options[optionPos], 1);
                }
                
            }
            if (startButton.pressed())
            {
                editing = false;
                inMenu = false;
                speedSig = true;
                max7219.Clear();
            }
        }
        else
        {
            if (startButton.pressed())
            {
                startSig = true;
            }
            else if (menuButton.pressed())
            {
                inMenu = true;
                max7219.Clear();
                max7219.DisplayText(options[optionPos], 1);
            }
        }
    }
}