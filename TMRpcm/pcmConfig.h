
/*
This library was intended to be a simple and user friendly wav audio player using standard
Arduino libraries, playing bare-bones standard format WAV files.

Many of the extra features have been added due to user request, and are enabled
optionally only to preserve the out of the box simplicity and performance initially
intended.

Code/Updates: https://github.com/TMRh20/TMRpcm
Wiki: https://github.com/TMRh20/TMRpcm/wiki
Blog: https://tmrh20.blogspot.com/

*/


#ifndef pcmConfig_h   // if x.h hasn't been included yet...
#define pcmConfig_h   //   #define this so the compiler knows it has been included

#include <Arduino.h>
#include <TMRpcm.h>


/****************** GENERAL USER DEFINES *********************************
 See https://github.com/TMRh20/TMRpcm/wiki for info on usage

   Override the default size of the buffers (MAX 254). There are 2 buffers, so memory usage will be double this number
   Defaults to 64bytes for Uno etc. 254 for Mega etc. note: In multi mode there are 4 buffers*/
//#define buffSize 128  //must be an even number

  /* Uncomment to run the SD card at full speed (half speed is default for standard SD lib)**/
#define SD_FULLSPEED

  /* HANDLE_TAGS - This options allows proper playback of WAV files with embedded metadata*/
//#define HANDLE_TAGS

  /* Use 8-bit TIMER2 - If using an UNO, Nano, etc and need TIMER1 for other things*/
//#define USE_TIMER2

//#define debug
/****************** ADVANCED USER DEFINES ********************************
   See https://github.com/TMRh20/TMRpcm/wiki for info on usage

   /* Use the SDFAT library from http://code.google.com/p/sdfatlib/            */
//#define SDFAT

   /* MULTI Track mode currently allows playback of 2 tracks at once          */
//#define ENABLE_MULTI  //Using separate pins on a single 16-bit timer

   /* Enables 16-bit samples, which can be used for stereo playback, or to produce a
	   pseudo 16-bit output.                                                   */
//#define STEREO_OR_16BIT

   /* In Normal single track mode, MODE2 will modify the behavior of STEREO or 16BIT output
        With MODE2 off, stereo tracks will be played using 2 pins, for 2 speakers in non-complimentary mode (pin to ground)
	    With MODE2 on, stereo tracks will be played using 4 pins, for 2 speakers in complimentary mode
	  In MULTI dual track mode, MODE2 will use a second timer for the second track.
	    With MODE2 off, each track will use a separate pin, but the same timer
	                                                                           */
//#define MODE2  //Using separate 16-bit timers with up to 4 pins (Arduino Mega etc only)

   /* The library uses two different ramping methods to prevent popping sounds
      when PWM is enabled or disabled. This option is autodetected unless defined here
//#define rampMega

//*********************** Radio (NRF24L01+) Streaming *********************

   /* Comment or Uncomment to en/disable RF streaming of wav files. Make sure
to Comment if not using radio                                                  */

//#define ENABLE_RF

//*************************************************************************

#endif