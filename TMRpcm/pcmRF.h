
#ifndef pcmRF_h   // if x.h hasn't been included yet...
#define pcmRF_h   //   #define this so the compiler knows it has been included

#include <Arduino.h>
#include <TMRpcm.h>


#if defined (ENABLE_RF)

	class pcmRF
	{
	 public:
		pcmRF(int CE, int CS, int spkPin);
		void play(char* filename, int device);
		boolean isPlaying();
		void stop();

	 private:
	 	//void stopRfPlay();

	};

	void checkRF();
	void buffRX();
	void loadSingleBuffer(boolean wBf);


#endif

#endif