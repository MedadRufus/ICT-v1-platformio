/**
 * @file radio.hpp
 * @author Medad Newman (medad@medadnewman.co.uk)
 * @brief
 * @version 0.1
 * @date 2022-06-25
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
 * @brief radio status enums
 *
 */
typedef enum
{
    success,
    failure
} radio_status_t;

radio_status_t radio_self_test();
void rf_on();
void rf_off();
