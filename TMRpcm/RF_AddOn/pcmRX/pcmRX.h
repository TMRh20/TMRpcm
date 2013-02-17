
#include <Arduino.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>


#ifndef pcmRX_h
#define pcmRX_h


class pcmRX
{
 public:
	pcmRX(RF24 radio,int cs);
	void playRF(RF24 radi,char* filename, int device);
	boolean isPlaying();
	void stop();
	void pollForMusic();
	boolean txSel(byte sel, int timeoutDelay);
	void flushRX();
	int csPin;
	//void startTimer();
	//void resumeTimer();

 private:
 	//void stopRfPlay();
 	void startTimer();
	void resumeTimer();

};

void checkRF();
void loadSingleBuffer(boolean wBf);
void retryClose();
void noTun();
boolean txSelX(byte sel, int timeoutDelay);

#endif