#ifndef topUp_h
#define topUp_h

#include <Arduino.h>
#include <Servo.h>
#include <AccelStepper.h>
#include <Wire.h>

class TopUp
{
private:
float runningMass;
float desiredDumpSize;
float dumpSize;
unsigned long checkingInterval;
unsigned long prevCheckTime;
float extrudedDiameter;
float extrudeSpeed;
float density;
bool running;
bool dumping;

public:
TopUp(int stepPin, int dirPin, int servoDoorPin, int servoDumpPin, TwoWire &WirePort);
void begin(unsigned long interval, float diameter, float density, float dumpSize);
void run();
float massExtruded(float extrudedDiameter, float extrudeSpeed, unsigned long checkingInterval, float density);
void runDump();
float readScale();
void runScale();
void start();
void stop();
};

#endif