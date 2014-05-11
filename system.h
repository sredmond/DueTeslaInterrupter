#ifndef __SYSTEM_H
#include <Arduino.h> //<Arduino.h> is the default 

//Determines whether to log MIDI events to Serial or LCD
const bool useSerial = true;
const bool useLCD = true;

/*
REG_PIOC_OER = REGister - Parallel Input/Output C - Output Enable Register
REG_PIOC_SODR = REGister - Parallel Input/Output C - Set Output Data Register
REG_PIOC_CODR = REGister - Parallel Input/Output C - Clear Output Data Register
*/
//Macros for performance-intensive one-liners
#define setupLED() REG_PIOC_OER |= (1u << 28) //Sets Pin 3 as Output
#define ledOn() REG_PIOC_SODR = (1u << 28) //Turns on Pin 3
#define ledOff() REG_PIOC_CODR = (1u << 28) //Turns off Pin 3

//Function prototypes
void blinkLED(); //Blink the LED

#define __SYSTEM_H
#endif
