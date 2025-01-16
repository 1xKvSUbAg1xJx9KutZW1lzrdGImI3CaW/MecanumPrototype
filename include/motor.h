#pragma once
#include <Arduino.h>

class motor
{
private:
    int mStepPin;
    int mDirPin;
    bool mDir;

public:
    motor(int pinStep, int pinDir, bool reverse);
    void energize(bool forward);
    void deenergize();
};