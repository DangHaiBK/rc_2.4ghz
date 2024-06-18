#include "speaker.h"

/*
    * Call constructor to assign pins
*/
Speaker::Speaker(int pin)
{
    _pin = pin;
}

/* 
    * Timer Initialization for read audio data from header file
    * Currently using Timer 1 (AVR chip)
*/
void Speaker::TimerAudioInit(unsigned long timerFreq, uint16_t sampleRate)
{
#if (TRIAL_TEST_1 == 1)
    /* Disable global interrupt for initialzing timer */
    cli(); 

    /* --- Timer 1 configuration --- */
    /* Reset timer 1 registers */
    TCCR1A = 0;
    TCCR1B = 0; 
    TCNT1  = 0; // counter = 0

    /* Compare match register = (frequency / sample_rate) - 1 */
    // Đặt compare match register cho 22050 Hz sample rate
    OCR1A = timerFreq / sampleRate - 1;
    //OCR1A = 719; // = 16000000 / (22050) - 1 (với prescaler 1)

    /* CTC Mode Enable (Clear Timer on Compare Match) */
    TCCR1B |= (1 << WGM12);

    /* Set prescaler to 1 and start timer 1 */
    TCCR1B |= (1 << CS10);

    /* Enable Timer 1 compare interrupt */
    TIMSK1 |= (1 << OCIE1A);

    /* --- PWM for output audio to speaker with Timer 2 --- */
    /* Reset timer 2 registers */
    TCCR2A = 0;
    TCCR2B = 0; 
    TCNT2  = 0; // counter về 0

    /* Fast PWM on Timer 2*/
    TCCR2A |= (1 << WGM20) | (1 << WGM21);

    /* Non-inverting mode for OC2B ~ pin 3 */
    TCCR2A |= (1 << COM2B1);

    /* Set prescaler = 1 and start timer 2*/
    TCCR2B |= (1 << CS20);

    /* Enable global interrupt */
    sei();
#endif

#if (TRIAL_TEST_2 == 1)

    /* --- Timer 2 configuration --- */

    /* --- Timer 1 configuration --- */
    /* Disable global interrupt*/
    cli();
    /* CTC mode */
    TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);   // Set CTC mode (Clear Timer on Compare Match) (p.133)
    TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));   // Have to set OCR1A *after*, otherwise it gets reset to 0!

    /* No Prescaler or Prescaler = 1*/
    TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

    /* Set compare register */
    OCR1A = timerFreq / sampleRate;                                // Set the compare register (OCR1A).
    // OCR1A is a 16-bit register, so we have to do this with
    // interrupts disabled to be safe.

    /* Enable Timer compare interrupt */
    TIMSK1 |= _BV(OCIE1A);   

    /* Enable global inerrupt */
    sei();
#endif
}

/*
    * Speaker pin initialization
*/
void Speaker::begin()
{
    pinMode(_pin, OUTPUT);
}

/*
    * Play soundtrack
    * Params:
    * - data: soundtrack array
    * - length: length of soundtrack
*/
void Speaker::PlaySound(const unsigned char* data, unsigned int length) 
{

}