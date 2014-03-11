
#include <pcmRF.h>

#if defined(ENABLE_RF)

	#include <Arduino.h>
	#include <SD.h>
	#include <SdFat.h>
	#include <SPI.h>
	#include <nRF24L01.h>
	#include <RF24.h>
	#include <RF24_config.h>

	const int payloadSize = 32;
	byte nextAck[payloadSize+1];
	volatile boolean oneAck = 0;
	boolean rfPlaying = 1;
	byte zz=0;
	const uint64_t pipe[3] = { 0x544d526832LL, 0x544d52687CLL };

	RF24 radi(0,0);
	#if !defined(SDFAT)
		File txFile;
	#else
		SdFile txFile;
	#endif
	TMRpcm rfWav;



//************** RF SECTION ********************
// for use with NRF24L01+ radio modules



pcmRF::pcmRF(int CE, int CS, int spkPin){
	RF24 tmp(CE,CS);
	radi = tmp;
	pinMode(spkPin,OUTPUT);
	switch(spkPin){
		case 5: zz=1; break; //use TIMER3
		case 6: zz=2; break;//use TIMER4
		case 46:zz=3; break;//use TIMER5
		default:zz=0; break; //useTIMER1 as default
	}

}


boolean pcmRF::isPlaying(){
  if(rfPlaying == 1){return 1;}
  return 0;

}


void stopRfPlay(){
	#if !defined(SDFAT)
		if(txFile){txFile.close();}
	#else
		if(txFile.isOpen()){txFile.close();}
	#endif
	rfPlaying = 0;
	detachInterrupt(5); Serial.println("end");
	//radi.stopListening();
}

void pcmRF::stop(){
	#if !defined(SDFAT)
		if(txFile){txFile.close();}
	#else
		if(txFile.isOpen()){txFile.close();}
	#endif
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

            radi.writeAckPayload(1,&rfWav.SAMPLE_RATE,sizeof(rfWav.SAMPLE_RATE));

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



void pcmRF::play(char* filename, int device){
  oneAck = 1;
  stopRfPlay();
  if(rfPlaying){ stopRfPlay(); }
  rfPlaying = 1;
  rfWav.disable();

  radi.stopListening();
  radi.openReadingPipe(1,pipe[device]);
  radi.startListening();

  Serial.print("RF ");Serial.println(device);

  if(!rfWav.wavInfo(filename)){ stopRfPlay(); return;}

	#if !defined(SDFAT)
		if(!txFile){ txFile = SD.open(filename);}
		if(txFile){
		txFile.seek(44); //skip the header info
	#else
		if(!txFile.isOpen()){ txFile.open(filename);}
		if(txFile.isOpen()){
		txFile.seekSet(44); //skip the header info
	#endif


    byte intData[5] = {1,2,3,4};
    radi.writeAckPayload(1,&intData,sizeof(intData));

    attachInterrupt(5,checkRF,FALLING);

  }else{Serial.println("failed to open music file"); }

}



#endif


