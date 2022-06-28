/*
   Message encoding
*/

#include "encode.h"
#include "Arduino.h"
#include "si5351.h"
#include <IWatchdog.h>

extern uint8_t tx_buffer[255];
extern uint8_t symbol_count;
extern Si5351 si5351;
extern unsigned long freq;
extern uint16_t tone_delay, tone_spacing; // JTencode

void encode() // Loop through the string, transmitting one character at a time
{
    uint8_t i;
    si5351.output_enable(SI5351_CLK0, 1); // Reset the tone to the base frequency and turn on the output

    for (i = 0; i < symbol_count; i++) // Now transmit the channel symbols
    {
        si5351.set_freq((freq * 100) + (tx_buffer[i] * tone_spacing), SI5351_CLK0);
        delay(tone_delay);
        IWatchdog.reload();
    }

    si5351.output_enable(SI5351_CLK0, 0); // Turn off the output
}