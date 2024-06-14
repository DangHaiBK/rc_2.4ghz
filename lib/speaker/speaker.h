#ifndef SPEAKER_H
#define SPEAKER_H

#include <Arduino.h>

class Speaker
{
public:
    Speaker(
        int pin,
        uint16_t sampleRate,
        uint16_t fadeDuration
    );

    void begin();
    void PlaySound(const unsigned char* data, unsigned int length);

private:
    int _pin;
    uint16_t _sampleRate;
    uint16_t _fadeDuration;

};

#endif /* SPEAKER_H */