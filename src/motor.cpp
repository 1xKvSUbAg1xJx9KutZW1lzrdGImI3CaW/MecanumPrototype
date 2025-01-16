#include "motor.h"

motor::motor(int pinStep, int pinDir, bool reverse)
{
    mDirPin = pinDir;
    mStepPin = pinStep;
    mDir = reverse;
}

void motor::energize(bool forward) {
    digitalWrite(mStepPin, HIGH);
    if(mDir)
        forward = !forward;
    digitalWrite(mDirPin, forward);
}

void motor::deenergize() {
    digitalWrite(mStepPin, LOW);
}