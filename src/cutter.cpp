#include <Arduino.h>
#include <Servo.h>
#include <cutter.h>

Cutter::Cutter(int selectedPin, unsigned long interval, int length){
    Servo myservo;
    servoPin = selectedPin;
    checkingInterval = interval;
    filamentLength = length;

}

void Cutter::begin(){
    myservo.attach(servoPin);
    myservo.write(160);
    extrudeSpeed = 5.80f;
    filamentRemaining = (float)filamentLength;
    // prevCheckTime = millis();
}

// Call this whenever the extruder speed changes to update the speed
void Cutter::setSpeed(float speed){
    extrudeSpeed = speed;
}

void Cutter::setLength(int length){
    filamentRemaining += (float)(length - filamentLength); // Update the remaining extrusion distance to the new length
    filamentLength = length;
}

// Starting the cutter resets the monitored length. Make sure you cut off any excess!
void Cutter::start(){
    running = true;
    filamentRemaining = (float)filamentLength;
    prevCheckTime = millis();
}

void Cutter::stop(){
    running = false;
}

// Tells the cutter to do a cut, you could call this on its own if you need a manual cut
void Cutter::cutFilament(){
    cut = true;
}

// Runs the cutting operation. delay between close and open is non-blocking, so it shouldnt affect the stepper timings
void Cutter::runCut(){
    if(cut){
      unsigned long currCutTime = millis();

      if (!cutterOpen) {
        myservo.write(180);
        prevCutTime = currCutTime;
        cutterOpen = true;
        // return the cutter after a small, non-blocking delay
      } else if (currCutTime - prevCutTime > 500) {
        myservo.write(160);
        cutterOpen = false;
        cut = false;
      }
    }
}

// General function that runs the cutter, put in the main loop
void Cutter::run(){
    if(running){
        unsigned long currCheckTime = millis();

        // Check and update extruded length on the interval we set
        if((currCheckTime - prevCheckTime) > checkingInterval){
          // mm/s * s = mm
          filamentRemaining -= extrudeSpeed * (checkingInterval / 1000.0f);
          //   Serial.print("Checked at: ");
          //   Serial.println(filamentRemaining);

          prevCheckTime = currCheckTime;

          // When it's long enough, cut it
          if (filamentRemaining < 0) {
            Cutter::cutFilament();
            filamentRemaining = (float)filamentLength;
          }
        }
    }

    Cutter::runCut();
}
