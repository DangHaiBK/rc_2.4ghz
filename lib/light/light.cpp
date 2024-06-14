#include "light.h"

/* 
    * Call constructor to assign pins  
*/
Light::Light(
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
    )
{
    // Front lights
    _fLeftRectPin = pin_rect_left_front;
    _fLeftCirclePin = pin_circle_left_front;
    _fRightRectPin = pin_rect_right_front;
    _fRightCirclePin = pin_circle_right_front;

    // Beacon lights
    _bcLeftPin = pin_beacon_left;
    _bcRightPin = pin_beacon_right;

    // Rooftop lights
    _rtLeftPin1 = pin_rooftop_left_1;
    _rtLeftPin2 = pin_rooftop_left_2;
    _rtRightPin1 = pin_rooftop_right_1;
    _rtRightPin2 = pin_rooftop_right_2;

    // Brake lights
    _brLeftPin = pin_brake_left;
    _brRightPin = pin_brake_right;

    // Reversed lights
    _rLeftPin = pin_reversed_left;
    _rRightPin = pin_reversed_right;

    // Signal lights
    _sLeftFrontPin = pin_signal_left_front;
    _sLeftBackPin = pin_signal_left_back;
    _sRightFrontPin = pin_signal_right_front;
    _sRightBackPin = pin_signal_right_back;

    // Smoke lights
    _smkLeftPin1 = pin_smoke_left_1;
    _smkLeftPin2 = pin_smoke_left_2;
    _smkRightPin1 = pin_smoke_right_1;
    _smkRightPin2 = pin_smoke_right_2;
}

/* 
    * Initialize pins used for Lights
*/
void Light::begin()
{
    // Front lights
    pinMode(_fLeftRectPin, OUTPUT);
    pinMode(_fLeftCirclePin, OUTPUT);
    pinMode(_fRightRectPin, OUTPUT);
    pinMode(_fRightCirclePin, OUTPUT);

    // Beacon lights
    pinMode(_bcLeftPin, OUTPUT);
    pinMode(_bcRightPin, OUTPUT);

    // Rooftop lights
    pinMode(_rtLeftPin1, OUTPUT);
    pinMode(_rtLeftPin2, OUTPUT);
    pinMode(_rtRightPin1, OUTPUT);
    pinMode(_rtRightPin2, OUTPUT);

    // Brake lights
    pinMode(_brLeftPin, OUTPUT);
    pinMode(_brRightPin, OUTPUT);

    // Reversed lights 
    pinMode(_rLeftPin, OUTPUT);
    pinMode(_rRightPin, OUTPUT);

    // Signal lights
    pinMode(_sLeftFrontPin, OUTPUT);
    pinMode(_sLeftBackPin, OUTPUT);
    pinMode(_sRightFrontPin, OUTPUT);
    pinMode(_sRightBackPin, OUTPUT);

    // Smoke lights
    pinMode(_smkLeftPin1, OUTPUT);
    pinMode(_smkLeftPin2, OUTPUT);
    pinMode(_smkRightPin1, OUTPUT);
    pinMode(_smkRightPin2, OUTPUT);
}

/* 
    * Turn on Light forward (or Front light)
*/
void Light::ForwardOn()
{
    digitalWrite(_fLeftRectPin, HIGH);
    digitalWrite(_fLeftCirclePin, HIGH);
    digitalWrite(_fRightRectPin, HIGH);
    digitalWrite(_fRightCirclePin, HIGH);
}

/* 
    * Turn off Light forward (or Front light)
*/
void Light::ForwardOff()
{
    digitalWrite(_fLeftRectPin, LOW);
    digitalWrite(_fLeftCirclePin, LOW);
    digitalWrite(_fRightRectPin, LOW);
    digitalWrite(_fRightCirclePin, LOW);
}

/* 
    * Turn on Light reversed
*/
void Light::ReversedOn()
{
    digitalWrite(_rLeftPin, HIGH);
    digitalWrite(_rRightPin, HIGH);
}

/* 
    * Turn off Light reversed
*/
void Light::ReversedOff()
{
    digitalWrite(_rLeftPin, LOW);
    digitalWrite(_rRightPin, LOW);
}

/* 
    * Turn on Smoke light
*/
void Light::SmokeOn()
{
    digitalWrite(_smkLeftPin1, HIGH);
    digitalWrite(_smkLeftPin2, HIGH);
    digitalWrite(_smkRightPin1, HIGH);
    digitalWrite(_smkRightPin2, HIGH);
}

/* 
    * Turn off Smoke light 
*/
void Light::SmokeOff()
{
    digitalWrite(_smkLeftPin1, LOW);
    digitalWrite(_smkLeftPin2, LOW);
    digitalWrite(_smkRightPin1, LOW);
    digitalWrite(_smkRightPin2, LOW);
}

/*
    * Light signal initialization
*/
void Light::LightSignalInit()
{
    digitalWrite(_sLeftFrontPin, LOW);
    digitalWrite(_sLeftBackPin, LOW);
    digitalWrite(_sRightFrontPin, LOW);
    digitalWrite(_sRightBackPin, LOW);
}

/* 
    * Blink Light signal left
    * Params:
    * - time_period: toggle Light time
*/
void Light::LeftSignal(uint16_t time_period, uint16_t constant_time)
{
    digitalWrite(_sRightFrontPin, LOW);   // Turn off Light front right signal
    digitalWrite(_sRightBackPin, LOW);    // Turn off Light back right signal

    if (time_period % constant_time == 0)  // Toggle Light left and beacon left every 500 ms
    {
        /* Check the previous status of left Light */
        if (digitalRead(_sLeftFrontPin) == LOW) {
            digitalWrite(_sLeftFrontPin, HIGH);
        }
        else {
            digitalWrite(_sLeftFrontPin, LOW);
        }

        if (digitalRead(_sLeftBackPin) == LOW) {
            digitalWrite(_sLeftBackPin, HIGH);
        }
        else {
            digitalWrite(_sLeftBackPin, LOW);
        }
    }
}

/* 
    * Blink Light signal right
    * Params:
    * - time_period: toggle Light time
*/
void Light::RightSignal(uint16_t time_period, uint16_t constant_time)
{
    digitalWrite(_sLeftFrontPin, LOW);   // Turn off Light left signal
    digitalWrite(_sLeftBackPin, LOW);

    if (time_period % constant_time == 0)  // Toggle Light left and beacon left every 500 ms
    {
        /* Check the previous status of left Light */
        if (digitalRead(_sRightFrontPin) == LOW) {
            digitalWrite(_sRightFrontPin, HIGH);
        }
        else {
            digitalWrite(_sRightFrontPin, LOW);
        }

        if (digitalRead(_sRightBackPin) == LOW) {
            digitalWrite(_sRightBackPin, HIGH);
        }
        else {
            digitalWrite(_sRightBackPin, LOW);
        }
    }
}

/*
    * Turn off brake light
*/
void Light::BrakeOff()
{
    digitalWrite(_brLeftPin, LOW);      
    digitalWrite(_brRightPin, LOW);
}

/* 
    * Active Light brake
    * Params:
    * - time_period: toggle Light time
*/
void Light::BrakeActive(uint16_t time_period, uint16_t constant_time)
{
    digitalWrite(_brLeftPin, HIGH);      // On brake Light when active
    digitalWrite(_brRightPin, HIGH);
    if (time_period % constant_time == 0)     // After 500ms, off this Light
    {
        digitalWrite(_brLeftPin, LOW);
        digitalWrite(_brRightPin, LOW);
    }
}

/* 
    * Beacon Light Initialization
*/
void Light::BeaconSignalInit()
{
    digitalWrite(_bcLeftPin, LOW);
    digitalWrite(_bcRightPin, LOW);
}

/* 
    * Blink Light beacon
    * Params:
    * - time_period: toggle Light time
    * - constant_time: constant time toggle
*/
void Light::BeaconSignal(uint16_t time_period, uint16_t constant_time)
{
    /* Toggle Light every 500ms */
    if ((time_period / constant_time) % 2 == 0) {
        digitalWrite(_bcLeftPin, HIGH);
        digitalWrite(_bcRightPin, LOW);
    }
    else {
        digitalWrite(_bcLeftPin, LOW);
        digitalWrite(_bcRightPin, HIGH);
    }
}

/*
    * Hazard Light 
    * Params:
    * - time_period: toggle Light time
    * - constant_time: constant time toggle
*/
void Light::HazardSignal(uint16_t time_period, uint16_t constant_time)
{
    if ((time_period / constant_time) % 2 == 0) {
        digitalWrite(_sLeftFrontPin, LOW);
        digitalWrite(_sLeftBackPin, LOW);
        digitalWrite(_sRightFrontPin, LOW);
        digitalWrite(_sRightBackPin, LOW);
    }
    else {
        digitalWrite(_sLeftFrontPin, HIGH);
        digitalWrite(_sLeftBackPin, HIGH);
        digitalWrite(_sRightFrontPin, HIGH);
        digitalWrite(_sRightBackPin, HIGH);
    }
}
