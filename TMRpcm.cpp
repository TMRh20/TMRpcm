/*Library by TMRh20 2012-2014*/

#define RESOLUTION_BASE ((F_CPU) / 10)
    
#include <pcmConfig.h>
#if !defined (SDFAT)
	#include <SD.h>
#else
	#include <SdFat.h>
#endif
#include <TMRpcm.h>

#if !defined (RF_ONLY)

//********************* Timer arrays and pointers **********************
//********** Enables use of different timers on different boards********

#if !defined (USE_TIMER2) //NOT using TIMER2

	const byte togByte = _BV(ICIE1); //Get the value for toggling the buffer interrupt on/off

	#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

	#if !defined (buffSize)
		#define buffSize 254
	#endif
	#if !defined (rampMega)
			#define rampMega
	#endif

		volatile byte *TIMSK[] = {&TIMSK1,&TIMSK3,&TIMSK4,&TIMSK5};
		volatile byte *TCCRnA[] = {&TCCR1A,&TCCR3A,&TCCR4A,&TCCR5A};
		volatile byte *TCCRnB[] = {&TCCR1B, &TCCR3B,&TCCR4B,&TCCR5B};
		volatile unsigned int *OCRnA[] = {&OCR1A,&OCR3A,&OCR4A,&OCR5A};
		volatile unsigned int *ICRn[] = {&ICR1, &ICR3,&ICR4,&ICR5};
		volatile unsigned int *TCNT[] = {&TCNT1,&TCNT3,&TCNT4,&TCNT5};

		#if !defined (DISABLE_SPEAKER2)
			volatile unsigned int *OCRnB[] = {&OCR1B, &OCR3B,&OCR4B,&OCR5B};
		#else
		 	volatile unsigned int one, two, three, four;
			volatile unsigned int *OCRnB[] ={&one,&two,&three,&four};
		#endif

	//*** These aliases can be commented out to enable full use of alternate timers for other things
		ISR(TIMER3_OVF_vect, ISR_ALIASOF(TIMER1_OVF_vect));
		ISR(TIMER3_CAPT_vect, ISR_ALIASOF(TIMER1_CAPT_vect));

		ISR(TIMER4_OVF_vect, ISR_ALIASOF(TIMER1_OVF_vect));
		ISR(TIMER4_CAPT_vect, ISR_ALIASOF(TIMER1_CAPT_vect));

		ISR(TIMER5_OVF_vect, ISR_ALIASOF(TIMER1_OVF_vect));
		ISR(TIMER5_CAPT_vect, ISR_ALIASOF(TIMER1_CAPT_vect));

		#if defined (ENABLE_RECORDING)
		ISR(TIMER3_COMPA_vect, ISR_ALIASOF(TIMER1_COMPA_vect));
		ISR(TIMER3_COMPB_vect, ISR_ALIASOF(TIMER1_COMPB_vect));

		ISR(TIMER4_COMPA_vect, ISR_ALIASOF(TIMER1_COMPA_vect));
		ISR(TIMER4_COMPB_vect, ISR_ALIASOF(TIMER1_COMPB_vect));

		ISR(TIMER5_COMPA_vect, ISR_ALIASOF(TIMER1_COMPA_vect));
		ISR(TIMER5_COMPB_vect, ISR_ALIASOF(TIMER1_COMPB_vect));
		#endif


	#elif defined (__AVR_ATmega32U4__) || (__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__) || (__AVR_ATmega128__) ||defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)

		#if !defined buffSize
			#define buffSize 128
		#endif
		#if !defined (rampMega)
				#define rampMega
		#endif

		volatile byte *TIMSK[] = {&TIMSK1,&TIMSK3};
		volatile byte *TCCRnA[] = {&TCCR1A,&TCCR3A};
		volatile byte *TCCRnB[] = {&TCCR1B, &TCCR3B};
		volatile unsigned int *OCRnA[] = {&OCR1A,&OCR3A};
		volatile unsigned int *ICRn[] = {&ICR1, &ICR3};
		volatile unsigned int *TCNT[] = {&TCNT1,&TCNT3};

		#if !defined (DISABLE_SPEAKER2)
			volatile unsigned int *OCRnB[] = {&OCR1B, &OCR3B};
		#else
		 	volatile unsigned int one, two;
			volatile unsigned int *OCRnB[] ={&one,&two};
		#endif
	//*** These aliases can be commented out to enable full use of alternate timers for other things
		ISR(TIMER3_OVF_vect, ISR_ALIASOF(TIMER1_OVF_vect));
		ISR(TIMER3_CAPT_vect, ISR_ALIASOF(TIMER1_CAPT_vect));

		#if defined (ENABLE_RECORDING)
		ISR(TIMER3_COMPA_vect, ISR_ALIASOF(TIMER1_COMPA_vect));
		ISR(TIMER3_COMPB_vect, ISR_ALIASOF(TIMER1_COMPB_vect));
		#endif

	#else
		#if !defined (buffSize)
			#define buffSize 64
		#endif

		volatile byte *TIMSK[] = {&TIMSK1};
		volatile byte *TCCRnA[] = {&TCCR1A};
		volatile byte *TCCRnB[] = {&TCCR1B};
		volatile unsigned int *OCRnA[] = {&OCR1A};
		volatile unsigned int *ICRn[]	= {&ICR1};
		volatile unsigned int *TCNT[] = {&TCNT1};

		#if !defined (DISABLE_SPEAKER2)
			volatile unsigned int *OCRnB[] = {&OCR1B};
		#else
		 	volatile unsigned int one;
			volatile unsigned int *OCRnB[] ={&one};
		#endif

	#endif



#else //USING TIMER 2

	#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

		#if !defined (buffSize)
			#define buffSize 128
		#endif
	#elif defined (__AVR_ATmega32U4__) || (__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__) || (__AVR_ATmega128__) ||defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)
		#if !defined (buffSize)
				#define buffSize 128
		#endif
	#else
		#if !defined (buffSize)
			#define buffSize 64
		#endif
	#endif

		volatile byte *TIMSK[] = {&TIMSK2};
		volatile byte *TCCRnA[] = {&TCCR2A};
		volatile byte *TCCRnB[] = {&TCCR2B};
		volatile byte *OCRnA[] = {&OCR2A};
		volatile byte *OCRnB[] = {&OCR2B};
		volatile byte *TCNT[] = {&TCNT2};

		const byte togByte = _BV(OCIE2B);


#endif


//**********Global Variables for Regular and Multi modes ************

#if defined (ENABLE_MULTI)
	volatile byte buffer2[2][buffSize];
	volatile byte buffCount2 = 0;
	volatile boolean buffEmpty2[2] = {false,false}, whichBuff2 = false, playing2 = 0;

	char volMod2=0;
	char *fileName2;
	#if !defined (SDFAT)
	File tFile;
	#else
	SdFile tFile;
	#endif
#endif

//*********** Standard Global Variables ***************
volatile unsigned int dataEnd;
volatile boolean buffEmpty[2] = {true,true}, whichBuff = false, playing = 0, a, b;

//*** Options/Indicators from MSb to LSb: paused, qual, rampUp, 2-byte samples, loop, loop2nd track, 16-bit ***
byte optionByte = B01100000;

volatile byte buffer[2][buffSize], buffCount = 0;
char volMod=0;
byte tt;

#if !defined (USE_TIMER2)
	volatile boolean loadCounter=0;
#else
	volatile byte loadCounter = 0, SR = 3;
#endif

#if !defined (SDFAT)
	File sFile;
#else
	SdFile sFile;
#endif

#if defined (MODE2)
	byte tt2 = 0;
#endif

#if defined (ENABLE_RECORDING)
	Sd2Card card1;
	byte recording = 0;
#endif
//**************************************************************
//********** Core Playback Functions used in all modes *********


#if !defined (USE_TIMER2) //NOT using timer2
void TMRpcm::timerSt(){
	*ICRn[tt] = resolution;
	#if !defined (DISABLE_SPEAKER2)
		*TCCRnA[tt] = _BV(WGM11) | _BV(COM1A1) | _BV(COM1B0) | _BV(COM1B1); //WGM11,12,13 all set to 1 = fast PWM/w ICR TOP
	#else
		*TCCRnA[tt] = _BV(WGM11) | _BV(COM1A1); //WGM11,12,13 all set to 1 = fast PWM/w ICR TOP
	#endif
	*TCCRnB[tt] = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
  #if defined (MODE2)
  	*ICRn[tt2] = resolution;
	*TCCRnA[tt2] = _BV(WGM11) | _BV(COM1A1) | _BV(COM1B0) | _BV(COM1B1); //WGM11,12,13 all set to 1 = fast PWM/w ICR TOP
	*TCCRnB[tt2] = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
  #endif
}
#else //Using TIMER2

void TMRpcm::timerSt(){
	*TCCRnA[tt] = _BV(WGM21) | _BV(WGM20) | _BV(COM2B1); //Fast PWM with 0xFF (255) top
}
#endif


void TMRpcm::setPin(){
	disable();
	pinMode(speakerPin,OUTPUT);

	#if !defined (USE_TIMER2) //NOT using TIMER2
		switch(speakerPin){
			case 5: tt=1; break; //use TIMER3
			case 6: tt=2; break;//use TIMER4
			case 46:tt=3; break;//use TIMER5
			default:tt=0; break; //useTIMER1 as default
		}
	#else //Using TIMER2 only
		tt = 0;
	#endif

	#if defined (SD_FULLSPEED)
		SPSR |= (1 << SPI2X);
	   	SPCR &= ~((1 <<SPR1) | (1 << SPR0));
	#endif
}


#if defined (MODE2)
void TMRpcm::setPins(){
	disable();
	pinMode(speakerPin,OUTPUT);
	pinMode(speakerPin2,OUTPUT);
	switch(speakerPin){
		case 5: tt=1; break; //use TIMER3
		case 6: tt=2; break;//use TIMER4
		case 46:tt=3; break;//use TIMER5
		default:tt=0; break; //useTIMER1 as default
	}
	switch(speakerPin2){
		case 5: tt2=1; break; //use TIMER3
		case 6: tt2=2; break;//use TIMER4
		case 46:tt2=3; break;//use TIMER5
		default:tt2=0; break; //useTIMER1 as default
	}

	#if defined (SD_FULLSPEED)
		SPSR |= (1 << SPI2X);
    	SPCR &= ~((1 <<SPR1) | (1 << SPR0));
	#endif
}
#endif


boolean TMRpcm::wavInfo(char* filename){

  //check for the string WAVE starting at 8th bit in header of file
  #if !defined (SDFAT)
  sFile = SD.open(filename);
  #else
  sFile.open(filename);
  #endif

  if( !ifOpen() ){ return 0; }
  seek(8);
  char wavStr[] = {'W','A','V','E'};
  for (byte i =0; i<4; i++){
	  if(sFile.read() != wavStr[i]){
		  #if defined (debug)
		  	Serial.println("WAV ERROR");
		  #endif
		  break; }
  }
	#if defined (STEREO_OR_16BIT)
	    byte stereo, bps;
		seek(22);
	    stereo = sFile.read();
	    seek(24);
	#else
	    seek(24);
	#endif

    SAMPLE_RATE = sFile.read();
    SAMPLE_RATE = sFile.read() << 8 | SAMPLE_RATE;

    #if defined (USE_TIMER2)

    	if(SAMPLE_RATE < 9000 ){ SR = 0; }
    	else if(SAMPLE_RATE < 20000){ SR = 1; }
    	else if(SAMPLE_RATE < 28000){ SR = 2; }
    	else{ SR = 3; }
    #endif
	#if defined (STEREO_OR_16BIT)
	    //verify that Bits Per Sample is 8 (0-255)
		seek(34);
	    bps = sFile.read();
	    bps = sFile.read() << 8 | bps;
	    if( stereo == 2){ //_2bytes=1;
	    	bitSet(optionByte,4);
		}else
		if( bps == 16 ){
			bitSet(optionByte,1);
			bitSet(optionByte,4);
	    }else{ bitClear(optionByte,4); bitClear(optionByte,1);}
	#endif

	#if defined (HANDLE_TAGS)

		seek(36);
	    char datStr[4] = {'d','a','t','a'};
	    for (byte i =0; i<4; i++){
			if(sFile.read() != datStr[i]){
			 	seek(40);
			 	unsigned int siz = sFile.read();
			 	siz = (sFile.read() << 8 | siz)+2;
			 	seek(fPosition() + siz);
 	     		for (byte i =0; i<4; i++){
					if(sFile.read() != datStr[i]){
						return 0;
					}
		 		}
			}
		 }

		unsigned long dataBytes = sFile.read();
	    for (byte i =8; i<32; i+=8){
			dataBytes = sFile.read() << i | dataBytes;
		}
		#if !defined (SDFAT)
			dataEnd = sFile.size() - fPosition() - dataBytes + buffSize;
		#else
			dataEnd = sFile.fileSize() - fPosition() - dataBytes + buffSize;
		#endif

	#else //No Tag handling

		seek(44); dataEnd = buffSize;

	#endif

	return 1;

}


//*************** General Playback Functions *****************

void TMRpcm::quality(boolean q){
	if(!playing){	bitWrite(optionByte,6,q); } //qual = q; }
}

void TMRpcm::stopPlayback(){
	playing = 0;

	*TIMSK[tt] &= ~(togByte | _BV(TOIE1));

	if(ifOpen()){ sFile.close(); }

	#if defined (ENABLE_MULTI)
		playing2 = 0;
		#if !defined (SDFAT)
		if(tFile){tFile.close();}
		#else
		if(tFile.isOpen()){tFile.close();}
		#endif
	#endif

}

void TMRpcm::pause(){
	//paused = !paused;
	if(bitRead(optionByte,7) && playing){
		bitClear(optionByte,7);
		#if !defined (USE_TIMER2)
			*TIMSK[tt] |= ( _BV(ICIE1) | _BV(TOIE1) );
		#else
			*TIMSK[tt] |= ( _BV(OCIE2B) | _BV(TOIE1) );
		#endif
	}else if(!bitRead(optionByte,7) && playing){
		bitSet(optionByte,7);
		*TIMSK[tt] &= ~( _BV(TOIE1) );
	}
}

void TMRpcm::loop(boolean set){
	bitWrite(optionByte,3,set);
}
/**************************************************
This section used for translation of functions between
 SDFAT library and regular SD library
Prevents a whole lot more #if defined statements */

#if !defined (SDFAT)

	boolean TMRpcm::seek( unsigned long pos ){
		return sFile.seek(pos);
	}

	unsigned long TMRpcm::fPosition( ){
		return sFile.position();
	}

	boolean TMRpcm::ifOpen(){
		if(sFile){ return 1;}
	}

#else

	boolean TMRpcm::seek(unsigned long pos ){
		return sFile.seekSet(pos);
	}

	unsigned long TMRpcm::fPosition(){
		return sFile.curPosition();
	}

	boolean TMRpcm::ifOpen(){
		return sFile.isOpen();
	}

#endif



//***************************************************************************************
//********************** Functions for single track playback ****************************

#if !defined (ENABLE_MULTI) //Below section for normal playback of 1 track at a time


void TMRpcm::play(char* filename){
	play(filename,0);
}


void TMRpcm::play(char* filename, unsigned long seekPoint){

  if(speakerPin != lastSpeakPin){
	  #if !defined (MODE2)
	  	setPin();
	  #else
	    setPins();
	  #endif
	  lastSpeakPin=speakerPin;
   }
  stopPlayback();
  if(!wavInfo(filename)){
  	#if defined (debug)
  		Serial.println("WAV ERROR");
  	#endif
  return;
  }//verify its a valid wav file


  		if(seekPoint > 0){ seekPoint = (SAMPLE_RATE*seekPoint) + fPosition();
  		seek(seekPoint); //skip the header info

  }
	playing = 1; bitClear(optionByte,7); //paused = 0;

	if(SAMPLE_RATE > 45050 ){ SAMPLE_RATE = 24000;
	#if defined (debug)
  	  	Serial.print("SAMPLE RATE TOO HIGH: ");
  	  	Serial.println(SAMPLE_RATE);
  	#endif
  	}

#if !defined (USE_TIMER2)
    //if(qual)
    if(bitRead(optionByte,6)){resolution = 10 * ((RESOLUTION_BASE/2)/SAMPLE_RATE);}
    else{ resolution = 10 * (RESOLUTION_BASE/SAMPLE_RATE);
	}
#else
	resolution = 255;
	if(SAMPLE_RATE < 9000){
		*TCCRnB[tt] &= ~_BV(CS20);
		*TCCRnB[tt] |= _BV(CS21);
	}else{
		*TCCRnB[tt] &= ~_BV(CS21);
		*TCCRnB[tt] |= _BV(CS20);
	}
#endif
    byte tmp = (sFile.read() + sFile.peek()) / 2;

	#if defined(rampMega)
    if(bitRead(optionByte,5)){

			*OCRnA[tt] = 0; *OCRnB[tt] = resolution;
			timerSt();
			for(unsigned int i=0; i < resolution; i++){

				*OCRnB[tt] = constrain(resolution-i,0,resolution);

			//if(bitRead(*TCCRnB[tt],0)){
			//	for(int i=0; i<10; i++){
			//		while(*TCNT[tt] < resolution-50){}
			//	}
			//}else{
				delayMicroseconds(150);
			//}
			}

	}
	bitClear(optionByte,5);
	#endif

	//rampUp = 0;
	unsigned int mod;
	if(volMod > 0){ mod = *OCRnA[tt] >> volMod; }else{ mod = *OCRnA[tt] << (volMod*-1); }
	if(tmp > mod){
		for(unsigned int i=0; i<buffSize; i++){ mod = constrain(mod+1,mod, tmp); buffer[0][i] = mod; }
		for(unsigned int i=0; i<buffSize; i++){ mod = constrain(mod+1,mod, tmp); buffer[1][i] = mod; }
	}else{
		for(unsigned int i=0; i<buffSize; i++){ mod = constrain(mod-1,tmp ,mod); buffer[0][i] = mod; }
		for(unsigned int i=0; i<buffSize; i++){ mod = constrain(mod-1,tmp, mod); buffer[1][i] = mod; }
	}

    whichBuff = 0; buffEmpty[0] = 0; buffEmpty[1] = 0; buffCount = 0;
    noInterrupts();
	timerSt();

    *TIMSK[tt] = ( togByte | _BV(TOIE1) );

    interrupts();

}

void TMRpcm::volume(char upDown){

  if(upDown){
	  volMod++;
  }else{
	  volMod--;
  }
}

void TMRpcm::setVolume(char vol) {
    volMod = vol - 4 ;
}

#if defined (ENABLE_RECORDING)

  ISR(TIMER1_COMPA_vect){
		if(buffEmpty[!whichBuff] == 0){
	 		a = !whichBuff;
   			*TIMSK[tt] &= ~(_BV(OCIE1A));
   			sei();
   			sFile.write((byte*)buffer[a], buffSize );
   			buffEmpty[a] = 1;
   			*TIMSK[tt] |= _BV(OCIE1A);
		}
  }
#endif

#if !defined (USE_TIMER2) //Not using TIMER2
ISR(TIMER1_CAPT_vect){
#else 					  //Using TIMER2
ISR(TIMER2_COMPB_vect){
#endif

  // The first step is to disable this interrupt before manually enabling global interrupts.
  // This allows this interrupt vector (COMPB) to continue loading data while allowing the overflow interrupt
  // to interrupt it. ( Nested Interrupts )
  // TIMSK1 &= ~_BV(ICIE1);
 //Then enable global interupts before this interrupt is finished, so the music can interrupt the buffering
  //sei();

	if(buffEmpty[!whichBuff]){

		a = !whichBuff;
		*TIMSK[tt] &= ~togByte;
		sei();

		if( sFile.available() <= dataEnd){

		  	#if !defined (SDFAT)
		  		if(bitRead(optionByte,3)){ sFile.seek(44); *TIMSK[tt] |= togByte; return;}
		  		*TIMSK[tt] &= ~( togByte | _BV(TOIE1) );
			  	if(sFile){ sFile.close();}
			#else
				if(bitRead(optionByte,3)){ sFile.seekSet(44); *TIMSK[tt] |= togByte; return;}
				*TIMSK[tt] &= ~( togByte | _BV(TOIE1) );
				if(sFile.isOpen()){ sFile.close();}
			#endif
			playing = 0;
			return;
	  	}
	  	sFile.read((byte*)buffer[a],buffSize);
			buffEmpty[a] = 0;
			*TIMSK[tt] |= togByte;
   	}
}

#if defined(USE_TIMER2)

	ISR(TIMER2_OVF_vect){

		//TIMER2 runs at 16mhz/255 (62,745Hz), so the timing for sample rate must be manually counted
		switch (SR){
			case 0: break;
			case 3: if(loadCounter){loadCounter = 0;return;}  break;
			case 1: if(loadCounter){ if(loadCounter >= 3){loadCounter=0; return;} loadCounter++; return;} break;
			case 2: if(loadCounter){ if(loadCounter >= 2){loadCounter=0; return; }loadCounter++;  return;} break;
		}
		loadCounter++;

		if(volMod < 0 ){  *OCRnB[tt] = buffer[whichBuff][buffCount] >> (volMod*-1);
		}else{  		  *OCRnB[tt] = buffer[whichBuff][buffCount] << volMod;
		}
	  	++buffCount;
	  	  if(buffCount >= buffSize){
		  buffCount = 0;
	      buffEmpty[whichBuff] = true;
	      whichBuff = !whichBuff;
	  	}
	}

#else

#if defined (ENABLE_RECORDING)
  ISR(TIMER1_COMPB_vect){

    buffer[whichBuff][buffCount] = ADCH;
	if(recording > 1){
		if(volMod < 0 ){  *OCRnA[tt] = ADCH >> (volMod*-1);
	  	}else{  		  *OCRnA[tt] = ADCH << volMod;
	  	}
	}
 		buffCount++;
 		if(buffCount >= buffSize){
    		buffCount = 0;
    		buffEmpty[!whichBuff] = 0;
  			whichBuff = !whichBuff;
  		}
  }

#endif

ISR(TIMER1_OVF_vect){


    if(bitRead(optionByte,6)){loadCounter = !loadCounter;if(loadCounter){ return; } }

	#if defined (STEREO_OR_16BIT)
	if( !bitRead(optionByte,4) ){
	#endif
		if(volMod < 0 ){  *OCRnA[tt] = *OCRnB[tt] = buffer[whichBuff][buffCount] >> (volMod*-1);
	  	}else{  		  *OCRnA[tt] = *OCRnB[tt] = buffer[whichBuff][buffCount] << volMod;
	  	}
	  	++buffCount;

	#if defined (STEREO_OR_16BIT)
	}else{
		if(bitRead(optionByte,1)){
			buffer[whichBuff][buffCount] += 127;
			//buffer[whichBuff][buffCount+1] += 127;
		}
		#if !defined (MODE2)
			if(volMod < 0 ){ *OCRnA[tt] = buffer[whichBuff][buffCount] >> (volMod*-1);
  							 *OCRnB[tt] = buffer[whichBuff][buffCount+1] >> (volMod*-1);

	  		}else{    		 *OCRnA[tt] = buffer[whichBuff][buffCount] << volMod;
							 *OCRnB[tt] = buffer[whichBuff][buffCount+1] << volMod;
  			}
  		#else
			if(volMod < 0 ){
							 *OCRnA[tt] = *OCRnB[tt] = buffer[whichBuff][buffCount] >> (volMod*-1);
  							 *OCRnA[tt2] = *OCRnB[tt2] =buffer[whichBuff][buffCount+1] >> (volMod*-1);

	  		}else{    		 *OCRnA[tt] = *OCRnB[tt] = buffer[whichBuff][buffCount] << volMod;
							 *OCRnA[tt2] = *OCRnB[tt2] = buffer[whichBuff][buffCount+1] << volMod;
  			}

  		#endif
  		buffCount+=2;
  	}
	#endif

  	if(buffCount >= buffSize){
	  buffCount = 0;
      buffEmpty[whichBuff] = true;
      whichBuff = !whichBuff;
  	}
}

#endif



void TMRpcm::disable(){
	playing = 0;
	*TIMSK[tt] &= ~( togByte | _BV(TOIE1) );
	if(ifOpen()){ sFile.close();}
	if(bitRead(*TCCRnA[tt],7) > 0){
		int current = *OCRnA[tt];
		for(int i=0; i < resolution; i++){
			#if defined(rampMega)
				*OCRnB[tt] = constrain((current + i),0,resolution);
				*OCRnA[tt] = constrain((current - i),0,resolution);
			#else
				*OCRnB[tt] = constrain((current - i),0,resolution);
				*OCRnA[tt] = constrain((current - i),0,resolution);
			#endif
			for(int i=0; i<10; i++){ while(*TCNT[tt] < resolution-50){} }
		}
	}
    bitSet(optionByte,5);
    //rampUp = 1;
    *TCCRnA[tt] = *TCCRnB[tt] = 0;
}



boolean TMRpcm::isPlaying(){
	return playing;
}



//***************************************************************************************
//********************** Functions for dual track playback ****************************
#else //Else Dual

void TMRpcm::loop(boolean set, boolean which){
	if(!which){ bitWrite(optionByte,3,set); }
	else{	    bitWrite(optionByte,2,set); }
}

void TMRpcm::play(char* filename, boolean which){
	play(filename,0,which);
}

void TMRpcm::play(char* filename){
	play(filename,0,0);
}

byte hold = 0;

void TMRpcm::play(char* filename, unsigned long seekPoint, boolean which){

  if(speakerPin != lastSpeakPin){
	  #if defined (MODE2)
	  setPins();
	  #else
	  setPin();
	  #endif
	  lastSpeakPin=speakerPin;
  }
  byte dual = 0;

	#if defined(rampMega)
    if(bitRead(optionByte,5)){//RampUp
		*OCRnA[tt] = 0; *OCRnB[tt] = resolution;
		#if defined (MODE2)
			*OCRnA[tt2] = 0; *OCRnB[tt2] = resolution;
		#endif
		timerSt();
		//rampUp = 0;
		bitClear(optionByte,5);
		for(unsigned int i=0; i < resolution; i++){
			*OCRnB[tt] = constrain(resolution-i,0,resolution);
			#if defined (MODE2)
				*OCRnB[tt2] = constrain(resolution-i,0,resolution);
			#endif
			delayMicroseconds(50);

		}
	}
	#endif

  if((!playing && !playing2) || (playing && !playing2 && !which) || (playing2 && !playing && which) ){
	  stopPlayback();
	  if(!wavInfo(filename) ){ return; }//verify its a valid wav file
	  if(seekPoint > 0){ seekPoint = SAMPLE_RATE*seekPoint;}
	  if(!which){
		#if !defined (SDFAT)
		sFile = SD.open(filename);
		#else
	  	sFile.open(filename,O_READ);
	  	#endif
	  }else{
		  #if !defined (SDFAT)
			tFile = SD.open(filename);
	      #else
	    	tFile.open(filename,O_READ);
  	  	  #endif
  	  }
  	  	if(SAMPLE_RATE > 45050 ){ SAMPLE_RATE = 24000;
  	  		#if defined (debug)
	  	  		Serial.print("SampleRate Too High");
			#endif
		}
	#if !defined (USE_TIMER2)
	    if(bitRead(optionByte,6)){resolution = 10 * ((RESOLUTION_BASE/2)/SAMPLE_RATE);}
        else{ resolution = 10 * (RESOLUTION_BASE/SAMPLE_RATE);}
    #else
    	resolution = 255;
    #endif


  }else{
	  if(!which){ playing = 0; sFile.close(); dual = 1; }
	  if(which ){ playing2 = 0; tFile.close(); dual = 1; }
  }

 if(!dual){

	if(!which){
		//paused = 0;
		bitClear(optionByte,7);
		if(ifOpen()){
			if(seekPoint > 0){ seek(seekPoint); }else{ seek(44); }
			ramp(0);
			playing = 1;
		}else{
			#if defined (debug)
				Serial.println("Read fail");
			#endif
			return;
		}
	}else{
		//paused = 0;
		bitClear(optionByte,7);
		#if !defined (SDFAT)
			if(tFile){
				if(seekPoint > 0){ tFile.seek(seekPoint); }else{ tFile.seek(44); }
		#else
			if(tFile.isOpen()){
				tFile.seekSet(seekPoint);
				if(seekPoint > 0){ tFile.seekSet(seekPoint); }else{ tFile.seekSet(44); }
		#endif
		    	ramp(1);
		    	playing2 = 2;
			}else{
				#if defined (debug)
					Serial.println("Read fail");
				#endif
				return;
			}
	}

    noInterrupts();
    timerSt();
    *TIMSK[tt] = ( togByte | _BV(TOIE1) );
	interrupts();

  }

  if(dual){
 		if(seekPoint > 0){ seekPoint = SAMPLE_RATE*seekPoint;}
		*TIMSK[tt] &= ~togByte;
		if(!which){
			if(ifOpen()){ sFile.close(); }

			#if !defined (SDFAT)
				sFile = SD.open(filename);
			#else
				sFile.open(filename,O_READ);
			#endif
			if(ifOpen()){
				if(seekPoint > 0){ seek(seekPoint); }else{ seek(44); }
				ramp(0); playing = 1;
	    	}else{
				#if defined (debug)
				Serial.println("Dual Read fail");
				#endif
				return;
			}//open1 = 1; fileName2 = filename;  }
 		 }else

		 if(which){
			#if !defined (SDFAT)
				if(tFile){tFile.close();}
				tFile = SD.open(filename);
				if(tFile){
					if(seekPoint > 0){ tFile.seek(seekPoint); }else{ tFile.seek(44); }

			#else
				if(tFile.isOpen()){tFile.close();}
				tFile.open(filename);
				if(tFile.isOpen()){
					if(seekPoint > 0){ tFile.seekSet(seekPoint); }else{ tFile.seekSet(44); }

			#endif
					ramp(1); playing2 = 1;
				}else{
					#if defined (debug)
						Serial.println("Dual Read fail");
					#endif
				return;
				} //open1 = 1; fileName2 = filename;  }
  		 }
  		 *TIMSK[tt] |= togByte;
		//interrupts();
  }
}

void TMRpcm::ramp(boolean wBuff){


	  byte tmp;
	  byte mod;

	#if !defined (MODE2)
	  if(!wBuff){

		  //seek(44);
	  	  tmp = sFile.read();
		  if(volMod >= 0){ mod = *OCRnA[tt] >> volMod; }else{ mod = *OCRnA[tt] << (volMod*-1); }
		if(tmp > mod){
			for(byte i=0; i<buffSize; i++){ mod = constrain(mod+1,mod, tmp); buffer[0][i] = mod; }
		}else{
			for(byte i=0; i<buffSize; i++){ mod = constrain(mod-1,tmp ,mod); buffer[0][i] = mod; }
		}
		whichBuff = 0; buffEmpty[0] = 0;buffCount = 0; //buffEmpty[1] = 0;
	  }else{
		  //#if !defined (SDFAT)
		  //tFile.seek(44);
		  //#else
		  //tFile.seekSet(44);
		  //#endif
	  	  tmp = tFile.read();
		  if(volMod >= 0){ mod = *OCRnB[tt] >> volMod; }else{ mod = *OCRnB[tt] << (volMod*-1); }

		if(tmp > mod){
			for(byte i=0; i<buffSize; i++){ mod = constrain(mod+1,mod, tmp); buffer2[0][i] = mod; }
		}else{
			for(byte i=0; i<buffSize; i++){ mod = constrain(mod-1,tmp ,mod); buffer2[0][i] = mod; }
		}
		whichBuff2 = 0; buffEmpty2[0] = 0; buffCount2 = 0; //buffEmpty2[1] = 0;
  	 }
	#else //MODE2
	  if(!wBuff){
		  //seek(44);
	  	  tmp = sFile.read();
		  if(volMod >= 0){ mod = *OCRnA[tt] >> volMod; }else{ mod = *OCRnA[tt] << (volMod*-1); }
		if(tmp > mod){
			for(byte i=0; i<buffSize; i++){ mod = constrain(mod+1,mod, tmp); buffer[0][i] = mod; }
			for(byte i=0; i<buffSize; i++){ mod = constrain(mod+1,mod, tmp); buffer[1][i] = mod; }
		}else{
			for(byte i=0; i<buffSize; i++){ mod = constrain(mod-1,tmp ,mod); buffer[0][i] = mod; }
			for(byte i=0; i<buffSize; i++){ mod = constrain(mod-1,tmp ,mod); buffer[1][i] = mod; }
		}
		whichBuff = 0; buffEmpty[0] = 0;buffCount = 0; //buffEmpty[1] = 0;
	  }else{
		  //#if !defined (SDFAT)
		  //tFile.seek(44);
		  //#else
		  //tFile.seekSet(44);
	  	  //#endif
	  	  tmp = tFile.read();
		  if(volMod >= 0){ mod = *OCRnB[tt2] >> volMod; }else{ mod = *OCRnB[tt2] << (volMod*-1); }

		if(tmp > mod){
			for(byte i=0; i<buffSize; i++){ mod = constrain(mod+1,mod, tmp); buffer2[0][i] = mod; }
			for(byte i=0; i<buffSize; i++){ mod = constrain(mod+1,mod, tmp); buffer2[1][i] = mod; }
		}else{
			for(byte i=0; i<buffSize; i++){ mod = constrain(mod-1,tmp ,mod); buffer2[0][i] = mod; }
			for(byte i=0; i<buffSize; i++){ mod = constrain(mod-1,tmp ,mod); buffer2[1][i] = mod; }
		}
		whichBuff2 = 0; buffEmpty2[0] = 0; buffCount2 = 0; //buffEmpty2[1] = 0;
  	 }

	#endif
}


void TMRpcm::volume(char upDown){

  if(upDown){
	  volMod++;
  }else{
	  volMod--;
  }
}

void TMRpcm::setVolume(char vol) {
    volMod = vol - 4 ;
}

void TMRpcm::volume(char upDown,boolean which){

  if(upDown){
	  volMod++;
  }else{
	  volMod--;
  }
}

void TMRpcm::setVolume(char vol, boolean which) {
    volMod = vol - 4 ;
}

volatile boolean clear = 0;


#if !defined (USE_TIMER2)
ISR(TIMER1_CAPT_vect){
#else
ISR(TIMER2_COMPB_vect){
#endif
	//if(playing || playing2){

	b = !whichBuff2; a = !whichBuff;
	if(buffEmpty[a] || buffEmpty2[b]){
		*TIMSK[tt] &= ~togByte;
		sei();

		if(buffEmpty[a] ){

			if(sFile.read((byte*)buffer[a],buffSize) < buffSize){

				#if !defined (SDFAT)
				if(bitRead(optionByte,3)){ sFile.seek(44); *TIMSK[tt] |= togByte; return;}
				if(sFile){sFile.close();}
				#else
				if(bitRead(optionByte,3)){ sFile.seekSet(44); *TIMSK[tt] |= togByte; return;}
				if(sFile.isOpen()){sFile.close();}
		  		#endif
		  		playing = 0;
  			}
			buffEmpty[a] = 0;

		}

		if(buffEmpty2[b] ){
			if(tFile.read((byte*)buffer2[b],buffSize) < buffSize){

				#if !defined (SDFAT)
				if(bitRead(optionByte,2)){ tFile.seek(44); *TIMSK[tt] |= togByte; return;}
				if(tFile){tFile.close();}
				#else
				if(bitRead(optionByte,2)){ tFile.seekSet(44); *TIMSK[tt] |= togByte; return;}
				if(tFile.isOpen()){tFile.close();}
		  		#endif
		  		playing2 = 0;
		  	}
			buffEmpty2[b] = 0;

	    }
	if(!playing && !playing2){  *TIMSK[tt] &= ~( togByte | _BV(TOIE1) );}
	else{ 	*TIMSK[tt] |= togByte;	}
}
}


#if !defined (USE_TIMER2)
ISR(TIMER1_OVF_vect){
if(bitRead(optionByte,6)){ loadCounter = !loadCounter;if(loadCounter){ return; }}
#else

ISR(TIMER2_OVF_vect){
	switch (SR){
		case 0: if( loadCounter > 0 ){  if( loadCounter >= 3 ){loadCounter=0; return;} loadCounter++; return;} break;
		case 1: if(loadCounter > 0){ if(loadCounter >= 2){loadCounter=0; return; }loadCounter++;  return;} break;
		case 2: if(loadCounter > 0){loadCounter = 0;return;}  break;
	}
	loadCounter++;
#endif

	if(playing){

	   if(!bitRead(optionByte,4)){

		if(volMod < 0 ){
			  #if !defined (MODE2)
		  	  *OCRnA[tt] = buffer[whichBuff][buffCount] >> (volMod*-1);
		  	  #else
			  *OCRnA[tt] = *OCRnB[tt] = buffer[whichBuff][buffCount] >> (volMod*-1);
		  	  #endif
		}else{
			  #if !defined (MODE2)
			  *OCRnA[tt] = buffer[whichBuff][buffCount] << volMod;
			  #else
			  *OCRnA[tt] = *OCRnB[tt] = buffer[whichBuff][buffCount] << volMod;
			  #endif
		}
		buffCount++;
	    if(buffCount >= buffSize){
		buffCount = 0;
		buffEmpty[whichBuff] = true;
		whichBuff = !whichBuff;
	    }
	  }else{//STEREO

		if(volMod < 0 ){
			  #if !defined (MODE2)
		  	  *OCRnA[tt] = buffer[whichBuff][buffCount] >> (volMod*-1);
		  	  *OCRnB[tt] = buffer[whichBuff][buffCount+1] >> (volMod*-1);
		  	  #else
			  *OCRnA[tt] = buffer[whichBuff][buffCount] >> (volMod*-1);
			  *OCRnB[tt] = buffer[whichBuff][buffCount+1] >> (volMod*-1);
		  	  #endif
		}else{
			  #if !defined (MODE2)
			  *OCRnA[tt] = buffer[whichBuff][buffCount] << volMod;
			  *OCRnB[tt] = buffer[whichBuff][buffCount+1] << volMod;
			  #else
			  *OCRnA[tt] = buffer[whichBuff][buffCount] << volMod;
			  *OCRnB[tt] = buffer[whichBuff][buffCount+1] << volMod;
			  #endif

	    }
		buffCount+=2;
	    if(buffCount >= buffSize){
		buffCount = 0;
		buffEmpty[whichBuff] = true;
		whichBuff = !whichBuff;
	    }
	  }

	}



	if(playing2){
		if(!bitRead(optionByte,4)){
			if(volMod < 0 ){
				#if !defined (MODE2)
	  			*OCRnB[tt] = buffer2[whichBuff2][buffCount2] >> (volMod*-1);
	  			#else
				*OCRnA[tt2] = *OCRnB[tt2] = buffer2[whichBuff2][buffCount2] >> (volMod*-1);
	  			#endif
			}else{
				#if !defined (MODE2)
				*OCRnB[tt] = buffer2[whichBuff2][buffCount2] << volMod;
				#else
				*OCRnA[tt2] = *OCRnB[tt2] = buffer2[whichBuff2][buffCount2] << volMod;
				#endif
		    }
			  buffCount2++;
			  if(buffCount2 >= buffSize){
				buffCount2 = 0;
				buffEmpty2[whichBuff2] = true;
				whichBuff2 = !whichBuff2;
			  }


	    }else{
			if(volMod < 0 ){
				#if !defined (MODE2)
	  			*OCRnB[tt] = buffer2[whichBuff2][buffCount2] >> (volMod*-1);
		  		*OCRnA[tt] = buffer2[whichBuff2][buffCount2+1] >> (volMod*-1);
	  			#else
				*OCRnA[tt2] = buffer2[whichBuff2][buffCount2] >> (volMod*-1);
				*OCRnB[tt2] = buffer2[whichBuff2][buffCount2+1] >> (volMod*-1);
	  			#endif
			}else{
				#if !defined (MODE2)
				*OCRnB[tt] = buffer2[whichBuff2][buffCount2] << volMod;
				*OCRnA[tt] = buffer2[whichBuff2][buffCount2+1] << volMod;
				#else
				*OCRnA[tt2] = buffer2[whichBuff2][buffCount2] << volMod;
				*OCRnB[tt2] = buffer2[whichBuff2][buffCount2+1] << volMod;
				#endif
		    }
			  buffCount2 += 2;
				  if(buffCount2 >= buffSize){
				buffCount2 = 0;
				buffEmpty2[whichBuff2] = true;
				whichBuff2 = !whichBuff2;
			  }


		}
	}

}


void TMRpcm::stopPlayback(boolean which){
	if(!playing && !playing2){
		*TIMSK[tt] &= ~( togByte | _BV(TOIE1) );
	}
	if(!which){
		playing = 0;
		if(ifOpen()){sFile.close();}
	}else{
		playing2 = 0;
		#if !defined (SDFAT)
		if(tFile){tFile.close();}
		#else
		if(tFile.isOpen()){tFile.close();}
		#endif
	}
}

void TMRpcm::disable(){
	playing = 0; playing2 = 0;
	*TIMSK[tt] &= ~( togByte | _BV(TOIE1) );
	if(ifOpen()){sFile.close(); }
		#if !defined (SDFAT)
			if(tFile){tFile.close();}
		#else
			if(tFile.isOpen()){tFile.close();}
		#endif
	int current = *OCRnA[tt];
	int currentB = *OCRnB[tt];
	for(int i=0; i < resolution; i++){
	#if defined(rampMega)
		*OCRnB[tt] = constrain((current + i),0,resolution);
		*OCRnA[tt] = constrain((currentB - i),0,resolution);
	#else
		*OCRnB[tt] = constrain((current - i),0,resolution);
		*OCRnA[tt] = constrain((currentB - i),0,resolution);
	#endif
		delayMicroseconds(50);
	}
	#if defined (MODE2)
		current = *OCRnA[tt2];
		currentB = *OCRnB[tt2];
		for(int i=0; i < resolution; i++){
			*OCRnB[tt2] = constrain((current + i),0,resolution);
		  	*OCRnA[tt2] = constrain((currentB - i),0,resolution);
		  	delayMicroseconds(50);
		}
  		*TCCRnA[tt2] = *TCCRnB[tt2] = 0;
  	#endif
    //rampUp = 1;
    bitSet(optionByte,5);
    *TCCRnA[tt] = *TCCRnB[tt] = 0;
}


boolean TMRpcm::isPlaying(){
	return (playing | playing2);
}

boolean TMRpcm::isPlaying(boolean which){
	if(!which){	return playing; }
	else {	    return playing2;}
}

#endif





//****************** Metadata Features ****************************
//****************** ID3 and LIST Tags ****************************



byte TMRpcm::getInfo(char* filename, char* tagData, byte infoNum){
	byte gotInfo = 0;
	if( (gotInfo = metaInfo(1,filename, tagData, infoNum)) < 1){
		gotInfo = metaInfo(0,filename, tagData, infoNum);
	}
	return gotInfo;

}

byte TMRpcm::listInfo(char* filename, char* tagData, byte infoNum){
	return metaInfo(0, filename, tagData, infoNum);

}



//http://id3.org/id3v2.3.0
byte TMRpcm::id3Info(char* filename, char* tagData, byte infoNum){
	return metaInfo(1, filename, tagData, infoNum);
}


#if !defined (SDFAT)
boolean TMRpcm::searchMainTags(File xFile, char *datStr){
	xFile.seek(36);
#else
unsigned long TMRpcm::searchMainTags(SdFile xFile, char *datStr){
	xFile.seekSet(36);
#endif
		boolean found = 0;
	    char dChars[4] = {'d','a','t','a'};
		char tmpChars[4];

		//xFile.seek(36);
	    xFile.read((char*)tmpChars,4);
	    for (byte i =0; i<4; i++){
			if(tmpChars[i] != dChars[i]){
				#if !defined (SDFAT)
				 	xFile.seek(40);
				 	unsigned int siz = xFile.read(); siz = (xFile.read() << 8 | siz)+2;
				 	xFile.seek(xFile.position() + siz);
				#else
					xFile.seekSet(40);
					unsigned int siz = xFile.read(); siz = (xFile.read() << 8 | siz)+2;
				 	xFile.seekSet(xFile.curPosition() + siz);
				#endif
			 	xFile.read((char*)tmpChars,4);
 	     		for (byte i =0; i<4; i++){
					if(tmpChars[i] != dChars[i]){
						return 0;
					}
		 		}
			}
		 }

		unsigned long tmpp=0;
		unsigned long daBytes = xFile.read();
	    for (byte i =8; i<32; i+=8){
			tmpp = xFile.read();
			daBytes = tmpp << i | daBytes;
		}

		#if !defined (SDFAT)
			daBytes = xFile.position() + daBytes;
			if(xFile.size() == daBytes){ return 0; }
		#else
			daBytes = xFile.curPosition() + daBytes;
			if(xFile.fileSize() == daBytes){ return 0; }
		#endif

	//if(found == 0){ //Jump to file end - 1000 and search for ID3 or LIST
	#if !defined (SDFAT)
		xFile.seek(daBytes);
	#else
		xFile.seekSet(daBytes);
	#endif

		while(xFile.available() > 5){
			if(xFile.read() == datStr[0] && xFile.peek() == datStr[1]){
				xFile.read((char*)tmpChars,3);
	 			if( tmpChars[1] == datStr[2] &&  tmpChars[2] == datStr[3] ){
						found = 1;
						#if !defined (SDFAT)
							return 1; break;
						#else
							return xFile.curPosition();
						#endif
				}else{
					#if !defined (SDFAT)
						xFile.seek(xFile.position() - 1 - 4); //pos - tagSize
					#else
						unsigned long pos = xFile.curPosition()-1;
						xFile.seekSet(pos - 4);
					#endif
				}
			}
		}
		return 0;

}



byte TMRpcm::metaInfo(boolean infoType, char* filename, char* tagData, byte whichInfo){


	if(ifOpen()){ noInterrupts();}

	#if !defined (SDFAT)
		File xFile;
		xFile = SD.open(filename);
		xFile.seek(36);
	#else
		SdFile xFile;
		xFile.open(filename);
		xFile.seekSet(36);
	#endif

	boolean found=0;
		char* datStr = "LIST";
		if(infoType == 1){datStr = "ID3 "; datStr[3] = 3;}
		char tmpChars[4];

	if(infoType == 0){ //if requesting LIST info, check for data at beginning of file first
		xFile.read((char*)tmpChars,4);
		for (byte i=0; i<4; i++){ //4 tagSize
			if(tmpChars[i] != datStr[i]){
				break;
		  	}else if(i==3){
				found = 1;
		  	}
		}
	}
	if(found == 0){
		#if !defined (SDFAT)
			found = searchMainTags(xFile, datStr);
		#else
			unsigned long pos = searchMainTags(xFile, datStr);
			xFile.seekSet(pos);
			if(pos > 0){ found = 1; }
		#endif
	}

//** This section finds the starting point and length of the tag info
	if(found == 0){ xFile.close(); if(ifOpen()){ interrupts();} return 0; }

	unsigned long listEnd;
	unsigned int listLen;
	char* tagNames[] = {"INAM","IART","IPRD"};

	if(infoType == 0){ //LIST format
		listLen = xFile.read(); listLen = xFile.read() << 8 | listLen;
		#if !defined (SDFAT)
			xFile.seek(xFile.position() +6);
			listEnd = xFile.position() + listLen;
		#else
			xFile.seekSet(xFile.curPosition() +6);
			listEnd = xFile.curPosition() + listLen;
		#endif

	}else{				//ID3 format

		#if !defined (SDFAT)
			xFile.seek(xFile.position() + 5);
		#else
			xFile.seekSet(xFile.curPosition() + 5);
		#endif
			listLen = xFile.read() << 7 | listLen; listLen = xFile.read() | listLen;
			tagNames[0] = "TPE1"; tagNames[1] ="TIT2"; tagNames[2] ="TALB";
		#if !defined (SDFAT)
			listEnd = xFile.position() + listLen;
		#else
			listEnd = xFile.curPosition() + listLen;
		#endif
	}

	char tgs[4];
	unsigned int len = 0;
	unsigned long tagPos = 0;

//** This section reads the tags and gets the size of the tag data and its position
//** Should work with very long tags if a big enough buffer is provided
#if !defined (SDFAT)
	while(xFile.position() < listEnd){
#endif
#if defined (SDFAT)
	while(xFile.curPosition() < listEnd){
#endif

		xFile.read((char*)tgs,4);

		if(infoType == 0){ //LIST
			len = xFile.read()-1;
			len = xFile.read() << 8 | len;
			#if !defined (SDFAT)
				xFile.seek(xFile.position()+2);
			#else
				xFile.seekSet(xFile.curPosition()+2);
			#endif
		}else{ 				//ID3
			#if !defined (SDFAT)
				xFile.seek(xFile.position()+3);
			#else
				xFile.seekSet(xFile.curPosition()+3);
			#endif
			len = xFile.read();
			len = xFile.read() << 8 | len;
			len = (len-3)/2;
			#if !defined (SDFAT)
				tagPos = xFile.position() + 4;
				xFile.seek(tagPos);
			#else
			    xFile.seekSet(xFile.curPosition() +4);
				tagPos = xFile.curPosition();
			#endif

		}

		found =0;
	//** This section checks to see if the tag we found is the one requested
	//** If so, it loads the data into the buffer
		for(int p=0; p<4;p++){
			if(tgs[p] != tagNames[whichInfo][p]){
				break;
			}else{
				if(p==3){
					if(infoType == 1){
						byte junk;
						for(byte j=0; j<len; j++){
							tagData[j] = xFile.read();
							junk=xFile.read();
						}
					}else{
						xFile.read((char*)tagData,len);
					}
					tagData[len] = '\0';
					xFile.close();
					if(ifOpen()){ interrupts();}
 					return len;
				}
			}
		}

		if(found){break;}

	//**This section jumps to the next tag position if the requested tag wasn't found
	#if !defined (SDFAT)
		if(infoType == 0){
			if(!found){	xFile.seek(xFile.position()+len);}
			while(xFile.peek() == 0){xFile.read();}
		}else{
			if(!found){	xFile.seek(tagPos+len); }
			while(xFile.peek() != 'T'){xFile.read();}
		}
	#else
		if(infoType == 0){
			if(!found){	xFile.seekSet(xFile.curPosition()+len);}
			while(xFile.peek() == 0){xFile.read();}
		}else{
			if(!found){	xFile.seekSet(tagPos+len); }
			while(xFile.peek() != 'T'){xFile.read();}
		}
	#endif


 	}
 	xFile.close();
 	if(ifOpen()){ interrupts();}
 	return 0;
}


/*********************************************************************************
********************** DIY Digital Audio Generation ******************************/


void TMRpcm::finalizeWavTemplate(char* filename){
	disable();

	unsigned long fSize = 0;

  #if !defined (SDFAT)
		sFile = SD.open(filename,FILE_WRITE);

    if(!sFile){
		#if defined (debug)
			Serial.println("fl");
		#endif
		return;
	}
	fSize = sFile.size()-8;

  #else
		sFile.open(filename,O_WRITE );

    if(!sFile.isOpen()){
		#if defined (debug)
			Serial.println("failed to finalize");
		#endif
		return; }
    fSize = sFile.fileSize()-8;

  #endif



	seek(4); byte data[4] = {lowByte(fSize),highByte(fSize), fSize >> 16,fSize >> 24};
	sFile.write(data,4);
	byte tmp;
	seek(40);
	fSize = fSize - 36;
	data[0] = lowByte(fSize); data[1]=highByte(fSize);data[2]=fSize >> 16;data[3]=fSize >> 24;
	sFile.write((byte*)data,4);
	sFile.close();

	#if defined (debug)
	#if !defined (SDFAT)
		sFile = SD.open(filename);
	#else
		sFile.open(filename);
	#endif

	if(ifOpen()){

		    while (fPosition() < 44) {
		      Serial.print(sFile.read(),HEX);
		      Serial.print(":");
		   	}
		   	Serial.println("");

	   	//Serial.println(sFile.size());
    	sFile.close();
	}
	#endif
}



void TMRpcm::createWavTemplate(char* filename, unsigned int sampleRate){
	disable();

  #if !defined (SDFAT)
  	if(SD.exists(filename)){SD.remove(filename);}
  #endif

  #if defined (ENABLE_RECORDING)
    SdVolume vol;
	SdFile rut;
	SdFile fil;

	char* fNam = filename;
	uint32_t bgnBlock, endBlock;

	if (!card1.init(SPI_FULL_SPEED,CSPin)) {
	    return;
  	}else{}//Serial.println("SD OK");}

	if(!vol.init(&card1)){}//Serial.println("card failed"); }
	if (!rut.openRoot(&vol)) {}//Serial.println("openRoot failed"); }
	SdFile::remove(&rut, fNam);

	if (!fil.createContiguous(&rut, fNam, 512UL*BLOCK_COUNT)) {
	    //Serial.println("createContiguous failed");
	  }
	  // get the location of the file's blocks
	  if (!fil.contiguousRange(&bgnBlock, &endBlock)) {
	    //Serial.println("contiguousRange failed");
	  }
	  if (!card1.erase(bgnBlock, endBlock)) //Serial.println("card.erase failed");

	rut.close();
	fil.close();
#endif

  #if !defined (SDFAT)
  		if(SD.exists(filename)){SD.remove(filename);}
		sFile = SD.open(filename,FILE_WRITE);
	if(!sFile){
		#if defined (debug)
			Serial.println("failed to open 4 writing");
		#endif
		return;
	}else{

  #else
   	sFile.open(filename,O_CREAT | O_WRITE);
   	if(sFile.fileSize() > 44 ){ sFile.truncate(44);}
	if(!sFile.isOpen()){
		#if defined (debug)
			Serial.println("failed to open 4 writing");
		#endif
		return;
	}else{

  #endif
  		//Serial.print("Sr: ");
  		//Serial.println(sampleRate);
  		seek(0);
		byte data[] = {16,0,0,0,1,0,1,0,lowByte(sampleRate),highByte(sampleRate)};
		sFile.write((byte*)"RIFF    WAVEfmt ",16);
		sFile.write((byte*)data,10);
		//unsigned int byteRate = (sampleRate/8)*monoStereo*8;
		//byte blockAlign = monoStereo * (bps/8); //monoStereo*(bps/8)
		data[0] = 0; data[1] = 0; data[2] = lowByte(sampleRate); data[3] =highByte(sampleRate);//Should be byteRate
		data[4]=0;data[5]=0;data[6]=1; //BlockAlign
		data[7]=0;data[8]=8;data[9]=0;
		sFile.write((byte*)data,10);
		sFile.write((byte*)"data    ",8);
		//Serial.print("siz");
		//Serial.println(sFile.size());
		sFile.close();

	}
}

#if defined (ENABLE_RECORDING)

void TMRpcm::startRecording(char *fileName, unsigned int SAMPLE_RATE, byte pin){
	startRecording(fileName,SAMPLE_RATE,pin,0);
}

void TMRpcm::startRecording(char *fileName, unsigned int SAMPLE_RATE, byte pin, byte passThrough){

	recording = passThrough + 1;
	setPin();
	if(recording < 3){
		//*** Creates a blank WAV template file. Data can be written starting at the 45th byte ***
		createWavTemplate(fileName, SAMPLE_RATE);

		//*** Open the file and seek to the 44th byte ***
	  #if !defined (SDFAT)
		sFile = SD.open(fileName,FILE_WRITE);
		if(!sFile){
			#if defined (debug)
				Serial.println("fail");
			#endif
			return;
		}
  	  #else
    	sFile.open(fileName,O_WRITE );
    	if(!sFile.isOpen()){
			#if defined (debug)
				Serial.println("fail");
			#endif
			return;
		}

  	  #endif
	seek(44);
 	}
	buffCount = 0; buffEmpty[0] = 1; buffEmpty[1] = 1;


	/*** This section taken from wiring_analog.c to translate between pins and channel numbers ***/
	#if defined(analogPinToChannel)
	#if defined(__AVR_ATmega32U4__)
		if (pin >= 18) pin -= 18; // allow for channel or pin numbers
	#endif
		pin = analogPinToChannel(pin);
	#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
		if (pin >= 54) pin -= 54; // allow for channel or pin numbers
	#elif defined(__AVR_ATmega32U4__)
		if (pin >= 18) pin -= 18; // allow for channel or pin numbers
	#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
		if (pin >= 24) pin -= 24; // allow for channel or pin numbers
	#else
		if (pin >= 14) pin -= 14; // allow for channel or pin numbers
	#endif

	#if defined(ADCSRB) && defined(MUX5)
		// the MUX5 bit of ADCSRB selects whether we're reading from channels
		// 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
		ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
	#endif

	#if defined(ADMUX)
		ADMUX = (pin & 0x07);
	#endif

	//Set up the timer
	if(recording > 1){

		*TCCRnA[tt] = _BV(COM1A1); //Enable the timer port/pin as output for passthrough

	}
    *ICRn[tt] = 10 * (1600000/SAMPLE_RATE);//Timer will count up to this value from 0;
	*TCCRnA[tt] |= _BV(WGM11); //WGM11,12,13 all set to 1 = fast PWM/w ICR TOP
	*TCCRnB[tt] = _BV(WGM13) | _BV(WGM12) | _BV(CS10); //CS10 = no prescaling

	if(recording < 3){ //Normal Recording
		*TIMSK[tt] |=  _BV(OCIE1B)| _BV(OCIE1A); //Enable the TIMER1 COMPA and COMPB interrupts
	}else{
		*TIMSK[tt] |=  _BV(OCIE1B); //Direct pass through to speaker, COMPB only
	}


	ADMUX |= _BV(REFS0) | _BV(ADLAR);// Analog 5v reference, left-shift result so only high byte needs to be read
	ADCSRB |= _BV(ADTS0) | _BV(ADTS2);  //Attach ADC start to TIMER1 Compare Match B flag
	byte prescaleByte = 0;

	if(      SAMPLE_RATE < 18000){ prescaleByte = B00000110;} //ADC division factor 64 (16MHz / 64 / 13clock cycles = 19230 Hz Max Sample Rate )
	else if( SAMPLE_RATE < 27000){ prescaleByte = B00000101;} //32  (38461Hz Max)
	else{	                       prescaleByte = B00000100;} //16  (76923Hz Max)
	ADCSRA = prescaleByte; //Adjust sampling rate of ADC depending on sample rate
	ADCSRA |= _BV(ADEN) | _BV(ADATE); //ADC Enable, Auto-trigger enable


}

void TMRpcm::stopRecording(char *fileName){

	*TIMSK[tt] &= ~(_BV(OCIE1B) | _BV(OCIE1A));
	ADCSRA = 0;
    ADCSRB = 0;

	if(recording == 1 || recording == 2){
		recording = 0;
		unsigned long position = fPosition();
		#if defined (SDFAT)
			sFile.truncate(position);
		#endif
		sFile.close();
		finalizeWavTemplate(fileName);
	}
}


#endif

#endif // Not defined RF_ONLY

