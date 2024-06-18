/* Includes ------------------------------------------------------------------------ */
#include "rx.h"
#include "speaker.h"
#include "config.h"
#include "sounds/scania/ScaniaV8start.h"

/* Typedef define a new pointer function ------------------------------------------- */
typedef void (RX_MC6C::*functionPointer)();

/* Variables ----------------------------------------------------------------------- */
volatile uint16_t sampleIndex = 0;
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

// Create an instance for speaker
Speaker speaker(
    SPEAKER_CHANNEL
);

/* Static void to create a wrapper for function ------------------------------------- */
static void wrapFunctionCaller(RX_MC6C& cls, functionPointer func);

/* Wrapper functions for interrupt functions --------------------------------------- */
void wrapInterruptFunction_1();
void wrapInterruptFunction_2();
void wrapInterruptFunction_3();
void wrapInterruptFunction_4();
void wrapInterruptFunction_5();
void wrapInterruptFunction_6();

/* Create an array to contain pointers to point wrapper functions ----------------- */
void (*funcArray[6])(void) = {
    wrapInterruptFunction_1,
    wrapInterruptFunction_2,
    wrapInterruptFunction_3,
    wrapInterruptFunction_4,
    wrapInterruptFunction_5,
    wrapInterruptFunction_6
};

/* Setup function ---------------------------------------------------------------- */
void setup()
{
    // Speaker initialization
    speaker.begin();
#if ((AUDIO_TEST == 1) && (TRIAL_TEST_1 == 1))
    // Audio Timers setup
    speaker.TimerAudioInit(CPU_FREQ, startSampleRate);
#endif
    // RX initialization
    rxMc6c.begin();

    // Interrupt initialization
    for (uint8_t i=0; i<RX_MC6C_NUMBER_CHANNEL; i++)
    {
      attachInterrupt(digitalPinToInterrupt(channelInterrupt[i]), funcArray[i], CHANGE);
    }

    // UART initialization
    Serial.begin(9600);

#if (AUDIO_TEST == 1)

    ASSR &= ~(_BV(EXCLK) | _BV(AS2));
/* Set up Timer 2 with Fast PWM */
    cli();   // Disable global interrupt
#endif
}

/* Loop function ---------------------------------------------------------------- */
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

/* Static wrapper function */
static void wrapFunctionCaller(RX_MC6C& cls, functionPointer func)
{
    (cls.*func)();
}

/* Wrapper function for interrupt function on channel 1 */
void wrapInterruptFunction_1()
{
    wrapFunctionCaller(rxMc6c, &RX_MC6C::measuringChannel_1);
}

/* Wrapper function for interrupt function on channel 2 */
void wrapInterruptFunction_2()
{
    wrapFunctionCaller(rxMc6c, &RX_MC6C::measuringChannel_2);
}

/* Wrapper function for interrupt function on channel 3 */
void wrapInterruptFunction_3()
{
    wrapFunctionCaller(rxMc6c, &RX_MC6C::measuringChannel_3);
}

/* Wrapper function for interrupt function on channel 4 */
void wrapInterruptFunction_4()
{
    wrapFunctionCaller(rxMc6c, &RX_MC6C::measuringChannel_4);
}

/* Wrapper function for interrupt function on channel 5 */
void wrapInterruptFunction_5()
{
    wrapFunctionCaller(rxMc6c, &RX_MC6C::measuringChannel_5);
}

/* Wrapper function for interrupt function on channel 6 */
void wrapInterruptFunction_6()
{
    wrapFunctionCaller(rxMc6c, &RX_MC6C::measuringChannel_6);
}

#if (AUDIO_TEST == 1)
    ISR(TIMER1_COMPA_vect) 
    {
        if (sampleIndex < startSampleCount) {
            uint8_t sample = startSamples[sampleIndex];

            // Fade-in
            if (sampleIndex < AUDIO_FADEIN_DURATION) {
                sample = (sample * sampleIndex) / AUDIO_FADEIN_DURATION;
            }

            // Fade-out
            if (sampleIndex >= startSampleCount - AUDIO_FADEOUT_DURATION) {
                sample = (sample * (startSampleCount - sampleIndex)) / AUDIO_FADEOUT_DURATION;
            }

            OCR2B = sample;
            sampleIndex++;
        } else {
            sampleIndex = 0; 
        }
    }
#endif