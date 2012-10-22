

#ifndef pcmRF_h
#define pcmRF_h




class pcmRF
{
 public:
	pcmRF(int CE, int CS);
	void playRF(char* filename, int device);
	boolean isPlaying();
	void stop();

 private:
 	//void stopRfPlay();

};

void checkRF();
void buffRX();
void loadSingleBuffer(boolean wBf);


#endif