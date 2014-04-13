
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



/****************** GENERAL USER DEFINES *********************************
 See https://github.com/TMRh20/TMRpcm/wiki for info on usage

   Override the default size of the buffers (MAX 254). There are 2 buffers, so memory usage will be double this number
   Defaults to 64bytes for Uno etc. 254 for Mega etc. note: In multi mode there are 4 buffers*/
//#define buffSize 128  //must be an even number

  /* Uncomment to run the SD card at full speed (half speed is default for standard SD lib)*/
#define SD_FULLSPEED

  /* HANDLE_TAGS - This options allows proper playback of WAV files with embedded metadata*/
//#define HANDLE_TAGS

  /*Ethernet shield support etc. The library outputs on both timer pins, 9 and 10 on Uno by default. Uncommenting this
    will disable output on the 2nd timer pin and should allow it to function with shields etc that use Uno pin 10 (TIMER1 COMPB).*/
//#define DISABLE_SPEAKER2

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
      when PWM is enabled or disabled. This option is autodetected unless defined here*/
//#define rampMega

   /* Initial implementation for recording of WAV files to SD card via a microphone or input connected to an analog pin
   SdFat library is recommended
   Requires a class 4 card minimum, buffSize may need to be increased to 254 if audio is skipping etc.
   Depending on the card, can take a few seconds for recording to start
   																									*/
//#define ENABLE_RECORDING
	// Amount of space to pre-allocate for recording
//	#define BLOCK_COUNT 10000UL  // 10000 = 500MB   2000 = 100MB

//*********************** Radio (NRF24L01+) Streaming *********************

   /* Comment or Uncomment to en/disable RF streaming of wav files. Make sure
to Comment if not using radio                                                  */

//#define ENABLE_RF

  /* Uncomment this line to disable all standard features except RF playback.
     This will minimize resource usage if not playing or recording files locally */
//#define RF_ONLY

//*************************************************************************

#endif