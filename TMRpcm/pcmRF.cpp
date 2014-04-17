
#include <pcmRF.h>

#if defined(ENABLE_RF)

	#include <Arduino.h>
	#include <SD.h>
	#include <SdFat.h>
	#include <SPI.h>
	#include <RF24.h>


	boolean rfPlaying = 0;
	const uint64_t addresses[14] = { 0xABCDABCD71LL, 0x544d52687CLL, 0x544d526832LL, 0x544d52683CLL,0x544d526846LL, 0x544d526850LL,0x544d52685ALL, 0x544d526820LL, 0x544d52686ELL, 0x544d52684BLL, 0x544d526841LL, 0x544d526855LL,0x544d52685FLL,0x544d526869LL};

	#if !defined(SDFAT)
		File txFile;
	#else
		SdFile txFile;
	#endif

	byte buff[32];
	RF24 radi(0,0);

//************** RF SECTION ********************

pcmRF::pcmRF( RF24& _radio ){

	radi = _radio;
}

void pcmRF::begin(){
	radi.begin();
	radi.setChannel(1);                 // Set RF channel to 1
	radi.setAutoAck(0);                 // Disable ACKnowledgement packets
	radi.setDataRate(RF24_1MBPS);         // Set data rate as specified in user options
  	radi.setCRCLength(RF24_CRC_8);
    radi.openWritingPipe(addresses[1]);
    radi.openReadingPipe(1,addresses[0]);
}

boolean pcmRF::isPlaying(){
  return rfPlaying;

}

void pcmRF::stop(){
	TIMSK1 &= ~( _BV(OCIE1A) | _BV(OCIE1B) );
	#if !defined(SDFAT)
		if(txFile){txFile.close();}
	#else
		if(txFile.isOpen()){txFile.close();}
	#endif
	rfPlaying = 0;
}


void stop(){
	TIMSK1 &= ~( _BV(OCIE1A) | _BV(OCIE1B) );
	#if !defined(SDFAT)
		if(txFile){txFile.close();}
	#else
		if(txFile.isOpen()){txFile.close();}
	#endif
	rfPlaying = 0;

}

void pcmRF::broadcast(byte device){
	noInterrupts();
	if(device == 255){
		radi.openWritingPipe(addresses[1]);
	}else{
		radi.openWritingPipe(addresses[device+2]);
	}
	interrupts();
}

void pcmRF::play(char* filename, byte device){
  stop();
  rfPlaying = 1;

	if(device == 255){
		radi.openWritingPipe(addresses[1]);
	}else{
		radi.openWritingPipe(addresses[device+2]);
	}


	#if !defined(SDFAT)
		if(!txFile){ txFile = SD.open(filename);}
		if(txFile){
		txFile.seek(24); //skip the header info
	#else
		if(!txFile.isOpen()){ txFile.open(filename);}
		if(txFile.isOpen()){
		txFile.seekSet(24); //skip the header info
	#endif


    unsigned int SAMPLE_RATE = txFile.read();
    SAMPLE_RATE |= txFile.read() << 8;

    #if !defined(SDFAT)
    	txFile.seek(44);
	#else
		txFile.seekSet(44);
	#endif

	unsigned int res = (10 * (1600000/SAMPLE_RATE)) * 32;


	noInterrupts();
	ICR1 = res;
	TCCR1A = _BV(WGM11);
	TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
	TIMSK1 = ( _BV(OCIE1A) );
	interrupts();

  }else{
	  #if defined (debug)
		  Serial.println("failed to open music file");
	  #endif
  }
}



ISR(TIMER1_COMPA_vect){
	radi.writeFast(&buff,32);
	txFile.read((byte*)buff,32);
	if(txFile.available() < 32){ radi.txStandBy(); stop(); }
}




#endif


