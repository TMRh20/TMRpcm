#include <SD.h>                      // need to include the SD library
//#define SD_ChipSelectPin 53  //example uses hardware SS pin 53 on Mega2560
#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328
#include <TMRpcm.h>           //  also need to include this library...

TMRpcm tmrpcm;   // create an object for use in this sketch

unsigned long time = 0;

void setup(){

  tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc

  Serial.begin(115200);
  pinMode(A0,OUTPUT); //LED Connected to analog pin 0
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not

  }
  else{   
    Serial.println("SD ok");   
  }
  tmrpcm.play("music"); //the sound file "music" will play each time the arduino powers up, or is reset
}



void loop(){  

  //blink the LED manually to demonstrate music playback is independant of main loop
  if(tmrpcm.isPlaying() && millis() - time > 50 ) {      
      digitalWrite(A0,!digitalRead(A0));
      time = millis();    
  }else
  if(millis() - time > 500){     
    digitalWrite(A0,!digitalRead(A0)); 
    time = millis(); 
  }


  if(Serial.available()){    
    switch(Serial.read()){
    case 'd': tmrpcm.play("music"); break;
    case 'P': tmrpcm.play("temple"); break;
    case 't': tmrpcm.play("catfish"); break;
    case 'p': tmrpcm.pause(); break;
    case '?': if(tmrpcm.isPlaying()){ Serial.println("A wav file is being played");} break;
    case 'm': tmrpcm.pwmMode = !tmrpcm.pwmMode; break;
    case 'S': tmrpcm.stopPlayback(); break;
    case '=': tmrpcm.volume(1); break;
    case '-': tmrpcm.volume(0); break;
    default: break;
    }
  }

}