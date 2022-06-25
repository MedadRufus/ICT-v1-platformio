/**
 * @file radio.cpp
 * @author Medad Newman (medad@medadnewman.co.uk)
 * @brief
 * @version 0.1
 * @date 2022-06-25
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "radio.hpp"
#include "Arduino.h"
#include "si5351.h"
#include "config.hpp"

extern Si5351 si5351;

radio_status_t radio_self_test()
{
    radio_status_t status;
    digitalWrite(SI5351_ENABLE_PIN, HIGH);
    delay(2);
    bool i2c_found = si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
    digitalWrite(SI5351_ENABLE_PIN, LOW);

    if (!i2c_found)
    {
        return failure;
    }
    else
    {
        return success;
    }
}

void rf_on()
{
    digitalWrite(SI5351_ENABLE_PIN, HIGH);
    delay(2);
    si5351.init(SI5351_CRYSTAL_LOAD_8PF, SI5351_TCXO_FREQUENCY, 0);
    si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA); // Set for max power if desired. Check datasheet.
    si5351.output_enable(SI5351_CLK0, 0);                 // Disable the clock initially
}

void rf_off()
{
    digitalWrite(SI5351_ENABLE_PIN, LOW);
}
