#ifndef CONFIG_H
#define CONFIG_H

/* Includes ---------------------------------------------------------------- */

/* Interrupt pins ---------------------------------------------------------- */
#define INTERRUPT_CHANNEL_1   9
#define INTERRUPT_CHANNEL_2   8
#define INTERRUPT_CHANNEL_3   7
#define INTERRUPT_CHANNEL_4   6
#define INTERRUPT_CHANNEL_5   5
#define INTERRUPT_CHANNEL_6   4

/* Speaker pin ------------------------------------------------------------- */
#define SPEAKER_CHANNEL       3

/* Define CPU Frequency ---------------------------------------------------- */
#define CPU_FREQ              16000000L     // 16MHz Clock

/* For test elements ------------------------------------------------------- */
#define RX_TEST               1
#define AUDIO_TEST            1

#endif /* CONFIG_H */