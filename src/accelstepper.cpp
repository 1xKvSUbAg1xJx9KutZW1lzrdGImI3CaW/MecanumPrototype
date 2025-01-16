#include "accelstepper.h"

accelstepper::accelstepper(int pinStep, int pinDir, bool reverse) {
    mDirPin = pinDir;
    mStepPin = pinStep;
    mDir = reverse;
    
    pinMode(mDirPin, OUTPUT);
    pinMode(mStepPin, OUTPUT);

    pMotor = new motor(pinStep, pinDir, reverse);
}

accelstepper::~accelstepper() {
    delete pMotor;
}

void accelstepper::setTargetRpm(float rpm) {
    //mTargetStepInterval = (10e6) / (abs(rpm) * FULL_ROTATION_STEPS);
    mTargetStepInterval = 1e6 / (FULL_ROTATION_STEPS * abs(rpm) / 60.0);
    mTargetRpm = rpm;
}

bool accelstepper::step() {
    if(mTargetStepInterval == 0)
        return false;
    unsigned long curr = micros();
    if(curr - mLastRun < mTargetStepInterval / 2)
        pMotor->deenergize();
    if(curr - mLastRun < mTargetStepInterval)
        return false;
    bool forward = mTargetRpm > 0;
    pMotor->energize(forward);
    mLastRun = curr;
    return true;
}