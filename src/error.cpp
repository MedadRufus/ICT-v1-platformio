#include <IWatchdog.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN PC13
#endif

void fault_action()
{
  while (1)
  {
    // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, HIGH);
    // wait for a second
    delay(1000);
    IWatchdog.reload();

    // turn the LED off by making the voltage LOW
    digitalWrite(LED_BUILTIN, LOW);
    // wait for a second
    delay(1000);
    IWatchdog.reload();
    t
  }
}
