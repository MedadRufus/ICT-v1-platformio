#include "solar_voltage.hpp"

float get_solar_voltage()
{
    float volt;
    analogReference(DEFAULT);
    unsigned int sensorVolt = analogRead(0);
    sensorVolt += analogRead(0);
    sensorVolt += analogRead(0);
    sensorVolt += analogRead(0);
    sensorVolt += analogRead(0);
    sensorVolt = sensorVolt / 5;
    volt = sensorVolt * 1.1f;
    volt = volt / 1023.0f;
    volt = volt * 3.35f;
}

