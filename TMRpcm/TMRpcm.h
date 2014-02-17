/*Library by TMRh20 2012-2013*/

#ifndef TMRpcm_h   // if x.h hasn't been included yet...
#define TMRpcm_h   //   #define this so the compiler knows it has been included



//****************** USER DEFINES ********************************

/* Override the default size of the buffers (MAX 255). There are 2 buffers, so memory usage will be double this number
   Defaults to 64bytes for Uno etc. 128 for Mega etc*/

//#define buffSize 128  //note: In multi mode there are 4 buffers

/* Uncomment to run the SD card at full speed (half speed is default for standard SD lib)**/

#define SD_FULLSPEED

/* New MULTI Track mode currently allows playback of 2 tracks at once
   See https://github.com/TMRh20/TMRpcm/wiki for info on usage */

//#define ENABLE_MULTI  //Using separate pins on a single 16-bit timer
//	#define MODE2  //Using separate 16-bit timers with up to 4 pins (Arduino Mega etc only)

/* Comment or Uncomment to en/disable RF streaming of wav files
   Make sure to Comment if not using radio*/

//#define ENABLE_RF

//*****************************************************************

#include <Arduino.h>
#include <pcmRF.h>


class TMRpcm
{
 public:
 	//TMRpcm();
	void play(char* filename);
	void stopPlayback();
	void volume(char vol);
	void setVolume(char vol);
	void disable();
	void pause();
	void quality(boolean q);
	int speakerPin;
	boolean wavInfo(char* filename);
	boolean isPlaying();
	boolean rfPlaying;
	unsigned int SAMPLE_RATE;
	#if defined (ENABLE_MULTI)
		void quality(boolean q, boolean q2);
		void play(char* filename, boolean which);
		boolean isPlaying(boolean which);
		void stopPlayback(boolean which);
		void volume(char upDown,boolean which);
		void setVolume(char vol, boolean which);
		//void ramp(boolean wBuff);
		int speakerPin2;
	#endif

 private:
	int lastSpeakPin;
	void setPin();
	void timerSt();
	void ramp(boolean wBuff);
	#if defined (MODE2)
		void setPins();
	#endif
};



#endif



