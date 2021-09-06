#include <Arduino.h>
#include <AccelStepper.h>
#include <MotorDrive.h>

MotorDrive::MotorDrive(int motorInterfaceType, int stepPin, int dirPin, int enbPin)
{
    stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
    stepper.setEnablePin(enbPin);
}
void MotorDrive::begin()
{
    stepPerMilimetre = 34.00;
    running = false;
    speed = 3.00;
    stepper.setMaxSpeed(4000);
    stepper.setSpeed(speed * stepPerMilimetre);
    stepper.disableOutputs();
}
void MotorDrive::startStop()
{
    running = !running;
    if (running)
    {
        stepper.enableOutputs();
        stepper.setSpeed(speed * stepPerMilimetre);
    }
    else
    {
        stepper.disableOutputs();
    }
}
void MotorDrive::incSpeed(float speedInc)
{
    speed += speedInc;
    stepper.setSpeed(speed * stepPerMilimetre);
}
void MotorDrive::incSteps(float stepInc)
{
    stepPerMilimetre += stepInc;
}
void MotorDrive::stop()
{
    stepper.disableOutputs();
    running = false;
}
void MotorDrive::run()
{
    if(running)
    {
        stepper.runSpeed();
    }
}
