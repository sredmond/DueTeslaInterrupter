#include "system.h"
#include "DueTimer.h"
#include "MIDI.h"
#include "LCD.h"
#include "handlers.h"

void setup()
{
  setupLED(); //Configure pin 3 for output
  ledOff(); //Just in case the Due holds the I/O pins high by default  
 
  //Setup is inexpensive
  Serial.begin(9600); 
  setupLCD(); //Setup the Liquid Crystal Display
  
  //Print warning messages
  if (!useSerial) {
    Serial.println("Serial output is disabled");
  }
  if (!useLCD) {
    clearLCD();
    printToLCD(0,0,"LCD is disabled");
  }
  
  // Initiate MIDI communications, listen to all channels
  MIDI.begin(MIDI_CHANNEL_OMNI);  
   
  // Setup MIDI callback functions
  MIDI.setHandleNoteOn(HandleNoteOn); //Only need the name of the function
  MIDI.setHandleNoteOff(HandleNoteOff);
  MIDI.setHandleStop(HandleStop);
  MIDI.setHandleContinue(HandleContinue);
  MIDI.setHandleSystemReset(HandleSystemReset);
  
  while (1) //Loop
  {
    MIDI.read(); //Read any incoming MIDI signals
  }
}

//We avoid using the loop method, and instead loop inside setup
void loop(){}
