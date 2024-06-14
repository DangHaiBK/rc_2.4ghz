#ifndef RX_H
#define RX_H

#include <Arduino.h>

#define RX_MC6C_NUMBER_CHANNEL              6

#define RX_INPUT_MAP_CHANNEL_1_TO_NUMBER    0
#define RX_INPUT_MAP_CHANNEL_2_TO_NUMBER    1
#define RX_INPUT_MAP_CHANNEL_3_TO_NUMBER    2
#define RX_INPUT_MAP_CHANNEL_4_TO_NUMBER    3
#define RX_INPUT_MAP_CHANNEL_5_TO_NUMBER    4
#define RX_INPUT_MAP_CHANNEL_6_TO_NUMBER    5

#define RECEIVER_PWM_NEUTRAL                1500    /* Typical value */
#define RECEIVER_PWM_MAX                    2000    /* Typical value */
#define RECEIVER_PWM_MIN                    1000    /* Typical value */
#define RECEIVER_ERROR_RATE                 30      /* Typical value +- this value */
#define RECEIVER_FRESH_RATE                 100     /* [MS] Frame rate, typical 10 - 300 Hz */

#define RECEIVER_STICK_MIDDLE         0
#define RECEIVER_STICK_INCREASING     1
#define RECEIVER_STICK_DECREASING     2

class RX_MC6C
{
public:
    RX_MC6C(
        int pin_ch1,
        int pin_ch2,
        int pin_ch3,
        int pin_ch4,
        int pin_ch5,
        int pin_ch6
    );

    void begin();
    void rxReadPulse(uint8_t channel, uint8_t *direct, uint16_t *val);
    void measuringChannel_1(void);
    void measuringChannel_2(void);
    void measuringChannel_3(void);
    void measuringChannel_4(void);
    void measuringChannel_5(void);
    void measuringChannel_6(void);

private:
    int _pinch1;
    int _pinch2;
    int _pinch3;
    int _pinch4;
    int _pinch5;
    int _pinch6;
    unsigned long rxReadPwm(uint8_t channel);
    void rxReadRaw(uint8_t pin, uint8_t channel);
};

#endif /* RX_H */