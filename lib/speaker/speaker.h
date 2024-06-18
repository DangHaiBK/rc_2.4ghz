#ifndef SPEAKER_H
#define SPEAKER_H

#include <Arduino.h>

#define TRIAL_TEST_1              0
#define TRIAL_TEST_2              1
#define AUDIO_FADEIN_DURATION     500
#define AUDIO_FADEOUT_DURATION    500

class Speaker
{
public:
    Speaker(
        int pin
    );

    void begin();
    void Speaker::TimerAudioInit(unsigned long timerFreq, uint16_t sampleRate);
    void PlaySound(const unsigned char* data, unsigned int length);

private:
    int _pin;
    uint16_t _sampleRate;
    uint16_t _fadeDuration;

};

#endif /* SPEAKER_H */