#include "speaker.h"

/*
    * Call constructor to assign pins
*/
Speaker::Speaker(int pin)
{
    _pin = pin;
}

/*
    * Speaker pin initialization
*/
void Speaker::begin()
{
    pinMode(_pin, OUTPUT);
}

/* 
    * Timer Initialization for read audio data from header file
    * It is not used by this file
    * It is only used for speaker_timer.cpp
*/
void Speaker::TimerAudioInit(unsigned long timerFreq, uint16_t sampleRate)
{
    // Nothing to do here
}

/*
    * Play soundtrack
    * Params:
    * - data: soundtrack array
    * - length: length of soundtrack
*/
void Speaker::PlaySound(const unsigned char* data, unsigned int length) 
{
    // int delayTime = 1000000 / _sampleRate;

    // // Fade in
    // for (unsigned int i = 0; i < _fadeDuration * _sampleRate / 1000; i++) 
    // {
    //     float fadeFactor = (float)i / (_fadeDuration * _sampleRate / 1000);
    //     analogWrite(_pin, data[i] * fadeFactor);
    //     delayMicroseconds(delayTime);
    // }

    // // Play normally without fading in and out
    // for (unsigned int i = _fadeDuration * _sampleRate / 1000; i < length - _fadeDuration * _sampleRate / 1000; i++) 
    // {
    //     analogWrite(_pin, data[i]);
    //     delayMicroseconds(delayTime);
    // }

    // // Fade out
    // for (unsigned int i = length - _fadeDuration * _sampleRate / 1000; i < length; i++) 
    // {
    //     float fadeFactor = (float)(length - i) / (_fadeDuration * _sampleRate / 1000);
    //     analogWrite(_pin, data[i] * fadeFactor);
    //     delayMicroseconds(delayTime);
    // }
}
