
#ifndef pcmRF_h   // if x.h hasn't been included yet...
#define pcmRF_h   //   #define this so the compiler knows it has been included

#include <Arduino.h>
#include <TMRpcm.h>


#if defined (ENABLE_RF)


	class RF24;

	class pcmRF
	{
	 public:
	 	pcmRF( RF24& _radio);
		void play(char* filename, byte device);
		void broadcast(byte device);
		boolean isPlaying();
		void stop();
		void begin();
	 private:

	};

#endif
#endif