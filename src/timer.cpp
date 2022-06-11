#include "timer.hpp"

#define WSPR_CTC 2668 // CTC value for WSPR - 10672 @ 16Mhz //5336 @ 8Mhz //2668 @ 4Mhz //1334 @ 2Mhz //667 @ 1Mhz

volatile bool proceed = false;

// ISR(TIMER1_COMPA_vect)
// {
//     proceed = true;
// }

void setup_timer_interrupt()
{
    // TCCR1A = 0;
    // TCNT1 = 0;
    // TCCR1B = (1 << CS12) |
    //          (1 << CS10) |
    //          (1 << WGM12);
    // TIMSK1 = (1 << OCIE1A);
    // OCR1A = WSPR_CTC;
}
