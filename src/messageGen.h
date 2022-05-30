/*
   JT9 message generation
*/
void genMessage1()
{
  strcpy(message1, ""); // clear message string
  strcpy(message1, call);
  strcat(message1, " ");
  strcat(message1, loc6);
}

void genMessage2()
{
  alt_meters = gps.altitude.meters();
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
