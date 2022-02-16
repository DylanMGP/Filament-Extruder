#ifndef MotorDrive_h
#define MotorDrive_h

#include <Arduino.h>
#include <AccelStepper.h>

class MotorDrive
{
private:
    AccelStepper stepper;
public:
    MotorDrive(int motorInterfaceType, int stepPin, int dirPin);
    float stepPerMilimetre;
    bool running;
    float speed;
    void begin();
    void startStop();
    void stop();
    void incSpeed(float speedInc);
    void incSteps(float stepInc);
    void run();
};

#endif
