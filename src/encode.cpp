/*
   Message encoding
*/

#include <JTEncode.h>
#include "ModeDef.h"
#include "encode.h"
#include "Arduino.h"
#include "si5351.h"
#include <IWatchdog.h>

JTEncode jtencode;

extern uint8_t tx_buffer[255];
uint8_t symbol_count;
extern Si5351 si5351;
extern unsigned long freq;
uint16_t tone_delay, tone_spacing; // JTencode
extern char call_telemetry[7];            // WSPR telemetry callsign
extern char loc_telemetry[5];             // WSPR telemetry locator
extern uint8_t dbm_telemetry;             // WSPR telemetry dbm
extern uint8_t tx_buffer[255];            // WSPR Tx buffer
extern char loc4[5];                      // 4 digit gridsquare locator
extern char message1[14];                 // Message1 (13 char limit) for JT9
extern char message2[14];                 // Message2 (13 char limit) for JT9
extern char call[];                       // JT9/WSPR Standard callsign

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

void setModeJT9_1()
{
    symbol_count = JT9_SYMBOL_COUNT;
    tone_spacing = JT9_TONE_SPACING;
    tone_delay = JT9_DELAY;
    memset(tx_buffer, 0, 255); // Clears Tx buffer from previous operation.
    jtencode.jt9_encode(message1, tx_buffer);
}

void setModeJT9_2()
{
    symbol_count = JT9_SYMBOL_COUNT;
    tone_spacing = JT9_TONE_SPACING;
    tone_delay = JT9_DELAY;
    memset(tx_buffer, 0, 255); // Clears Tx buffer from previous operation.
    jtencode.jt9_encode(message2, tx_buffer);
}

void setModeWSPR()
{
    symbol_count = WSPR_SYMBOL_COUNT;
    tone_spacing = WSPR_TONE_SPACING;
    tone_delay = WSPR_DELAY;
    memset(tx_buffer, 0, 255); // Clears Tx buffer from previous operation.
    jtencode.wspr_encode(call, loc4, 10, tx_buffer);
}

void setModeWSPR_telem()
{
    symbol_count = WSPR_SYMBOL_COUNT;
    tone_spacing = WSPR_TONE_SPACING;
    tone_delay = WSPR_DELAY;
    memset(tx_buffer, 0, 255); // Clears Tx buffer from previous operation.
    jtencode.wspr_encode(call_telemetry, loc_telemetry, dbm_telemetry, tx_buffer);
}
