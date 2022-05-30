/*
   Timing Schedule - See readme file
*/

void TXtiming() // Timing
{
  // run additional scripts here to generate data prior to TX if there is a large delay involved.
  if ((timeStatus() == timeSet) && (second() == 0) && (minute() >= 1))
  {
    setGPStime();

    if ((minute() % 30 == 2) && (second() <= 2)) // Take data snapshot and start TX JT9 standard message
    {
      rf_on();
      loc8calc();      // Get position and update 4-char locator, 6-char locator and last 2 chars of 8-char locator
      genMessage1();   // Generate JT9 message type 1. Get callsign and 6-char locator
      genMessage2();   // Generate JT9 message type 2. Get altitude in meters and kmph speed. Send last 2 chars of 8-char locator, altitude and speed
      call_telem();    // Update WSPR telemetry callsign based on previous information : position and altitude in meters
      loc_dbm_telem(); // Update WSPR telemetry locator and dbm. Get temperature, voltage, speed in knots, GPS status and sats number
      telemetry_set = true;
      // send_serial_data();
      freq = JT9_FREQ;
      setModeJT9_1(); // set mode to JT9_1
      encode();       // begin radio transmission
      rf_off();
    }
    else if ((minute() % 30 == 3) && (second() <= 2) && (telemetry_set == true)) // TX JT9 telemetry
    {
      rf_on();
      // send_serial_data();
      freq = JT9_FREQ;
      setModeJT9_2(); // set mode to JT9_2
      encode();       // begin radio transmission
      rf_off();
    }
    else if ((minute() % 10 == 2) && (minute() % 30 != 2) && (second() <= 2)) // TX WSPR_1 standard message
    {
      rf_on();
      loc8calc();      // Get position and update 4-char locator, 6-char locator and last 2 chars of 8-char locator
      call_telem();    // Update WSPR telemetry callsign based on previous information : position and altitude in meters
      loc_dbm_telem(); // Update WSPR telemetry locator and dbm. Get temperature, voltage, speed in knots, GPS status and sats number
      telemetry_set = true;
      // send_serial_data();
      freq = WSPR_FREQ;
      setModeWSPR(); // set WSPR standard mode
      encode();      // begin radio transmission
      rf_off();
    }
    else if ((((minute() % 10 == 6) && (minute() % 30 != 6)) || (minute() % 30 == 4)) && (second() <= 2) && (telemetry_set == true)) // TX WSPR_2 standard message
    {
      rf_on();
      // send_serial_data();
      freq = WSPR_FREQ;
      setModeWSPR(); // set WSPR standard mode
      encode();      // begin radio transmission
      rf_off();
    }
    else if ((((minute() % 10 == 4) && (minute() % 30 != 4)) || (minute() % 30 == 6)) && (second() <= 2) && (telemetry_set == true)) // TX WSPR telemetry message
    {
      rf_on();
      // send_serial_data();
      freq = WSPR_FREQ;
      setModeWSPR_telem(); // set WSPR telemetry mode
      encode();            // begin radio transmission
      rf_off();
    }
    else if ((minute() % 10 == 8) && (second() <= 2)) // Check location age/validity and software GPS reset if necessary
    {
      if (gps.location.age() > 600000)
      {
        gps_reset();
        delay(500);
        setGPS_AirBorne(); // set GPS into airborne mode
        delay(500);
        gps_set_max_performance_mode();
        delay(500);
      }
    }
  }
}
