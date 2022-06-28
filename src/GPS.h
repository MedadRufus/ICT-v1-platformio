/*
   GPS functions
*/

#include "stdint.h"

void sendUBX(uint8_t *MSG, uint8_t len);
bool getUBX_ACK(uint8_t *MSG);
bool setGPS_AirBorne();
bool gps_reset();
bool gps_set_max_performance_mode();
