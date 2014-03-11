
/*
This sketch demonstrates playing a stereo audio track using a single timer.

16-24kHz sample rate recommended

Steps:
1. Edit pcmConfig.h
    a: On Uno or non-Mega boards, uncomment #define buffSize 128. May need to increase or lower sample rate.
    b: Uncomment #define STEREO_OR_16BIT

2. Usage is as below. See https://github.com/TMRh20/TMRpcm/wiki/Advanced-Features#wiki-stereo-and-16-bit-playback for
   additional information.

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
  
}


void loop() {
  
    if(Serial.available()){                        //Send commands over serial to play
      switch(Serial.read()){
        case '2': audio.play("tr1.wav"); break;    //Play stereo track tr1.wav on pins 11,12 (9,10 on Uno)
        case '3': audio.play("tr2.wav"); break;    //Play stereo track tr2.wav
        case '4': audio.play("tr2.wav",30); break; //Play stereo track tr2.wav starting at 30 seconds
        case '=': audio.volume(1); break;          //Increase volume by 1
        case '-': audio.volume(0); break;          //Decrease volume by 1
        case 'S': audio.stopPlayback(); break;     //Stop all playback
        
      }
    }
}



