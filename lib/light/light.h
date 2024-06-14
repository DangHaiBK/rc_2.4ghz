#ifndef LIGHT_H
#define LIGHT_H

#include <Arduino.h>

class Light
{
public:
    Light(
        int pin_rooftop_left_1,
        int pin_rooftop_left_2,
        int pin_rooftop_right_1,
        int pin_rooftop_right_2,
        int pin_beacon_left,
        int pin_beacon_right,
        int pin_signal_left_front,
        int pin_signal_left_back,
        int pin_signal_right_front,
        int pin_signal_right_back,
        int pin_rect_left_front,
        int pin_circle_left_front,
        int pin_rect_right_front,
        int pin_circle_right_front,
        int pin_brake_left,
        int pin_brake_right,
        int pin_reversed_left,
        int pin_reversed_right,
        int pin_smoke_left_1,
        int pin_smoke_left_2,
        int pin_smoke_right_1,
        int pin_smoke_right_2
    );

    void begin();

    void ForwardOn();
    void ForwardOff();

    void ReversedOn();
    void ReversedOff();

    void RooftopOn();
    void RooftopOff();

    void SmokeOn();
    void SmokeOff();

    void BeaconSignalInit();
    void BeaconSignal(uint16_t time_period, uint16_t constant_time);

    void LightSignalInit();
    void LeftSignal(uint16_t time_period, uint16_t constant_time);
    void RightSignal(uint16_t time_period, uint16_t constant_time);

    void BrakeOff();
    void BrakeActive(uint16_t time_period, uint16_t constant_time);
    void HazardSignal(uint16_t time_period, uint16_t constant_time);

private:
    int _rtLeftPin1;
    int _rtLeftPin2;
    int _rtRightPin1;
    int _rtRightPin2;

    int _bcLeftPin;
    int _bcRightPin;

    int _fLeftRectPin;
    int _fRightRectPin;
    int _fLeftCirclePin;
    int _fRightCirclePin;

    int _brLeftPin;
    int _brRightPin;

    int _rLeftPin;
    int _rRightPin;

    int _sLeftFrontPin;
    int _sRightFrontPin;
    int _sLeftBackPin;
    int _sRightBackPin;

    int _smkLeftPin1;
    int _smkLeftPin2;
    int _smkRightPin1;
    int _smkRightPin2;
};

#endif /* LIGHT_H */