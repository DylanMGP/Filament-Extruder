#ifndef interface_h
#define interface_h

#include <Arduino.h>
#include <Button.h>
#include <max7219.h>
#include <Encoder.h>

class Interface
{
private:
    Encoder myEnc;
    long oldPosition;
    const float increments[3];
    const int incSize;
    int incPos;
    bool inMenu;
    bool editing;
    char options[2][12];
    char controls[2][12];
    int optionPos;
    const int optionSize;
    int controlMode;
    MAX7219 max7219;
    const int waitTime;
    char frames[6];
    int frameLength;
    int frameIndex;
    unsigned long animTimerOld;
public:
    Interface(uint8_t encAPin, uint8_t encBPin, uint8_t encButtonPin, uint8_t startPin, uint8_t menuPin);
    Button encButton;
    Button startButton;
    Button menuButton;
    void begin();
    void incChange();
    int readEncoder();
    void floatDisplay(float num);
    void goAnimation();
    bool startSig;
    bool speedSig;
    bool stepSig;
    float increment;
    void runControls();
};

#endif