#include "temperature.hpp"

int get_temperature()
{
    int wADC;
    int temp;

    ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
    ADCSRA |= _BV(ADEN);
    delay(20);
    ADCSRA |= _BV(ADSC);
    while (bit_is_set(ADCSRA, ADSC))
    {
    }
    wADC = ADCW;
    temp = (wADC - 322.2) / 1.43;

    return temp;
}
