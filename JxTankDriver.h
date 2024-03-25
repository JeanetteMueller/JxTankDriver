
#ifndef JxTankDriver_h
#define JxTankDriver_h

#include "Arduino.h"
#include <CytronMotorDriver.h>

class JxTankDriver
{
public:
    int16_t currentSpeedLeft() const { return _speedLeft; };
    int16_t currentSpeedRight() const { return _speedRight; };

    JxTankDriver(int16_t inputMin, int16_t inputMax, int16_t inputCenter, uint32_t analogFrequenc = 18000);

    void setupLeftMotor(MODE mode, uint8_t pin1, uint8_t pin2);
    void setupRightMotor(MODE mode, uint8_t pin1, uint8_t pin2);

    void updateMotorsWith(int16_t horizontalValue, int16_t verticalValue, uint16_t deadPoint, int16_t maxSpeed = 255);
    void stop();

private:
    int16_t _inputMin = -500;
    int16_t _inputMax = 500;
    int16_t _inputCenter = 0;

    int16_t _speedLeft = 0;
    int16_t _speedRight = 0;

    CytronMD *_motorLeft;
    CytronMD *_motorRight;

    void updateSpeed();
};

#endif