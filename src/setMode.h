/*
   TX modes
*/

void setModeJT9_1()
{
  symbol_count = JT9_SYMBOL_COUNT;
  tone_spacing = JT9_TONE_SPACING;
  tone_delay = JT9_DELAY;
  memset(tx_buffer, 0, 255); // Clears Tx buffer from previous operation.
  jtencode.jt9_encode(message1, tx_buffer);
}

void setModeJT9_2()
{
  symbol_count = JT9_SYMBOL_COUNT;
  tone_spacing = JT9_TONE_SPACING;
  tone_delay = JT9_DELAY;
  memset(tx_buffer, 0, 255); // Clears Tx buffer from previous operation.
  jtencode.jt9_encode(message2, tx_buffer);
}

void setModeWSPR()
{
  symbol_count = WSPR_SYMBOL_COUNT;
  tone_spacing = WSPR_TONE_SPACING;
  tone_delay = WSPR_DELAY;
  memset(tx_buffer, 0, 255); // Clears Tx buffer from previous operation.
  jtencode.wspr_encode(call, loc4, 10, tx_buffer);
}

void setModeWSPR_telem()
{
  symbol_count = WSPR_SYMBOL_COUNT;
  tone_spacing = WSPR_TONE_SPACING;
  tone_delay = WSPR_DELAY;
  memset(tx_buffer, 0, 255); // Clears Tx buffer from previous operation.
  jtencode.wspr_encode(call_telemetry, loc_telemetry, dbm_telemetry, tx_buffer);
}
