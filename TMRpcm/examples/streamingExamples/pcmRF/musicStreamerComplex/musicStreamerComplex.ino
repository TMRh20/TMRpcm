#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include "printf.h"
#include <SD.h>
#include <TMRpcm.h>

#define SD_ChipSelectPin 53

// Radio pipe addresses for the 2 nodes to communicate.

const uint64_t pipes[3] = { 0x544d526832LL, 0x544d52687CLL };

RF24 radio(48,49);


char* file1[14] = {"beeps/beep-6","beeps/beep-7","beeps/beep-8","beeps/beep-21"
,"beeps/beep-22","beeps/beep-23","beeps/button27","beeps/button28","beeps/button29"
,"beeps/button30","beeps/button45","beeps/button46","beeps/button48"};
int lastDev = 0;

TMRpcm tmrpcm;
pcmRF pcmrf(48,49,11);//CE Pin,CS Pin,Speaker Pin

void setup(){

  tmrpcm.speakerPin = 11; 
  Serial.begin(115200);
  printf_begin();
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");
    return;   // don't do anything more: 
  }else{
  Serial.println("SD ok");
  } 
  setRadio();  
  randomSeed(analogRead(A1));

}

int lastPlayed = 0;
boolean go = 0;

void loop(){      
  

  if(Serial.available()){    
    switch(Serial.read()){
     case 'd': if(pcmrf.isPlaying()){ pcmrf.stop();} tmrpcm.play("cat24"); break;
     case 'P': if(pcmrf.isPlaying()){ pcmrf.stop();}tmrpcm.play("temple"); break;
     case 'q': pcmrf.play("temple",0); break;
     case 'w': pcmrf.play("temple",1); break;
     case 'e': pcmrf.play("catfish",0);break;
     case '1': pcmrf.play("dix8",1);break;
     case '2': pcmrf.play("dix11",0);break;
     case '3': pcmrf.play("dix16",1);break;
     case '4': pcmrf.play("soc11",0);break;
     case '5': pcmrf.play("soc22",1);break;
     case 'p': tmrpcm.pause(); break;
     case 't': if(pcmrf.isPlaying()){ pcmrf.stop();} tmrpcm.play("catfish"); break;
     case 'S': tmrpcm.stopPlayback(); break;
     case '=': tmrpcm.volume(1); break;
     case '-': tmrpcm.volume(0); break;
     case 'g': go = !go; break;
     default: break;
    }
  }
  
  
if(go){

  if(tmrpcm.isPlaying() == 0 && pcmrf.isPlaying() == 0 ){

  int who = random(3); //if(who==lastDev){delay(35);} lastDev = who; //delay of 5 or 25??
  int which = random(1,3);
  delay(random(10));
  //delay(5);
  if(who < 2){ pcmrf.play(file1[which],who);}else
  if(who == 2){ tmrpcm.play(file1[which]);}
  
  
  }
}

  
}



