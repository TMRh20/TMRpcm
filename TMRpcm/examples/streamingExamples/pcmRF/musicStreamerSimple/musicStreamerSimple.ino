#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include "printf.h"
#include <SD.h>
#include <TMRpcm.h>

#define SD_ChipSelectPin 53 //Arduino Mega

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[3] = { 0x544d526832LL, 0x544d52687CLL };

RF24 radio(48,49);

TMRpcm tmrpcm;
pcmRF pcmrf(48,49,11); //CE Pin, CS Pin, speaker Pin

void setup(){
  
  Serial.begin(9600);
  printf_begin();
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");
    return;   // don't do anything more: 
  }
  setRadio();
}


void loop(){     
  

  if(Serial.available()){    
    switch(Serial.read()){
     case 'P': if(pcmrf.isPlaying()){ pcmrf.stop();}tmrpcm.play("temple"); break;
     case 'q': pcmrf.play("temple",0); break;
     case 'e': pcmrf.play("catfish",0);break;
     case '4': pcmrf.play("soc11",0);break;
     case 't': if(pcmrf.isPlaying()){ pcmrf.stop();} tmrpcm.play("catfish"); break;
     default: break;
    }
  }  

}




