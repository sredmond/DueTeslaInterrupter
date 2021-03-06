/*
  DueTimer.h - DueTimer header file, definition of methods and attributes...
  For instructions, go to https://github.com/ivanseidel/DueTimer

  Created by Ivan Seidel Gomes, March, 2013.
  Modified by Philipp Klaus, June 2013.
  Released into the public domain.
*/
#ifdef __arm__

#ifndef DueTimer_h
#define DueTimer_h

#include <Arduino.h>

#include <inttypes.h>

/*
	This fixes compatibility for Arduono Servo Library.
	Uncomment to make it compatible.

	Note that:
		+ Timers: 0,2,3,4,5 WILL NOT WORK, and will
				  neither be accessible by Timer0,...
*/
// #define USING_SERVO_LIB	true

#ifdef USING_SERVO_LIB
	#warning "HEY! You have set flag USING_SERVO_LIB. Timer0, 2,3,4 and 5 are not available"
#endif

class DueTimer
{
protected:

	// Represents the timer id (index for the array of Timer structs)
	int timer;

	// Stores the object timer frequency
	// (allows to access current timer period and frequency):
	static double _frequency[9];

	// Picks the best clock to lower the error
	static uint8_t bestClock(double frequency, uint32_t& retRC);

public:
	struct Timer
	{
		Tc *tc;
		uint32_t channel;
		IRQn_Type irq;
	};

	static DueTimer getAvailable();

	// Store timer configuration (static, as it's fix for every object)
	static const Timer Timers[9];

	// Needs to be public, because the handlers are outside class:
	static void (*callbacks[9])();

	DueTimer(int _timer);
	DueTimer attachInterrupt(void (*isr)());
	DueTimer detachInterrupt();
	DueTimer start(long microseconds = -1);
	DueTimer stop();
	DueTimer setFrequency(double frequency);
	DueTimer setPeriod(long microseconds);

        double getFrequency();
	long getPeriod();
};

// Just to call Timer.getAvailable instead of Timer::getAvailable() :
extern DueTimer Timer;

extern DueTimer Timer1;
// Fix for compatibility with Servo library
#ifndef USING_SERVO_LIB
	extern DueTimer Timer0;
	extern DueTimer Timer2;
	extern DueTimer Timer3;
	extern DueTimer Timer4;
	extern DueTimer Timer5;
#endif
extern DueTimer Timer6;
extern DueTimer Timer7;
extern DueTimer Timer8;

#endif
#else
	#error Oops! Trying to include DueTimer on another device?
#endif
