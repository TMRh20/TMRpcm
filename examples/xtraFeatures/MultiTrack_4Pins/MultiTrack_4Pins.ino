
/*
This sketch demonstrates playing 2 audio tracks simultaneously using two timers. (Mega etc ONLY)

Steps:
1. Edit pcmConfig.h
    a: Uncomment #define ENABLE_MULTI
    b: Uncomment #define MODE2

2. Usage is as below. See https://github.com/TMRh20/TMRpcm/wiki/Advanced-Features#wiki-multi-mode for
   additional informaiton.

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
  
  audio.speakerPin2 = 5; //Enable the second set of timer pins
  pinMode(2,OUTPUT);
  
  Serial.begin(115200);
  
  if (!SD.begin(SD_ChipSelectPin)) {  
    return;
  }else{
    Serial.println("SD OK"); 
  }
  
}


void loop() {
  
    if(Serial.available()){                          //Send commands over serial to play
      switch(Serial.read()){
        case '2': audio.play("tr1.wav",0); break;    //Play tr1.wav on pins 11,12
        case '3': audio.play("tr1.wav",1); break;    //Play tr1.wav on pins 5,2
        case '4': audio.play("tr2.wav",0); break;    //Play tr2.wav on pins 11,12
        case '5': audio.play("tr2.wav",1); break;    //Play tr2.wav on pins 5,2
        case '6': audio.play("tr1.wav",15,0); break; //Play tr1.wav starting at 15 seconds on pins 11,12
        case '7': audio.play("tr1.wav",30,1); break; //Play tr1.wav starting at 30 seconds, pins 5,2
        case '=': audio.volume(1); break;            //Increase volume by 1
        case '-': audio.volume(0); break;            //Decrease volume by 1
        case 's': audio.stopPlayback(0); break;      //Stop playback on output 0
        case 'S': audio.stopPlayback(1); break;      //Stop playback on output 1
        case 'x': audio.stopPlayback(); break;       //Stop all playback
        
      }
    }
}



