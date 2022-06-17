#include <Arduino.h>
#include <AccelStepper.h>
#include <MotorDrive.h>
/*
    MotorDrive.h - Class which handles all the behaviors of the stepper motors.
*/
MotorDrive::MotorDrive(int motorInterfaceType, int stepPin, int dirPin, int enablePin)
{
    stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
    stepper.setEnablePin(enablePin);
    }
// Initialises the behaviour of the stepper motors. Sets the initial and max speed and turns motors off.
void MotorDrive::begin()
{
    stepPerMilimetre = 34.00;
    running = false;
    speed = 5.80; // in mm/s
    stepper.setMaxSpeed(4000);
    stepper.setSpeed(speed * stepPerMilimetre);
    stepper.disableOutputs();
}
// Toggles the if the motor is running or stopped.
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
        stepper.enableOutputs();//stepper.disableOutputs();          TEST
    }
}
// Increments the speed at which the motors are running and sets the new speed. Input is the float increment to add.
void MotorDrive::incSpeed(float speedInc)
{
    speed += speedInc;
    stepper.setSpeed(speed * stepPerMilimetre);
}
// Increments the steps per milimetre used to calculate motor speed. Used for calibration. Input is the float increment to add.
void MotorDrive::incSteps(float stepInc)
{
    stepPerMilimetre += stepInc;
}
// Stops the motor
void MotorDrive::stop()
{
    stepper.enableOutputs();//stepper.disableOutputs();               TEST
    running = false;
}
void MotorDrive::run()
{
    if(running)
    {
        stepper.runSpeed();
    }
}
