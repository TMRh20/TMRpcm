/*
This sketch demonstrates generating a standards compliant WAV file that can be played on any device that
supports WAV format files.

WAV files are a fairly simple audio format. Creating digital ramps and writing the data to a file
will allow the generation of tones, pseudo sine waves, and computer generated audio in general.

This sketch demonstrates its use with the standard SD lib, but also includes commented code demonstrating
usage with the SDFAT lib. Comment out the SD sections and uncomment the SDFAT sections to enable it.
Uncomment #define SDFAT in pcmConfig.h to enable the alternate library.
*/

#include <TMRpcm.h>           //  also need to include this library...

  /*Using SD lib*/
#include <SD.h>
#include <SPI.h>
File uFile;

  /* Using SDFAT lib */
//#include <SdFat.h>
//SdFat sd;
//SdFile uFile;

#define SD_ChipSelectPin 53  //example uses hardware SS pin 53 on Mega2560
//#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328, can use other pins

TMRpcm audio;   // create an object for use in this sketch
char *filename = "myWav.wav"; //The name of the file that will be generated

void setup(){

  audio.speakerPin = 11; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
  pinMode(12,OUTPUT);  //Pin pairs: 9,10 Mega: 5-2,6-7,11-12,46-45
  Serial.begin(115200);
  
/* Standard SD Lib */
  pinMode(13,OUTPUT); //LED Connected to analog pin 0
  if (!SD.begin(SD_ChipSelectPin)) {  return;
  }else{Serial.println("SD OK"); }

 /* SDFAT Lib */
//  if (!sd.begin(53, SPI_FULL_SPEED)) { return; 
//  }else{Serial.println("SD OK");}
 
 }

//This function generates some simple ramping signals to write to a wav file
void writeSimpleDigitalTones(){
  /*Standard SD Lib */
  uFile = SD.open(filename, FILE_WRITE);
  if(!uFile){ Serial.print("Failed to open"); return; } 
  
  /*SDFAT Lib*/
//  uFile.open(filename, O_WRITE);
//  uFile.seekSet(44);
// if(!uFile.isOpen()){ Serial.print("Failed to open"); return; }  
  
//  Generate some waves, ramping up then down 6 times and write the data to the wav file
  int rampVal = 25;
  
  for(int g=0; g<6; g++){                 //Do this 3 times
    for(int h=0; h<1000; h++){             //Do this 100 Times
        for(int i=1; i<254; i+=rampVal){  //Count from 1 to 254, stepping up by rampVal
	    uFile.write(i);               //Write the ramping values to the wav file
        }
        for(int i=254; i>10; i-=rampVal){ //Ramp the values down again
            uFile.write(i);               //Write the ramping values to the wav file
        }
     }
     Serial.print("Wrote "); Serial.print(g); Serial.println(" of 5");
     rampVal = rampVal + 20;    //Adjust the ramping value to create different tones
  }

  uFile.close();
  Serial.print("WAV file written, ready to finalize."); 
}


void loop(){  

  
  if(Serial.available()){   
    switch(Serial.read()){
      //This will generate a WAV file with headers and no data, then write simple 6 tones to it
    case 'w': audio.createWavTemplate(filename,16000);
              writeSimpleDigitalTones();
              break;
      //This function will write file size information to the file to complete it
      //Run this function before playing a file. It can be run      
    case 'f': audio.finalizeWavTemplate(filename); 
              Serial.println("Finalized. Send 'g' to play the file.");
              break;
    case 'g': audio.play(filename); break;
    case 'p': audio.pause(); break;
    case '?': if(audio.isPlaying()){ Serial.println("A wav file is being played");} break;
    case 'S': audio.stopPlayback(); break;
    case 'D': audio.disable();  break;
    case '=': audio.volume(1); break;
    case '-': audio.volume(0); break;
    default: break;
    }
  }
  





}
