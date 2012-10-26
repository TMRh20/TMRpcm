/*Library by TMRh20
  Released into the public domain.*/
//#include <Arduino.h>


#ifndef TMRpcm_h   // if x.h hasn't been included yet...
#define TMRpcm_h   //   #define this so the compiler knows it has been included

//#include <RF24.h>

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
	boolean rfPlaying;
	unsigned long SAMPLE_RATE;


 private:
 	void startPlayback();



};

void buffSD();

#endif