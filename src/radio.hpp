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

class radio
{
public:
    enum class radio_status_t
    {
        success,
        failure
    };
    radio();
    ~radio();
    radio_status_t radio_self_test();
    void rf_on();
    void rf_off();
};
