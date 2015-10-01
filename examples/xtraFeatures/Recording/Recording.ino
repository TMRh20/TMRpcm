
/*
This sketch demonstrates recording of standard WAV files that can be played on any device that supports WAVs. The recording
uses a single ended input from any of the analog input pins. Uses AVCC (5V) reference currently.

Requirements:
Class 4 or 6 SD Card
Audio Input Device (Microphone, etc)
Arduino Uno,Nano, Mega, etc.

Steps:
1. Edit pcmConfig.h
    a: On Uno or non-mega boards, #define buffSize 128. May need to increase.
    b: Uncomment #define ENABLE_RECORDING and #define BLOCK_COUNT 10000UL

2. Usage is as below. See https://github.com/TMRh20/TMRpcm/wiki/Advanced-Features#wiki-recording-audio for
   additional informaiton.

Notes: Recording will not work in Multi Mode.
Performance is very dependant on SD write speed, and memory used.
Better performance may be seen using the SdFat library. See included example for usage.
Running the Arduino from a battery or filtered power supply will reduce noise.
*/

#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>

#define SD_ChipSelectPin 53  //example uses hardware SS pin 53 on Mega2560
//#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328, can use other pins

TMRpcm audio;   // create an object for use in this sketch 

void setup() {
  
  audio.speakerPin = 11; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
  pinMode(12,OUTPUT);  //Pin pairs: 9,10 Mega: 5-2,6-7,11-12,46-45
  
  Serial.begin(115200);
  
  if (!SD.begin(SD_ChipSelectPin)) {  
    return;
  }else{
    Serial.println("SD OK"); 
  }
  // The audio library needs to know which CS pin to use for recording
  audio.CSPin = SD_ChipSelectPin;
}


void loop() {
  
    if(Serial.available()){                          //Send commands over serial to play
      switch(Serial.read()){
        case 'r': audio.startRecording("test.wav",16000,A0); break;    //Record at 16khz sample rate on pin A0
        case 'R': audio.startRecording("test.wav",16000,A0,1); break;  //Record, but with passthrough to speaker.
        case 't': audio.startRecording("test.wav",16000,A0,2); break;  //Do not record. Output direct to speaker
        							       //Note: If samples are dropped before writing, it
        							       //      will not be heard in passthrough mode
        case 's': audio.stopRecording("test.wav"); break;              //Stop recording
        case 'p': audio.play("test.wav"); break;                       //Play the recording 
        case '=': audio.volume(1); break;                              //Increase volume by 1. Does not affect recording
        case '-': audio.volume(0); break;                              //Decrease volume by 1. Does not affect recording
        case 'S': audio.stopPlayback(); break;                         //Stop all playback
        
      }
    }
}