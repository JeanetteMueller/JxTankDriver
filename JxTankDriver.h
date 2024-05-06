
#ifndef JxTankDriver_h
#define JxTankDriver_h

#include "Arduino.h"
#include <CytronMotorDriver.h>
#include <Adafruit_PWMServoDriver.h>

class JxTankDriver
{
public:

    enum MotorType {
        Direct, PWMServoDriver
    };

    int16_t currentSpeedLeft() const { return _speedLeft; };
    int16_t currentSpeedRight() const { return _speedRight; };

    JxTankDriver(int16_t inputMin, int16_t inputMax, int16_t inputCenter, uint32_t analogFrequenc = 18000);

    void setupLeftMotor(MODE mode, uint8_t pin1, uint8_t pin2);
    void setupLeftMotor(Adafruit_PWMServoDriver pwm, uint8_t pinDir, uint8_t pinPwm);

    void setupRightMotor(MODE mode, uint8_t pin1, uint8_t pin2);
    void setupRightMotor(Adafruit_PWMServoDriver pwm, uint8_t pinDir, uint8_t pinPwm);

    void updateMotorsWith(int16_t horizontalValue, int16_t verticalValue, uint16_t deadPoint, int16_t maxSpeed = 255);
    
    int16_t getCurrentSpeedLeft();
    int16_t getCurrentSpeedRight();
    void stop();

private:
    int16_t _inputMin = -500;
    int16_t _inputMax = 500;
    int16_t _inputCenter = 0;

    MotorType _currentMotorTypeLeft;
    CytronMD *_motorLeft;
    bool _isReadyLeft = false;
    Adafruit_PWMServoDriver _pwmLeft;
    uint8_t _pinDirLeft;
    uint8_t _pinPwmLeft;
    int16_t _speedLeft = 0;

    MotorType _currentMotorTypeRight;
    CytronMD *_motorRight;
    bool _isReadyRight = false;
    Adafruit_PWMServoDriver _pwmRight;
    uint8_t _pinDirRight;
    uint8_t _pinPwmRight;
    int16_t _speedRight = 0;

    void updateSpeed(uint16_t deadPoint, int16_t maxSpeed);
    void updateSpeedLeft(uint16_t deadPoint, int16_t maxSpeed);
    void updateSpeedRight(uint16_t deadPoint, int16_t maxSpeed);
};

#endif