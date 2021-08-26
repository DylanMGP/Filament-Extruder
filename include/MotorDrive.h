#ifndef MotorDrive_h
#define MotorDrive_h

#include <Arduino.h>
#include <AccelStepper.h>

class MotorDrive
{
private:
    AccelStepper stepper;
public:
    MotorDrive(int motorInterfaceType, int stepPin, int dirPin, int enbPin);
    void begin();
    void startStop();
    void calibrate(float newStepPerMilimetre);
    void setSpeed(float newSpeed);
    void run();

    float stepPerMilimetre;
    bool running;
    float speed;
};

#endif
