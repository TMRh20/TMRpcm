
#include <Arduino.h>
#include <pcmRF.h>
#include <SD.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <TMRpcm.h>

const int payloadSize = 32;
byte nextAck[payloadSize+1];
volatile boolean oneAck = 0;
boolean rfPlaying = 1;
const uint64_t pipe[3] = { 0x544d526832LL, 0x544d52687CLL };


RF24 radi(0,0);
File txFile;
TMRpcm wav;



pcmRF::pcmRF(int CE, int CS){
	RF24 tmp(CE,CS);
	radi = tmp;
}


boolean pcmRF::isPlaying(){
  if(rfPlaying == 1){return 1;}
  return 0;

}


void stopRfPlay(){

  if(txFile){txFile.close();}
  rfPlaying = 0;
  detachInterrupt(5); Serial.println("end");
    //radi.stopListening();

}

void pcmRF::stop(){

  if(txFile){txFile.close();}
  rfPlaying = 0;
  detachInterrupt(5); Serial.println("end");
    //radi.stopListening();
}


void checkRF(){

  bool tx,fail,rx;
  radi.whatHappened(tx,fail,rx);


  if(!rx){ return; }

    byte intData[5] = {1,2,3,4};
    byte recvd[payloadSize+1];

    radi.read(&recvd,payloadSize);

    if(recvd[0] == 3){
            if(txFile.available() > payloadSize){
              radi.writeAckPayload(1,&nextAck,payloadSize);
                for(int i=0; i<payloadSize; i++){
                 nextAck[i] = txFile.read();
              }
            }else{
              if(oneAck){ byte endSignal[5] = {4,3,2,1};
              radi.writeAckPayload(1,&endSignal,payloadSize); oneAck = 0;}
              //stopRfPlay();
              return;
            }

    }else

    if(recvd[0] == 4){

            radi.writeAckPayload(1,&wav.SAMPLE_RATE,sizeof(wav.SAMPLE_RATE));

    }else

    if(recvd[0] == 5){
            radi.writeAckPayload(1,&intData,sizeof(intData));
            if(txFile.available() > payloadSize){
              for(int i=0; i<payloadSize; i++){
                nextAck[i] = txFile.read();
              }
            }


    }else

    if(recvd[0] == 6){
            byte junk[5] = {7,6,5,4};
            radi.writeAckPayload(1,&junk,sizeof(junk));
            radi.writeAckPayload(1,&junk,sizeof(junk));
            Serial.println("ebp");
            stopRfPlay();
    }else

    if(recvd[0] == 9){
      //Serial.println(" 9recvd: ");
      byte junk[5] = {7,6,5,4};
      radi.writeAckPayload(1,&junk,sizeof(junk));

    }

}



void pcmRF::playRF(char* filename, int device){
  oneAck = 1;
  stopRfPlay();
  if(rfPlaying){ stopRfPlay(); }
  rfPlaying = 1;
  wav.disable();

  radi.stopListening();
  radi.openReadingPipe(1,pipe[device]);
  radi.startListening();

  Serial.print("RF ");Serial.println(device);

  if(!wav.wavInfo(filename)){ stopRfPlay(); return;}
  if(!txFile){ txFile = SD.open(filename);}

  if(txFile){

    txFile.seek(44); //skip the header info

    byte intData[5] = {1,2,3,4};
    radi.writeAckPayload(1,&intData,sizeof(intData));

    attachInterrupt(5,checkRF,FALLING);

  }else{Serial.println("failed to open music file"); }

}



