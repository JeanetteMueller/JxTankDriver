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
    if (_isReadyLeft != true)
    {
        _motorLeft = new CytronMD(mode, pin1, pin2);
        _speedLeft = 0;
        _motorLeft->setSpeed(_speedLeft);
        _isReadyLeft = true;
    }
}
void JxTankDriver::setupLeftMotor(Adafruit_PWMServoDriver pwm, uint8_t pinDir, uint8_t pinPwm)
{
    if (_isReadyLeft != true)
    {
        _pwmLeft = pwm;
        _pinDirLeft = pinDir;
        _pinPwmLeft = pinPwm;

        _currentMotorTypeLeft = PWMServoDriver;
        _isReadyLeft = true;
    }
}

void JxTankDriver::setupRightMotor(MODE mode, uint8_t pin1, uint8_t pin2)
{
    if (_isReadyRight != true)
    {
        _motorRight = new CytronMD(mode, pin1, pin2);
        _speedRight = 0;
        _motorRight->setSpeed(_speedRight);
        _isReadyRight = true;
    }
}
void JxTankDriver::setupRightMotor(Adafruit_PWMServoDriver pwm, uint8_t pinDir, uint8_t pinPwm)
{
    if (_isReadyRight != true)
    {
        _pwmRight = pwm;
        _pinDirRight = pinDir;
        _pinPwmRight = pinPwm;

        _currentMotorTypeRight = PWMServoDriver;
        _isReadyRight = true;
    }
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
        joystickY = map(vertical, _inputMin, _inputMax, -maxSpeed, maxSpeed);

        // Convert joystick values to motor speeds
        leftMotorSpeedTarget += joystickY;
        rightMotorSpeedTarget += joystickY;
    }

    if (horizontal < _inputCenter - deadPoint || horizontal > _inputCenter + deadPoint)
    {
        joystickX = map(horizontal, _inputMin, _inputMax, -maxSpeed, maxSpeed);

        leftMotorSpeedTarget += joystickX;
        rightMotorSpeedTarget -= joystickX;
    }

    // Make sure motor speeds are within range
    _speedLeft = constrain(leftMotorSpeedTarget, -(maxSpeed), maxSpeed);
    _speedRight = constrain(rightMotorSpeedTarget, -(maxSpeed), maxSpeed);

    // Serial.print("updateMotorsWith Left ");
    // Serial.print(leftMotorSpeedTarget);
    // Serial.print(" Right ");
    // Serial.print(rightMotorSpeedTarget);

    // Serial.print("               Speed Left ");
    // Serial.print(_speedLeft);
    // Serial.print(" Right ");
    // Serial.println(_speedRight);

    updateSpeed();
}

int16_t JxTankDriver::getCurrentSpeedLeft()
{
    return _speedLeft;
}
int16_t JxTankDriver::getCurrentSpeedRight()
{
    return _speedRight;
}

void JxTankDriver::stop()
{
    _speedLeft = 0;
    _speedRight = 0;
    updateSpeed();
}

void JxTankDriver::updateSpeed()
{
    updateSpeedLeft();
    updateSpeedRight();
}

void JxTankDriver::updateSpeedLeft()
{
    if (_currentMotorTypeLeft == Direct)
    {
        updateSpeedByDirect(_motorLeft, _speedLeft);
    }
    else if (_currentMotorTypeLeft == PWMServoDriver)
    {
        updateSpeedByPWM(_pwmLeft, _pinDirLeft, _pinPwmLeft, _speedLeft);
    }
}

void JxTankDriver::updateSpeedRight()
{
    if (_currentMotorTypeRight == Direct)
    {
        updateSpeedByDirect(_motorRight, _speedRight);
    }
    else if (_currentMotorTypeRight == PWMServoDriver)
    {
        updateSpeedByPWM(_pwmRight, _pinDirRight, _pinPwmRight, _speedRight);
    }
}

void JxTankDriver::updateSpeedByDirect(CytronMD *motor, int16_t speed)
{
    motor->setSpeed(speed);
}

void JxTankDriver::updateSpeedByPWM(Adafruit_PWMServoDriver pwm, uint8_t dirPin, uint8_t pwmPin, int16_t speed)
{
    if (speed > 0)
    {
        uint16_t setSpeed = map(speed, 0, 255, 0, 4095);
        pwm.setPWM(dirPin, 0, 4095);
        pwm.setPWM(pwmPin, 0, setSpeed);
    }
    else if (speed < 0)
    {
        uint16_t setSpeed = map(speed, -255, 0, 4095, 0);
        pwm.setPWM(dirPin, 0, 0);
        pwm.setPWM(pwmPin, 0, setSpeed);
    }
    else
    {
        pwm.setPWM(dirPin, 0, 0);
        pwm.setPWM(pwmPin, 0, 0);
    }
}