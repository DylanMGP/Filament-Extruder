#include <Arduino.h>
#include <interface.h>
#include <Button.h>
#include <max7219.h>
#include <Encoder.h>

Interface::Interface(uint8_t encAPin, uint8_t encBPin, uint8_t encButtonPin, uint8_t startPin)
    : myEnc(encAPin, encBPin)
    , encButton(encButtonPin)
    , startButton(startPin)
    , oldPosition(0)
    , increments({0.1F, 0.01F, 0.001F})
    , incSize(sizeof(increments) / sizeof(float))
    , incPos(0)
    , waitTime(100)
    , frames({0b1000000, 0b0100000, 0b0010000, 0b0001000, 0b0000100, 0b0000010})
    , frameLength(sizeof(frames) / sizeof(char))
    , frameIndex(0)
{};
// Initializes the libraries for buttons, encoder and LED display.
void Interface::begin()
{
    encButton.begin();
    startButton.begin();
    max7219.Begin();
}
// Changes the increment of the encoder if the encoder button is pressed.
void Interface::incChange()
{
    if (encButton.pressed())
    {
        if (incPos < (incSize - 1)) incPos++;
        else incPos = 0;
    }
}
// Reads the state of the encoder. Returns 1 or -1 if moved in certain direction. Returns 0 if not.
int Interface::readEncoder()
{
    long newPosition = myEnc.read() / 4;
    if (newPosition != oldPosition)
    {
        int direction = newPosition - oldPosition;
        oldPosition = newPosition;
        return direction;
    }
    else return 0;
}
// Formats a float into compatible Char array and displays with the MAX7219 library.
void Interface::floatDisplay(float num)
{
    String temp = String(num, 4);
    char toDisplay[5];
    temp.toCharArray(toDisplay, temp.length());
    max7219.DisplayText(toDisplay, 1);
}
// Runs the "motor moving" animation
void Interface::goAnimation()
{
    unsigned long animTimer = millis();

    if (animTimer - animTimerOld >= waitTime)
    {
        animTimerOld = animTimer;

        frameIndex++;

        if (frameIndex >= frameLength) frameIndex = 0;

        max7219.MAX7219_Write(8, frames[frameIndex]);
    }
}
// TODO: This is where the menu needs to be removed.
// Handles all of the interface functions. Place in void loop().
void Interface::runControls()
{
    // These booleans "signal" to other classes if something needs to be done.
    startSig = false;   // Start/toggle motor
    speedSig = false;   // Change Speed
    stopSig  = false;   // Stop motor
    // This is the increment to be applied to other classes. (E.g for MotorDrive::incSpeed() or MotorDrive::incSteps())
    increment = 0;

    Interface::incChange();

    int encDir = Interface::readEncoder();
    // If the encoder has moved, edit some value
    if (encDir != 0)
    {
            // If we arent in the menu, signal that speed needs to be changed clear display ready for new value
            speedSig = true;
            increment = increments[incPos] * encDir;
            max7219.Clear();
    }
    else
    {
        // If encoder hasn't moved, check if any buttons are pressed
        // Out of the menu, the start button toggles the motors
        if (startButton.pressed())
        {
            startSig = true;
        }
    }
}