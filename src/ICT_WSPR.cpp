//#pragma GCC diagnostic error "-Wconversion"
/*
   HABalloon by KD2NDR, Miami Florida October 25 2018
   Improvements by YO3ICT, Bucharest Romania, April-May 2019
   You may use and modify the following code to suit
   your needs so long as it remains open source
   and it is for non-commercial use only.
   Please see readme file for more information.
*/
#include <IWatchdog.h>
#include <TimeLib.h>
#include <TinyGPS++.h>
#include <si5351.h>
#include <rs_common.h>
#include <int.h>
#include <string.h>
#include "comm_ports.hpp"

#include "Wire.h"
#include "ModeDef.h" // JT mode definitions
#include "config.hpp"

#include "TelemFunctions.h" // various telemetry functions
#include "encode.h"         // symbol encoding
#include "messageGen.h"     // telemetry > message generation
#include "GPS.h"            // code to set U-Blox GPS into airborne mode
#include "timing3.h"        // scheduling

// Enumerations
enum mode
{
  MODE_JT9,
  MODE_WSPR
};
TinyGPSPlus gps;
Si5351 si5351;
GPS gpsHardware;

// Global variables
char call[] = CALL_SIGN; // JT9/WSPR Standard callsign
// uint8_t dbm; // dbm field of WSPR

void setup()
{
  // Init the watchdog timer with 10 second timeout
  IWatchdog.begin(10000000);
  pinMode(4, OUTPUT);
  digitalWrite(SI5351_ENABLE_PIN, LOW); // Si5351 off
  delay(1000);                          // Allow GPS time to start
  gpsSerial.begin(9600);
  delay(1000);       // gpsSerial time to start
  gpsHardware.gps_set_max_performance_mode(); // Set GPS into high performance mode
  delay(500);
  debugSerial.println(F("START"));
  IWatchdog.reload();
}

void loop()
{
  IWatchdog.reload();
  while (gpsSerial.available() > 0)
  {
    if (gps.encode(gpsSerial.read())) // GPS related functions need to be in here to work with tinyGPS Plus library
    {
      if (timeStatus() == timeNotSet) // only sets time if already not done previously
      {
        setGPStime(); // sets system time to GPS UTC time for sync
      }
    }
  }

  if (gps.location.isValid())
  {
    TXtiming(); // process timing
  }
}
