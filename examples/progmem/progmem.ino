
/* Example sketch using progmem to store wav filenames. Uses 
program memory to store filenames instead of ram when playing
many different wav files. 
See http://arduino.cc/en/Reference/PROGMEM for more info.
Sketch below based on the above link
*/

#include <avr/pgmspace.h>
#include <TMRpcm.h>
#include <SPI.h>

TMRpcm wav;

#include <SD.h>
#define SD_ChipSelectPin 4

static const char wav_1[] PROGMEM = "beeps/beep-8";
static const char wav_2[] PROGMEM = "warning.wav";
static const char wav_3[] PROGMEM = "verified.wav";
static const char wav_4[] PROGMEM = "engage.wav";
static const char wav_5[] PROGMEM = "link.wav";


const char *wav_table[] = 
{
  wav_1,
  wav_2,
  wav_3,
  wav_4,
  wav_5
  //etc..
};

void setup(){
  
  Serial.begin(115200);
  wav.speakerPin = 9; pinMode(10,OUTPUT);
  
  if (!SD.begin(SD_ChipSelectPin)) { 
    Serial.println("SD fail");  
    return;
  }else{   Serial.println("SD ok"); }

}


void loop(){
  
  if(Serial.available()){
    
      char wavFile[33];
      
      switch(Serial.read()){      
        case 'P': strcpy_P(wavFile, wav_table[0]);
                wav.play(wavFile);
                break;
        case 'd': strcpy_P(wavFile, wav_table[1]);
                wav.play(wavFile);
                break;
        case 't': strcpy_P(wavFile, wav_table[3]);
                wav.play(wavFile);
                break;
        //etc.. etc...
      }
  }
}


