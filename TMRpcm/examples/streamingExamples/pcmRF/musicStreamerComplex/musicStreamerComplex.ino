#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include "printf.h"
#include <SD.h>
#include <TMRpcm.h>
#include <pcmRF.h>

#define SD_ChipSelectPin 53
//#define SD_ChipSelectPin 4

int speakerPin = 11;
//int speakerPin = 9;


// Radio pipe addresses for the 2 nodes to communicate.

const uint64_t pipes[3] = { 0x544d526832LL, 0x544d52687CLL };

RF24 radio(48,49);


char* file1[14] = {"beeps/beep-6","beeps/beep-7","beeps/beep-8","beeps/beep-21"
,"beeps/beep-22","beeps/beep-23","beeps/button27","beeps/button28","beeps/button29"
,"beeps/button30","beeps/button45","beeps/button46","beeps/button48"};
int lastDev = 0;


TMRpcm tmrpcm;
pcmRF pcmrf(48,49);
void setup(){

  pinMode(speakerPin,OUTPUT);
  pinMode(A0,OUTPUT);  
  
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
     case 'q': pcmrf.playRF("temple",0); break;
     case 'w': pcmrf.playRF("temple",1); break;
     case 'e': pcmrf.playRF("catfish",0);break;
     case '1': pcmrf.playRF("dix8",1);break;
     case '2': pcmrf.playRF("dix11",0);break;
     case '3': pcmrf.playRF("dix16",1);break;
     case '4': pcmrf.playRF("soc11",0);break;
     case '5': pcmrf.playRF("soc22",1);break;
     case 'p': tmrpcm.pause(); break;
     case 't': if(pcmrf.isPlaying()){ pcmrf.stop();} tmrpcm.play("catfish"); break;
     case 'S': tmrpcm.stopPlayback(); break;
     case '=': tmrpcm.volume(1); break;
     case '-': tmrpcm.volume(0); break;
     case 'g': go = !go; break;
     //case 'R': digitalWrite
     default: break;
    }
  }
  
  
if(go){

  if(tmrpcm.isPlaying() == 0 && pcmrf.isPlaying() == 0 ){

  int who = random(3); //if(who==lastDev){delay(35);} lastDev = who; //delay of 5 or 25??
  int which = random(1,3);
  delay(random(10));
  //delay(5);
  if(who < 2){ pcmrf.playRF(file1[which],who);}else
  if(who == 2){ tmrpcm.play(file1[which]);}
  
  
  }
}

  
//if(go){
//  if(playing == 0 && rfPlaying == 0 ){ 
//    if(!sFile && !txFile){
//      //delay(300);
//      if(lastPlayed == 0){playRF(file1[0],0);
//      }else
//      if(lastPlayed == 1){playRF(file1[0],1);
//      }else
//      if(lastPlayed == 2){ play(file1[1]); lastPlayed = -1;}
//      lastPlayed++;
//    }
//  }
//}

}





