#include "utils.hpp"
#include "Arduino.h"

float clamp(float n, float lower, float upper)
{
    return max(lower, min(n, upper));
}
