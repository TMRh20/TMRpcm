/* 
This example demonstrates how to use the SDFAT library for audio playback.
Read time and program space are reduced by using SDFAT directly

Requirements:
The SDFAT library must be installed. See http://code.google.com/p/sdfatlib/ 
The line #define SDFAT MUST be uncommented in pcmConfig.h

"error: 'File' has not been declared" means you need to read the above text
*/

#include <SdFat.h>
SdFat sd;

#define SD_ChipSelectPin 53  //use digital pin 4 on arduino Uno, nano etc, or can use other pins
#include <TMRpcm.h>          //  also need to include this library...
#include <SPI.h>

TMRpcm audio;   // create an object for use in this sketch

void setup(){

  audio.speakerPin = 11; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
  pinMode(12,OUTPUT); //Pin pairs: 9,10 Mega: 5-2,6-7,11-12,46-45  
  
  Serial.begin(115200);
  
  if (!sd.begin(53, SPI_FULL_SPEED)) { return; 
  }else{ Serial.println("SD OK"); }
    
  audio.play("song.wav");
}

void loop(){  

  
  if(Serial.available()){   
    switch(Serial.read()){
      case 'p': audio.play("song.wav"); break;
      default: break;
    }
  }


}
