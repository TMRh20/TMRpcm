/*Library by TMRh20 2012-2013*/

#ifndef TMRpcm_h   // if x.h hasn't been included yet...
#define TMRpcm_h   //   #define this so the compiler knows it has been included

#include <Arduino.h>
#include <pcmConfig.h>
#include <pcmRF.h>
#include <SD.h>
#include <SdFat.h>


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
	byte speakerPin;
	boolean wavInfo(char* filename);
	boolean isPlaying();
	boolean rfPlaying;
	unsigned int SAMPLE_RATE;
	byte listInfo(char* filename, char *tagData, byte infoNum);
	byte id3Info(char* filename, char *tagData, byte infoNum);
	byte getInfo(char* filename, char* tagData, byte infoNum);
	#if defined (MODE2)
		byte speakerPin2;
	#endif
	#if defined (ENABLE_MULTI)
		void quality(boolean q, boolean q2);
		void play(char* filename, boolean which);
		void play(char* filename, unsigned long seekPoint, boolean which);
		boolean isPlaying(boolean which);
		void stopPlayback(boolean which);
		void volume(char upDown,boolean which);
		void setVolume(char vol, boolean which);
		//void ramp(boolean wBuff);
	#else
		void play(char* filename, unsigned long seekPoint);
	#endif
	void createWavTemplate(char* filename,unsigned int sampleRate);
	void finalizeWavTemplate(char* filename);

 private:
	byte lastSpeakPin;
	//unsigned int resolution;
	void setPin();
	void timerSt();
	#if defined (ENABLE_MULTI)
		void ramp(boolean wBuff);
	#endif
	#if defined (MODE2)
		void setPins();
	#endif
	byte metaInfo(boolean infoType, char* filename, char* tagData, byte whichInfo);
	boolean seek(unsigned long pos);
	boolean ifOpen();
	unsigned long fPosition();
	#if !defined (SDFAT)
		boolean searchMainTags(File xFile, char *datStr);
	#else
		unsigned long searchMainTags(SdFile xFile, char *datStr);
	#endif
};

#endif



