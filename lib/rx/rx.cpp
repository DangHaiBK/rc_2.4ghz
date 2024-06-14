#include "rx.h"

/* Variables ----------------------------------------------------------- */
volatile bool rx_new_pulse[] = {false, false, false, false, false, false};
volatile unsigned long rx_pulse_begin[] = {0, 0, 0, 0, 0, 0};
volatile unsigned long rx_channel_width[] = {0, 0, 0, 0, 0, 0};

/* 
    * Call constructor to assign pins
*/
RX_MC6C::RX_MC6C(
    int pin_ch1,
    int pin_ch2,
    int pin_ch3,
    int pin_ch4,
    int pin_ch5,
    int pin_ch6
)
{
    _pinch1 = pin_ch1;
    _pinch2 = pin_ch2;
    _pinch3 = pin_ch3;
    _pinch4 = pin_ch4;
    _pinch5 = pin_ch5;
    _pinch6 = pin_ch6;
}

/* 
    * Receiver Initialization
*/
void RX_MC6C::begin()
{
    pinMode(_pinch1, INPUT);
    pinMode(_pinch2, INPUT);
    pinMode(_pinch3, INPUT);
    pinMode(_pinch4, INPUT);
    pinMode(_pinch5, INPUT);
    pinMode(_pinch6, INPUT);
}

/*
    * Private function to read raw data from transmitter
*/
void RX_MC6C::rxReadRaw(uint8_t pin, uint8_t channel)
{
    // Rising edge
    if (digitalRead(pin) == HIGH) {
        rx_pulse_begin[channel] = micros();
    }
    // Falling edge
    else {
        rx_channel_width[channel] = micros() - rx_pulse_begin[channel];
        rx_new_pulse[channel] = true;
    }
}

unsigned long RX_MC6C::rxReadPwm(uint8_t channel)
{
    rx_new_pulse[channel] = false;
    return rx_channel_width[channel];
}

/*
    * Public function used for interrupt
*/
void RX_MC6C::measuringChannel_1()
{
    rxReadRaw(_pinch1, RX_INPUT_MAP_CHANNEL_1_TO_NUMBER);
}

void RX_MC6C::measuringChannel_2()
{
    rxReadRaw(_pinch2, RX_INPUT_MAP_CHANNEL_2_TO_NUMBER);
}

void RX_MC6C::measuringChannel_3()
{
    rxReadRaw(_pinch3, RX_INPUT_MAP_CHANNEL_3_TO_NUMBER);
}

void RX_MC6C::measuringChannel_4()
{
    rxReadRaw(_pinch4, RX_INPUT_MAP_CHANNEL_4_TO_NUMBER);
}

void RX_MC6C::measuringChannel_5()
{
    rxReadRaw(_pinch5, RX_INPUT_MAP_CHANNEL_5_TO_NUMBER);
}

void RX_MC6C::measuringChannel_6()
{
    rxReadRaw(_pinch6, RX_INPUT_MAP_CHANNEL_6_TO_NUMBER);
}

/*
    * Public function to get and process data from transmitter
*/
void RX_MC6C::rxReadPulse(uint8_t channel, uint8_t *direct, uint16_t *val)
{
    unsigned long ulValue = rxReadPwm(channel);
    if (ulValue > RECEIVER_PWM_NEUTRAL)
    {
       if ((ulValue - RECEIVER_PWM_NEUTRAL) > RECEIVER_ERROR_RATE)
       {
         *direct = RECEIVER_STICK_INCREASING;
         *val = ulValue;
       }
       else 
       {
         *direct = RECEIVER_STICK_MIDDLE;
         *val = RECEIVER_PWM_NEUTRAL;
       }
    }
    if (ulValue < RECEIVER_PWM_NEUTRAL)
    {
       if ((RECEIVER_PWM_NEUTRAL - ulValue) > RECEIVER_ERROR_RATE)
       {
         *direct = RECEIVER_STICK_DECREASING;
         *val = ulValue;
       }
       else 
       {
         *direct = RECEIVER_STICK_MIDDLE;
         *val = RECEIVER_PWM_NEUTRAL;
       }
    }

    if (ulValue > RECEIVER_PWM_MAX)
    {
      *direct = RECEIVER_STICK_INCREASING;
      *val = RECEIVER_PWM_MAX;
    }
    else 
    {
      if ((RECEIVER_PWM_MAX - ulValue) <= RECEIVER_ERROR_RATE)
      {
        *direct = RECEIVER_STICK_INCREASING;
        *val = RECEIVER_PWM_MAX;
      }
    }

    if (ulValue <= RECEIVER_PWM_MIN)
    {
      *direct = RECEIVER_STICK_DECREASING;
      *val = RECEIVER_PWM_MIN;
    }
    else
    {
      if ((ulValue - RECEIVER_PWM_MIN) <= RECEIVER_ERROR_RATE)
      {
        *direct = RECEIVER_STICK_DECREASING;
        *val = RECEIVER_PWM_MIN;
      }
    }
}