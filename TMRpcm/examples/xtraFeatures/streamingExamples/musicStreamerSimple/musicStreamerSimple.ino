
// This example demonstrates the use of the TMRpcm audio library in conjunction with 
// the RF24Audio library. The listening nodes are configured with the MINIMAL example file
// included with the RF24Audio library. This sketch utilizes the radio functionality of the
// TMRpcm library to stream audio from an SD card to those listening nodes.
//  
// Requirements: 
// 2 Arduino: Uno, Nano, Mega, etc
// 1 SD card with 8-bit, mono audio
// 1 Output device: Speaker, etc
// 2 NRF24L01 Radio Modules
//
// Transmitter Setup:
// 1. Connect SD card and radio to Arduino and test individually
// 2. Open pcmConfig.h and uncomment #define ENABLE_RF  and optionally  #define RF_ONLY
// 3. Modify SD pin, and radio pin numbers below. Upload this sketch to the device.
// 4. Send serial commands via the serial monitor to start/stop playback
// 
// Receiver Setup:
// 1. Download and install the RF24Audio library: https://github.com/TMRh20/RF24Audio/archive/master.zip
// 2. Edit the userConfig.h for the intended sample rate of the audio on the SD card (default is 24kHz)
// 3. Upload the minimal example sketch to the receiving device
//
// See http://tmrh20.github.io/ for all library documentation.


#include <SD.h>                // need to include the SD library
#define SD_ChipSelectPin 53    //example uses hardware SS pin 53 on Mega2560
//#define SD_ChipSelectPin 4   //using digital pin 4 on arduino nano 328, can use other pins
#include <TMRpcm.h>            //  also need to include this library...
#include <SPI.h>
#include <RF24.h>
#include <printf.h>

RF24 radio(48,49);                   // I use 48,49 on Mega 2560, 7,8 on Uno, Nano, etc
pcmRF rfAudio(radio);

void setup(){

  Serial.begin(115200);

  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }else{   
    Serial.println("SD ok");   
  }
  //printf_begin();                  // Comment this out to save space
  rfAudio.begin();
  //radio.printDetails();            // Comment this out to save space
}



void loop(){  

  if(Serial.available()){    
    switch(Serial.read()){
    case 'e': rfAudio.play("catfish",1); break;          // Play to radio # 1 in the radio group
    case 'c': rfAudio.play("calibrat.wav",0); break;     // Play to radio # 0 in the radio group
    case 'b': rfAudio.play("calibrat.wav",255); break;   // Play to all radios in the radio group
    case 'f': rfAudio.stop(); break;
    default: break;
    }
  }

}