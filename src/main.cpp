/* Includes ------------------------------------------------------------------------ */
#include "rx.h"

/* Define constants ---------------------------------------------------------------- */
#define INTERRUPT_CHANNEL_1   8
#define INTERRUPT_CHANNEL_2   9
#define INTERRUPT_CHANNEL_3   7
#define INTERRUPT_CHANNEL_4   6
#define INTERRUPT_CHANNEL_5   5
#define INTERRUPT_CHANNEL_6   4

/* Variables ----------------------------------------------------------------------- */
uint8_t direct[] = {0, 0, 0, 0, 0, 0};
uint16_t valPwm[] = {0, 0, 0, 0, 0, 0};
uint8_t channelInterrupt[6] = {
    INTERRUPT_CHANNEL_1,
    INTERRUPT_CHANNEL_2,
    INTERRUPT_CHANNEL_3,
    INTERRUPT_CHANNEL_4,
    INTERRUPT_CHANNEL_5,
    INTERRUPT_CHANNEL_6
  };

/* Class objects ------------------------------------------------------------------- */

// Create an instance for receiver
RX_MC6C rxMc6c(
    INTERRUPT_CHANNEL_1,
    INTERRUPT_CHANNEL_2,
    INTERRUPT_CHANNEL_3,
    INTERRUPT_CHANNEL_4,
    INTERRUPT_CHANNEL_5,
    INTERRUPT_CHANNEL_6
);

void setup()
{
    void (RX_MC6C::*interruptFunctions[])() = {
        &RX_MC6C::measuringChannel_1,
        &RX_MC6C::measuringChannel_2,
        &RX_MC6C::measuringChannel_3,
        &RX_MC6C::measuringChannel_4,
        &RX_MC6C::measuringChannel_5,
        &RX_MC6C::measuringChannel_6
    };

    // RX initialization
    rxMc6c.begin();

    // Interrupt initialization
    for (uint8_t i=0; i<RX_MC6C_NUMBER_CHANNEL; i++)
    {
      attachInterrupt(digitalPinToInterrupt(channelInterrupt[i]), (rxMc6c.*interruptFunctions[i])(), CHANGE);
      //(rxMc6c.*interruptFunctions[i])();
    }

    // UART initialization
    Serial.begin(9600);
}

void loop()
{
    /* Read values from all channels RX */
    for (uint8_t i=0; i<RX_MC6C_NUMBER_CHANNEL; i++)
    {
        rxMc6c.rxReadPulse(i, &direct[i], &valPwm[i]);
    }
  
    /* Print the results on channel 1 and 2 */
    Serial.println(valPwm[0]);
    Serial.println(valPwm[1]);
    
    /* Refresh for the next read turn */
    delay(RECEIVER_FRESH_RATE);
}
