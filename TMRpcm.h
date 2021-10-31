/*Library by TMRh20 2012-2014

Contributors:

  ftp27 (GitHub) - setVolume(); function and code
  muessigb (GitHub) - metatata (ID3) support

*/

#ifndef TMRpcm_h   // if x.h hasn't been included yet...
#define TMRpcm_h   //   #define this so the compiler knows it has been included

#include <Arduino.h>
#include <pcmConfig.h>
#include <pcmRF.h>
#if !defined (SDFAT)
    #include <SD.h>
#else
    #include <SdFat.h>
#endif

#if defined (ENABLE_RF)
    class RF24;
#endif

class TMRpcm
{
 public:
    //TMRpcm();
    //*** General Playback Functions and Vars ***
    #if !defined (ENABLE_MULTI)
    void play(char* filename, unsigned long seekPoint=0);
    void play(const __FlashStringHelper* FS, unsigned long seekPoint=0);
    #endif
    void stopPlayback();
    void volume(char vol);
    void setVolume(char vol);
    void disable();
    void pause();
    void quality(boolean q);
    void loop(boolean set);
    byte speakerPin;
    boolean isPlaying();
    uint8_t CSPin;

    //*** Public vars used by RF library also ***
    boolean wavInfo(char* filename);
    boolean rfPlaying;
    unsigned int SAMPLE_RATE;

    //*** Advanced usage Vars ***
    byte listInfo(char* filename, char *tagData, byte infoNum);
    byte id3Info(char* filename, char *tagData, byte infoNum);
    byte getInfo(char* filename, char* tagData, byte infoNum);


    #if defined (ENABLE_MULTI)//Normal Mode
    //*** MULTI MODE **
        void quality(boolean q, boolean q2);
        void play(const __FlashStringHelper* FS, boolean which=0);
        void play(char* filename, boolean which=0);
        void play(char* filename, unsigned long seekPoint, boolean which);
        void play(const __FlashStringHelper* FS, unsigned long seekPoint, boolean which);
        boolean isPlaying(boolean which);
        void stopPlayback(boolean which);
        void volume(char upDown,boolean which);
        void setVolume(char vol, boolean which);
        void loop(boolean set, boolean which);
    #endif
    #if defined (MODE2)
            byte speakerPin2;
    #endif

    //*** Recording WAV files ***
    void createWavTemplate(char* filename,unsigned int sampleRate);
    void createWavTemplate(const __FlashStringHelper* FS, unsigned int sampleRate);
    void finalizeWavTemplate(char* filename);
    void finalizeWavTemplate(const __FlashStringHelper* FS);
    #if defined (ENABLE_RECORDING)
        void startRecording(char *fileName, unsigned int SAMPLE_RATE, byte pin, byte passThrough=0);
        void startRecording(const __FlashStringHelper* FS, unsigned int SAMPLE_RATE, byte pin, byte passThrough=0);
        void stopRecording(char *fileName);
        void stopRecording(const __FlashStringHelper* FS);
    #endif

 private:

    void setPin();
    void timerSt();
    unsigned long fPosition();
    unsigned int resolution;
    byte lastSpeakPin;
    byte metaInfo(boolean infoType, char* filename, char* tagData, byte whichInfo);
    boolean seek(unsigned long pos);
    boolean ifOpen();
    unsigned int FSHlength(const __FlashStringHelper * FSHinput);

    #if !defined (SDFAT)
        boolean searchMainTags(File xFile, char *datStr);
    #else
        unsigned long searchMainTags(SdFile xFile, char *datStr);
    #endif

    #if defined (ENABLE_MULTI)
        void ramp(boolean wBuff);
    #endif

    #if defined (MODE2)
        void setPins();
    #endif
};

#endif



