#include "handlers.h"
#include "system.h"
#include "DueTimer.h"
#include "LCD.h"

//Array mapping MIDI pitch values to note frequencies (in Hz)
const double frequencyTable[] = {8.1757989156,8.6619572180,9.1770239974,10.3008611535,10.3008611535,10.9133822323,11.5623257097,12.2498573744,12.9782717994,13.7500000000,14.5676175474,15.4338531643,16.3515978313,17.3239144361,18.3540479948,19.4454364826,20.6017223071,21.8267644646,23.1246514195,24.4997147489,25.9565435987,27.5000000000,29.1352350949,30.8677063285,32.7031956626,34.6478288721,36.7080959897,38.8908729653,41.2034446141,43.6535289291,46.2493028390,48.9994294977,51.9130871975,55.0000000000,58.2704701898,61.7354126570,65.4063913251,69.2956577442,73.4161919794,77.7817459305,82.4068892282,87.3070578583,92.4986056779,97.9988589954,103.8261743950,110.0000000000,116.5409403795,123.4708253140,130.8127826503,138.5913154884,146.8323839587,155.5634918610,164.8137784564,174.6141157165,184.9972113558,195.9977179909,207.6523487900,220.0000000000,233.0818807590,246.9416506281,261.625565300,277.182630976,293.664767917,311.126983722,329.627556912,349.228231433,369.994422711,391.995435981,415.304697579,440.000000,466.16376151,493.88330125,523.25113060,554.36526195,587.32953583,622.25396744,659.25511382,698.45646286,739.98884542,783.9908719635,830.6093951599,880.0000000000,932.3275230362,987.7666025122,1046.5022612024,1108.7305239075,1174.6590716696,1244.5079348883,1318.5102276515,1396.9129257320,1479.9776908465,1567.9817439270,1661.2187903198,1760.0000000000,1864.6550460724,1975.5332050245,2093.0045224048,2217.4610478150,2349.3181433393,2489.0158697766,2637.0204553030,2793.8258514640,2959.9553816931,3135.9634878540,3322.4375806396,3520.0000000000,3729.3100921447,3951.0664100490,4186.0090448096,4434.9220956300,4698.6362866785,4978.0317395533,5274.0409106059,5587.6517029281,5919.9107633862,5919.9107633862,6644.8751612791,7040.0000000000,7458.6201842894,7902.1328200980,8372.0180896192,8869.8441912599,9397.2725733570,9956.0634791066,10548.0818212118,11175.3034058561,11839.8215267723,12543.8539514160};

//Array mapping MIDI pitch values to the Timer (if any) that is controlling that note
DueTimer activeTimers[] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}; //Each slot corresponds to a particular pitch

/*
 * Handle note-on messages
 * Precondition: 1 <= channel <= 16, 0 <= pitch < 128, 0 <= velocity < 128
 */
void HandleNoteOn(byte channel, byte pitch, byte velocity)
{
  //If zero velocity, turn the note off
  if (velocity == 0) {
    HandleNoteOff(channel, pitch, velocity);
    return;
  }
  
  double freq = frequencyTable[pitch];
  if (useSerial) {
    //Print a message to Serial Monitor
    Serial.println("NOTE ON");
    Serial.println("Channel: " + String(channel) + ", Pitch: " + String(pitch) + ", Velocity: " + String(velocity));
    Serial.println("Computed Frequency: "+String(freq)+"Hz");
  }
  if (useLCD) {
    //Print a message to the LCD
    clearLCD();
    printToLCD(0,0,"On F="+String(freq)+"Hz");
    printToLCD(0,1,"P="+String(pitch)+", V="+String(velocity));
  }
  ledOff(); //just to be safe
  
  DueTimer timer = Timer.getAvailable();
  timer.attachInterrupt(blinkLED);
  timer.setFrequency(freq);
  timer.start();
  activeTimers[pitch] = timer;
}

/*
 * Handle note-off messages
 * If sustain is not on, kill the note
 * Otherwise, toggle the flag that puts the timers
 * into sustain mode
 */
void HandleNoteOff(byte channel, byte pitch, byte velocity) 
{
  if (useSerial) {
    //Print a message to Serial Monitor
    Serial.println("NOTE OFF");
    Serial.println("Channel: " + String(channel) + ", Pitch: " + String(pitch) + ", Velocity: " + String(velocity));
  }
  if (useLCD) {
    //Print a message to the LCD
    clearLCD();
    printToLCD(0,0,"Off");
    printToLCD(0,1,"P="+String(pitch)+", V="+String(velocity));
  }
  DueTimer timer = activeTimers[pitch]; //Error prone, if a NoteOff is sent for some note that wasn't turned on with NoteOn
  timer.detachInterrupt(); //Also stops the timer
  activeTimers[pitch] = NULL; //Clear the spot in the activeTimers array
}

void HandleStop() {
  if (useSerial) {
    Serial.println("STOP");
  }
  if (useLCD) {
    //Print to LCD
    clearLCD();
    printToLCD(0,0,"Stop!");
    printToLCD(0,1,"Hammertime");
  }
  Timer0.stop();
  Timer1.stop();
  Timer2.stop();
  Timer3.stop();
  Timer4.stop();
  Timer5.stop();
  Timer6.stop();
  Timer7.stop();
  Timer8.stop();
}

//Potentially leaves free-running timers
void HandleContinue() {
  if (useSerial) {
    Serial.println("CONTINUE");
  }
  if (useLCD) {
    //Print to LCD
    clearLCD();
    printToLCD(0,0,"Restarting...");
    printToLCD(0,1,"(Could crash)");
  }
  Timer0.start();
  Timer1.start();
  Timer2.start();
  Timer3.start();
  Timer4.start();
  Timer5.start();
  Timer6.start();
  Timer7.start();
  Timer8.start();
}

//Reset EVERYTHING
void HandleSystemReset() {
  if (useSerial) {
    Serial.println("SYSTEM RESET");
  }
  if (useLCD) {
    //Print to LCD
    clearLCD();
    printToLCD(0,0,"Resetting");
    printToLCD(0,1,"System");
  }
  Timer0.detachInterrupt();
  Timer1.detachInterrupt();
  Timer2.detachInterrupt();
  Timer3.detachInterrupt();
  Timer4.detachInterrupt();
  Timer5.detachInterrupt();
  Timer6.detachInterrupt();
  Timer7.detachInterrupt();
  Timer8.detachInterrupt();
  activeTimers = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
}
