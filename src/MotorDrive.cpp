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
void MotorDrive::calibrate(float newStepPerMilimetre)
{
    stepPerMilimetre = newStepPerMilimetre;
}
void MotorDrive::setSpeed(float newSpeed)
{
    speed = newSpeed;
    stepper.setSpeed(speed * stepPerMilimetre);
}
void MotorDrive::run()
{
    if(running)
    {
        stepper.runSpeed();
    }
}
