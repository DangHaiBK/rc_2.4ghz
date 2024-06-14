#include "rx.h"

#define INTERRUPT_CHANNEL_1   8
#define INTERRUPT_CHANNEL_2   9
#define INTERRUPT_CHANNEL_3   7
#define INTERRUPT_CHANNEL_4   6
#define INTERRUPT_CHANNEL_5   5
#define INTERRUPT_CHANNEL_6   4

void setup()
{
    // Create an instance for receiver
    RX_MC6C rxMc6c(
      INTERRUPT_CHANNEL_1,
      INTERRUPT_CHANNEL_2,
      INTERRUPT_CHANNEL_3,
      INTERRUPT_CHANNEL_4,
      INTERRUPT_CHANNEL_5,
      INTERRUPT_CHANNEL_6
    );

    // RX initialization
    rxMc6c.begin();

}

void loop()
{

}