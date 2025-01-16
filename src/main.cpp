#include <Arduino.h>
#include "motor.h"
#include "accelstepper.h"

#define sgn(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))

typedef struct Vec2 {
	float X, Y;
} Vec2;

float dot(const Vec2 &lhs, const Vec2 &rhs) {
	return lhs.X * rhs.X + lhs.Y * rhs.Y;
}

float length(const Vec2 &vec) {
	return sqrt(vec.X * vec.X + vec.Y * vec.Y);
}

Vec2 normalize(const Vec2 &vec) {
	float len = length(vec);
	return { vec.X / len, vec.Y / len };
}

void linearTest(const Vec2 &command);
void translate(Vec2 linearCommand, float angleCommand);

Vec2 motorPositions[] = { {-73.0f, 52.0f}, {73.0f, 52.0f}, {-73.0f, -52.0f}, {73.0f, -52.0f} };
accelstepper motors[] = { accelstepper(1, 0, false), accelstepper(3, 2, true), accelstepper(4, 5, true), accelstepper(7, 6, true) };

void setup() {
    while (!Serial);

    delay(500);
    
	Serial.begin(9600);

    linearTest({ 50, 0 });
    delay(2500);
    linearTest({ -50, 0 });
}

void translateLinear(Vec2 command) {
	int nMotors = sizeof(motors) / sizeof(accelstepper);

	for(int i = 0; i < nMotors; i++) {
		float shaftCommand = dot(normalize(command), normalize({ -motorPositions[i].X, -motorPositions[i].Y }));
		shaftCommand *= dot(normalize(motorPositions[i]), { 0.0f, -1.0f });
		shaftCommand = max(min(shaftCommand, 1.0f), -1.0f);
		motors[i].setTargetRpm(120 * shaftCommand);
	}
}

void translateAngle(float command) {
    command = max(min(command, 1.0f), -1.0f);
    int nMotors = sizeof(motors) / sizeof(accelstepper);

	for(int i = 0; i < nMotors; i++) {
		float shaftCommand = dot(normalize(motorPositions[i]), { -1.0f, .00f }) * command;
		shaftCommand = max(min(shaftCommand, 1.0f), -1.0f);
		motors[i].setTargetRpm(120 * shaftCommand);
	}
}

void translate(Vec2 linearCommand, float angleCommand) {
    int nMotors = sizeof(motors) / sizeof(accelstepper);
    angleCommand = max(min(angleCommand, 1.0f), -1.0f);

    for(int i = 0; i < nMotors; i++) {
		float linearShaftCommand = dot(normalize(linearCommand), normalize({ -motorPositions[i].X, -motorPositions[i].Y }));
		linearShaftCommand *= dot(normalize(motorPositions[i]), { 0.0f, -1.0f });
		linearShaftCommand = max(min(linearShaftCommand, 1.0f), -1.0f);
        if(linearShaftCommand != linearShaftCommand)
            linearShaftCommand = 0;

        float angularShaftCommand = dot(normalize(motorPositions[i]), { -1.0f, .00f }) * angleCommand;
		angularShaftCommand = max(min(angularShaftCommand, 1.0f), -1.0f);
        if(angularShaftCommand != angularShaftCommand)
            angularShaftCommand = 0;

        float mixed = (linearShaftCommand + angularShaftCommand) / 2.0;

		motors[i].setTargetRpm(30.0 * mixed);
	}
}

void linearTest(const Vec2 &destPosition) {
    int nMotors = sizeof(motors) / sizeof(accelstepper);
    Vec2 currentPosition = { 0, 0 };
    auto dirVec = normalize(destPosition);
    translate(dirVec, 0);
    while (abs(currentPosition.X - destPosition.X) > .5 || abs(currentPosition.Y - destPosition.Y) > .5)
    {
        dirVec = normalize({ destPosition.X - currentPosition.X, destPosition.Y - currentPosition.Y });
        translate(dirVec, 0);

        for(int i = 0; i < nMotors; i++) {
            if(motors[i].step())
            {
                currentPosition.X += (dirVec.X * 2.0 * PI * 40.0)/FULL_ROTATION_STEPS/4.0;
                currentPosition.Y += (dirVec.Y * 2.0 * PI * 40.0)/FULL_ROTATION_STEPS/4.0;
            }
        }
    }

    translate({ 0, 0 }, 0);
}

void loop() {
    return;
    int nMotors = sizeof(motors) / sizeof(accelstepper);
	Vec2 command = { 1, 1 };
	command = normalize(command);
    // translateAngle(sin(millis() / 1000.0f));
    translate({ 1, 1 }, 1);
    //translateAngle(1);

	for(int i = 0; i < nMotors; i++)
		motors[i].step();
}