#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include "printf.h"
#include <pcmRX.h>

int csnPin = 7;
int speakerPin = 9;
const uint64_t pipe = { 0x544d526832LL};

RF24 radio(6,csnPin);
pcmRX pcmrx(radio,csnPin,speakerPin);


void setup(){  
  Serial.begin(57600);
  printf_begin();
  rfCfg();
}


void loop(){  
  pcmrx.pollForMusic();   
}
