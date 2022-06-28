/*
   Telemetry functions
*/
#include "temperature.hpp"
#include "solar_voltage.hpp"
#include "utils.hpp"
#include "comm_ports.hpp"
#include "config.hpp"
#include "radio.hpp"
#include "TelemFunctions.h"
#include "Arduino.h"
#include <TimeLib.h>
#include <TinyGPS++.h>

extern long lat, lon, oldlat, oldlon; // used for location
char loc4[5]; // 4 digit gridsquare locator
char loc6[7]; // 6 digit gridsquare locator
char loc8[3]; // Last 2 digits of the 8-digit locatorextern TinyGPSPlus gps;
extern TinyGPSPlus gps;
byte Hour, Minute, Second; // used for timing
char call_telemetry[7]; // WSPR telemetry callsign
char loc_telemetry[5];  // WSPR telemetry locator
int Sats = 0;
int gps_speed = 0;
uint8_t dbm_telemetry;  // WSPR telemetry dbm

void setGPStime() // Sets system time from GPS
{
  if (gps.time.isValid())
  {
    Hour = gps.time.hour();
    Minute = gps.time.minute();
    Second = gps.time.second();
    setTime(Hour, Minute, Second, 1, 1, 2000); // (hr,min,sec,day,month,yr)
  }
}

void loc8calc() // Calculates full locator from GPS
{

  lon = (gps.location.lng() * 100000) + 18000000L;
  lat = (gps.location.lat() * 100000) + 9000000L;

  char MH[8] = {'A', 'A', '0', '0', 'A', 'A', '0', '0'}; // Full locator characters

  MH[0] += lon / 2000000;
  MH[1] += lat / 1000000;
  MH[2] += (lon % 2000000) / 200000;
  MH[3] += (lat % 1000000) / 100000;
  MH[4] += ((lon % 200000) / 8333);
  MH[5] += ((lat % 100000) / 4166);
  MH[6] += ((lon % 200000) % 8333) / 833;
  MH[7] += ((lat % 100000) % 4166) / 416;

  String loc4_txt = "";
  int i = 0;
  while (i < 4)
  {
    loc4_txt += MH[i];
    i++;
  }
  loc4_txt.toCharArray(loc4, 5);

  String loc6_txt = "";
  i = 0;
  while (i < 6)
  {
    loc6_txt += MH[i];
    i++;
  }
  loc6_txt.toCharArray(loc6, 7);

  String loc8_txt = "";
  i = 6;
  while (i < 8)
  {
    loc8_txt += MH[i];
    i++;
  }
  loc8_txt.toCharArray(loc8, 3);
}

void call_telem()
{

  double alt_meters = gps.altitude.meters();

  /*
    char MH[8] = {'A', 'A', '0', '0', 'A', 'A', '0', '0'}; // Full locator characters

    MH[0] +=  lon / 2000000;
    MH[1] +=  lat / 1000000;
    MH[2] += (lon % 2000000) / 200000;
    MH[3] += (lat % 1000000) / 100000;
    MH[4] += ((lon % 200000) / 8333);
    MH[5] += ((lat % 100000) / 4166);
    MH[6] += ((lon % 200000) % 8333) / 833;
    MH[7] += ((lat % 100000) % 4166) / 416;
  */

  char MH[2] = {'A', 'A'};
  MH[0] += ((lon % 200000) / 8333);
  MH[1] += ((lat % 100000) / 4166);

  call_telemetry[0] = 'X'; // telemetry channel 15
  call_telemetry[2] = 'X';

  int a = MH[0] - 'A';
  int b = MH[1] - 'A';
  int c = a * 24 + b;
  int d = int(alt_meters / 20);

  long e = (long)(1068L * c + d);
  long f = float(e / 17576L);

  if (f < 10)
  {
    call_telemetry[1] = f + '0';
  }
  else
  {
    call_telemetry[1] = f - 10 + 'A';
  }

  long g = e - f * 17576L;
  int h = int(g / 676);
  call_telemetry[3] = h + 'A';

  long i = g - h * 676L;
  int j = int(i / 26);
  call_telemetry[4] = j + 'A';

  long k = i - j * 26L;
  int l = int(k / 1);
  call_telemetry[5] = l + 'A';
}

void loc_dbm_telem()
{
  Sats = gps.satellites.value();
  gps_speed = gps.speed.knots();

  int temp = get_temperature();

  // Read voltage from pin A0. Solar voltage.
  delay(20);
  float volt = get_solar_voltage();

  volt = clamp(volt, 3.0, 4.94);
  temp = clamp(temp, -49, 39);

  int GPS = 0;
  if ((lon != oldlon) || (lat != oldlat))
  {
    GPS = 1;
  }
  else
  {
    GPS = 0;
  }
  oldlon = lon;
  oldlat = lat;

  if (Sats < 5)
  {
    Sats = 0;
  }
  else
  {
    Sats = 1;
  }

  debugSerial.print(temp);
  debugSerial.print(" ");
  debugSerial.print(volt);
  debugSerial.print(" ");
  debugSerial.print(gps_speed);
  debugSerial.print(" ");
  debugSerial.print(GPS);
  debugSerial.print(" ");
  debugSerial.print(Sats);
  debugSerial.print("  ");

  int temp_raw = (int)(1024L * (temp * 0.01 + 2.73)) / 5;
  temp_raw = (int)(temp_raw - 457) / 2;

  float batt_raw = (int)(1024L * volt) / 5;

  batt_raw = (batt_raw - 614) / 10;

  float round_number = round(batt_raw);
  if (round_number > batt_raw)
    round_number = round_number - 1;

  batt_raw = (int)round_number;

  long x = (temp_raw * 40L) + batt_raw;
  long y = (x * 42L) + (int)gps_speed / 2;
  long z = (y * 2L) + (int)GPS;
  long xy = (z * 2L) + (int)Sats;

  debugSerial.println(temp_raw);
  debugSerial.println(x);
  debugSerial.println(y);
  debugSerial.println(z);
  debugSerial.println(xy);

  long a = (int)(xy / 34200L);
  loc_telemetry[0] = a + 'A';

  long b = xy - (34200L * a);
  long c = (int)(b / 1900L);
  loc_telemetry[1] = c + 'A';

  long d = b - (1900L * c);
  long e = (int)(d / 190L);
  loc_telemetry[2] = e + '0';

  long f = d - (190L * e);
  long g = (int)(f / 19L);
  loc_telemetry[3] = g + '0';

  debugSerial.print(loc_telemetry[0]);
  debugSerial.print(loc_telemetry[1]);
  debugSerial.print(loc_telemetry[2]);
  debugSerial.print(loc_telemetry[3]);
  debugSerial.print(" ");

  dbm_telemetry = f - (g * 19L);

  if (dbm_telemetry == 0)
  {
    dbm_telemetry = 0;
  }
  else if (dbm_telemetry == 1)
  {
    dbm_telemetry = 3;
  }
  else if (dbm_telemetry == 2)
  {
    dbm_telemetry = 7;
  }
  else if (dbm_telemetry == 3)
  {
    dbm_telemetry = 10;
  }
  else if (dbm_telemetry == 4)
  {
    dbm_telemetry = 13;
  }
  else if (dbm_telemetry == 5)
  {
    dbm_telemetry = 17;
  }
  else if (dbm_telemetry == 6)
  {
    dbm_telemetry = 20;
  }
  else if (dbm_telemetry == 7)
  {
    dbm_telemetry = 23;
  }
  else if (dbm_telemetry == 8)
  {
    dbm_telemetry = 27;
  }
  else if (dbm_telemetry == 9)
  {
    dbm_telemetry = 30;
  }
  else if (dbm_telemetry == 10)
  {
    dbm_telemetry = 33;
  }
  else if (dbm_telemetry == 11)
  {
    dbm_telemetry = 37;
  }
  else if (dbm_telemetry == 12)
  {
    dbm_telemetry = 40;
  }
  else if (dbm_telemetry == 13)
  {
    dbm_telemetry = 43;
  }
  else if (dbm_telemetry == 14)
  {
    dbm_telemetry = 47;
  }
  else if (dbm_telemetry == 15)
  {
    dbm_telemetry = 50;
  }
  else if (dbm_telemetry == 16)
  {
    dbm_telemetry = 53;
  }
  else if (dbm_telemetry == 17)
  {
    dbm_telemetry = 57;
  }
  else if (dbm_telemetry == 18)
  {
    dbm_telemetry = 60;
  }

  debugSerial.println(dbm_telemetry);
}

void send_serial_data()
{
  debugSerial.print(F("Age "));
  debugSerial.println(gps.location.age());
}
