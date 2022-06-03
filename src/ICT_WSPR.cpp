//#pragma GCC diagnostic error "-Wconversion"
/*
   HABalloon by KD2NDR, Miami Florida October 25 2018
   Improvements by YO3ICT, Bucharest Romania, April-May 2019
   You may use and modify the following code to suit
   your needs so long as it remains open source
   and it is for non-commercial use only.
   Please see readme file for more information.
*/
#include <avr/wdt.h>
#include <TimeLib.h>
#include <TinyGPS++.h>
#include <si5351.h>
#include <JTEncode.h>
#include <rs_common.h>
#include <int.h>
#include <string.h>

#include "Wire.h"
#include "ModeDef.h" // JT mode definitions

#define WSPR_CTC 2668 // CTC value for WSPR - 10672 @ 16Mhz //5336 @ 8Mhz //2668 @ 4Mhz //1334 @ 2Mhz //667 @ 1Mhz

// Enumerations
enum mode
{
  MODE_JT9,
  MODE_WSPR
};
TinyGPSPlus gps;
Si5351 si5351;
JTEncode jtencode;

// Global variables
unsigned long freq;

char call[] = "AA0AAA"; // JT9/WSPR Standard callsign
// uint8_t dbm; // dbm field of WSPR

char call_telemetry[7]; // WSPR telemetry callsign
char loc_telemetry[5];  // WSPR telemetry locator
uint8_t dbm_telemetry;  // WSPR telemetry dbm

char message1[14] = ""; // Message1 (13 char limit) for JT9
char message2[14] = ""; // Message2 (13 char limit) for JT9

char loc4[5]; // 4 digit gridsquare locator
char loc6[7]; // 6 digit gridsquare locator
char loc8[3]; // Last 2 digits of the 8-digit locator

byte Hour, Minute, Second;         // used for timing
long lat, lon, oldlat, oldlon;     // used for location
uint8_t tx_buffer[255];            // WSPR Tx buffer
uint8_t symbol_count;              // JTencode
uint16_t tone_delay, tone_spacing; // JTencode

int alt_meters = 0;
bool telemetry_set = false;
int Sats = 0;
int gps_speed = 0;
volatile bool proceed = false;

#include "TelemFunctions.h" // various telemetry functions
#include "encode.h"         // symbol encoding
#include "setMode.h"        // mode functions
#include "messageGen.h"     // telemetry > message generation
#include "GPS.h"            // code to set U-Blox GPS into airborne mode
#include "timing3.h"        // scheduling

ISR(TIMER1_COMPA_vect)
{
  proceed = true;
}

void setup()
{
  wdt_enable(WDTO_8S); // First enable the watchdog timer
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW); // Si5351 off
  delay(1000);          // Allow GPS time to start
  Serial.begin(9600);
  delay(1000);       // Serial time to start
  setGPS_AirBorne(); // Set GPS into airborne mode
  delay(500);
  gps_set_max_performance_mode(); // Set GPS into high performance mode
  delay(500);
  // Serial.println(F("START"));
  noInterrupts(); // Set up Timer1 for interrupts every symbol period.
  TCCR1A = 0;
  TCNT1 = 0;
  TCCR1B = (1 << CS12) |
           (1 << CS10) |
           (1 << WGM12);
  TIMSK1 = (1 << OCIE1A);
  OCR1A = WSPR_CTC;
  interrupts();
  wdt_reset();
}

void loop()
{
  wdt_reset();
  while (Serial.available() > 0)
  {
    if (gps.encode(Serial.read())) // GPS related functions need to be in here to work with tinyGPS Plus library
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
