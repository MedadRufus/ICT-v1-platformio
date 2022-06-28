/*
   GPS functions
*/

#include "stdint.h"

class GPS
{
private:
   void sendUBX(uint8_t *MSG, uint8_t len);
   bool getUBX_ACK(uint8_t *MSG);
public:
   GPS();
   ~GPS();

   bool setGPS_AirBorne();
   bool gps_reset();
   bool gps_set_max_performance_mode();
};

