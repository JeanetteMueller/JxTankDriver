#include "JxTankDriver.h"

JxTankDriver::JxTankDriver(int16_t inputMin, int16_t inputMax, int16_t inputCenter, uint32_t analogFrequency)
{
    analogWriteFreq(analogFrequency);

    _inputMin = inputMin;
    _inputMax = inputMax;
    _inputCenter = inputCenter;
}

void JxTankDriver::setupLeftMotor(MODE mode, uint8_t pin1, uint8_t pin2)
{
    _motorLeft = new CytronMD(mode, pin1, pin2);
    _speedLeft = 0;
    _motorLeft->setSpeed(_speedLeft);
}
void JxTankDriver::setupRightMotor(MODE mode, uint8_t pin1, uint8_t pin2)
{
    _motorRight = new CytronMD(mode, pin1, pin2);
    _speedRight = 0;
    _motorRight->setSpeed(_speedRight);
}

void JxTankDriver::updateMotorsWith(int16_t horizontalValue, int16_t verticalValue, uint16_t deadPoint, int16_t maxSpeed)
{   
    int16_t horizontal = constrain(horizontalValue, _inputMin, _inputMax);
    int16_t vertical = constrain(verticalValue, _inputMin, _inputMax);

    int16_t leftMotorSpeedTarget = 0;
    int16_t rightMotorSpeedTarget = 0;

    int16_t joystickX = 0;
    int16_t joystickY = 0;

    if (vertical < _inputCenter - deadPoint || vertical > _inputCenter + deadPoint)
    {
        joystickY = map(vertical, _inputMin, _inputMax, maxSpeed, -(maxSpeed));

        // Convert joystick values to motor speeds
        leftMotorSpeedTarget += joystickY;
        rightMotorSpeedTarget += joystickY;
    }

    if (horizontal < _inputCenter - deadPoint || horizontal > _inputCenter + deadPoint)
    {
        joystickX = map(horizontal, _inputMin, _inputMax, maxSpeed, -(maxSpeed));

        leftMotorSpeedTarget += joystickX;
        rightMotorSpeedTarget -= joystickX;
    }

    // Make sure motor speeds are within range
    _speedLeft = constrain(leftMotorSpeedTarget, -(maxSpeed), maxSpeed);
    _speedRight = constrain(rightMotorSpeedTarget, -(maxSpeed), maxSpeed);

    updateSpeed();
}

void JxTankDriver::stop() {
    _speedLeft = 0;
    _speedRight = 0;
    updateSpeed();
}

void JxTankDriver::updateSpeed() {
    _motorLeft->setSpeed(_speedLeft);
    _motorRight->setSpeed(_speedRight);
}