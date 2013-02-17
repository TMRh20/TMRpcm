#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include "printf.h"
#include <pcmRX.h>




int csnPin = 7;
RF24 radio(6,csnPin);
pcmRX pcmrx(radio,csnPin);

const uint64_t pipe = { 0x544d526832LL};

void setup(){
  
  pinMode(9,OUTPUT);
  Serial.begin(115200);
  printf_begin();
  rfCfg();

}



void loop(){
  
  pcmrx.pollForMusic(); 
  
}
