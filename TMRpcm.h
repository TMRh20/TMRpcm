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
    void play(const char* filename, unsigned long seekPoint=0);
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
    boolean wavInfo(const char* filename );
    boolean rfPlaying;
    unsigned int SAMPLE_RATE;

    //*** Advanced usage Vars ***
    byte listInfo(const char* filename, char *tagData, byte infoNum);
    byte id3Info(const char* filename, char *tagData, byte infoNum);
    byte getInfo(const char* filename, char* tagData, byte infoNum);


    #if defined (ENABLE_MULTI)//Normal Mode
    //*** MULTI MODE **
        void quality(boolean q, boolean q2);
        void play(const __FlashStringHelper* FS, boolean which=0);
        void play(const char* filename, boolean which=0);
        void play(const char* filename, unsigned long seekPoint, boolean which);
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
    void createWavTemplate(const char* filename,unsigned int sampleRate);
    void createWavTemplate(const __FlashStringHelper* FS, unsigned int sampleRate);
    void finalizeWavTemplate(const char* filename);
    void finalizeWavTemplate(const __FlashStringHelper* FS);
    #if defined (ENABLE_RECORDING)
        void startRecording(const char *fileName, unsigned int SAMPLE_RATE, byte pin, byte passThrough=0);
        void startRecording(const __FlashStringHelper* FS, unsigned int SAMPLE_RATE, byte pin, byte passThrough=0);
        void stopRecording(const char *fileName);
        void stopRecording(const __FlashStringHelper* FS);
    #endif
    #if defined (SPEED_CONTROL)
        void speedUp(uint8_t amount = 1, bool persist = 0, uint8_t timerNo = 0);
        void speedDown(uint8_t amount = 1, bool persist = 0, uint8_t timerNo = 0);
        void setSpeed(uint32_t newSpeed, bool persist = 0, uint8_t timerNo = 0);
        uint32_t getSpeed(uint8_t timerNo = 0);
        bool speedPersist;
        uint16_t speedControlVar[4];
    #endif
    
 private:

    void setPin();
    void timerSt();
    unsigned long fPosition();
    int resolution;
    byte lastSpeakPin;
    byte metaInfo(boolean infoType, const char* filename, char* tagData, byte whichInfo);
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
    #if defined (ENABLE_RECORDING)
      uint8_t adcsra;
      uint8_t adcsrb;
      uint8_t admux;
    #endif
    
};

#endif
