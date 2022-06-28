/*
   JT9 message generation
*/

#include "messageGen.h"
#include "string.h"
#include <TinyGPS++.h>

extern char loc8[3];    // Last 2 digits of the 8-digit locator
extern char loc6[7];    // 6 digit gridsquare locator
char message1[14] = ""; // Message1 (13 char limit) for JT9
char message2[14] = ""; // Message2 (13 char limit) for JT9
extern char call[];     // JT9/WSPR Standard callsign
extern TinyGPSPlus gps;

void genMessage1()
{
  strcpy(message1, ""); // clear message string
  strcpy(message1, call);
  strcat(message1, " ");
  strcat(message1, loc6);
}

void genMessage2()
{
  double alt_meters = gps.altitude.meters();
  int speed_kmph = gps.speed.kmph();

  char myalt[5];
  char myspeed[3];

  itoa(alt_meters, myalt, 10);
  itoa(speed_kmph, myspeed, 10);

  strcpy(message2, ""); // clear message string
  strcpy(message2, loc8);
  strcat(message2, " ");
  strcat(message2, myalt);
  strcat(message2, " ");
  strcat(message2, myspeed);
}
