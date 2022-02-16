#ifndef interface_h
#define interface_h

#include <Arduino.h>
#include <Button.h>
#include <max7219.h>
#include <Encoder.h>

/*
    interface.h - Class which handles all the behaviours of the interface.
*/

class Interface
{
private:
    Encoder myEnc;
    long oldPosition;
    const float increments[3];
    const int incSize;
    int incPos;
    const int waitTime;
    char frames[6];
    int frameLength;
    int frameIndex;
    unsigned long animTimerOld;
    MAX7219 max7219;
public:
    Interface(uint8_t encAPin, uint8_t encBPin, uint8_t encButtonPin, uint8_t startPin);
    Button encButton;
    Button startButton;
    void begin();
    void incChange();
    int readEncoder();
    void floatDisplay(float num);
    void goAnimation();
    bool startSig;
    bool speedSig;
    bool stepSig;
    bool stopSig;
    float increment;
    void runControls();
};

#endif