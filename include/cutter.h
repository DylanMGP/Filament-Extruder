#ifndef cutter_h
#define cutter_h

#include <Arduino.h>
#include <Servo.h>

class Cutter
{
private:
    Servo myservo;
    int servoPin;
    unsigned long checkingInterval;
    unsigned long prevCheckTime;
    float extrudeSpeed;
    unsigned long prevCutTime;
    
    void runCut();
public:
    int filamentLength;
    float filamentRemaining;
    bool cut;
    bool cutterOpen;
    bool running;

    Cutter(int selectedPin, unsigned long interval, int length);
    void begin();
    void setSpeed(float speed);
    void setLength(int length);
    void cutFilament();
    void start();
    void stop();
    void run();
};

#endif