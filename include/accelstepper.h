#pragma once

#include "motor.h"

#define SUB_STEP_SIZE 8.0
#define STEP_SIZE (1.8 / SUB_STEP_SIZE)
#define FULL_ROTATION_STEPS (360.0 / STEP_SIZE)

class accelstepper
{
private:
    int mStepPin;
    int mDirPin;
    float mTargetRpm = 0;
    bool mDir;
    unsigned long mLastRun = 0;
    int mTargetStepInterval;

    motor *pMotor;
public:
    bool step();
    void setTargetRpm(float rpm);

    accelstepper(int pinStep, int pinDir, bool reverse);
    ~accelstepper();
};