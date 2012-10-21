/*Library by TMRh20 2012*/

#include <Arduino.h>



#ifndef tmrPCM_h
#define tmrPCM_h


class TMRpcm
{
 public:
	TMRpcm();
	void play(char* filename);
	void stopPlayback();
	void volume(int vol);
	void disable();
	void pause();
	boolean wavInfo(char* filename);
	int speakerPin;
	boolean pwmMode;
	boolean isPlaying();

 private:
 	void startPlayback();
 	int SAMPLE_RATE;


};


#endif