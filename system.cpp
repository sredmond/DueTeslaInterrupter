#include "system.h"

const int delayMicros = 20; //The amount of time to delay between rise and fall
void blinkLED()
{
  ledOn();
  delayMicroseconds(delayMicros);
  ledOff();
  delayMicroseconds(delayMicros);
}
