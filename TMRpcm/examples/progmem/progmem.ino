
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

prog_char wav_1[] PROGMEM = "beeps/beep-8";
prog_char wav_2[] PROGMEM = "warning.wav";
prog_char wav_3[] PROGMEM = "verified.wav";
prog_char wav_4[] PROGMEM = "engage.wav";
prog_char wav_5[] PROGMEM = "link.wav";
//etc...

PROGMEM const char *wav_table[] PROGMEM = 
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
        case 'P': strcpy_P(wavFile, (char*)pgm_read_word(&(wav_table[0])));
                wav.play(wavFile);
                break;
        case 'd': strcpy_P(wavFile, (char*)pgm_read_word(&(wav_table[1])));
                wav.play(wavFile);
                break;
        case 't': strcpy_P(wavFile, (char*)pgm_read_word(&(wav_table[2])));
                wav.play(wavFile);
                break; 
        //etc.. etc...
      }
  }
}



