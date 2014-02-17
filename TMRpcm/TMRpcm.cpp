/*Library by TMRh20 2012-2014*/

#include <SD.h>
#include <TMRpcm.h>



//********************* Timer arrays and pointers **********************

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#if !defined (buffSize)
	#define buffSize 128
#endif

	volatile byte *TIMSK[] = {&TIMSK1,&TIMSK3,&TIMSK4,&TIMSK5};
	volatile byte *TCCRnA[] = {&TCCR1A,&TCCR3A,&TCCR4A,&TCCR5A};
	volatile byte *TCCRnB[] = {&TCCR1B, &TCCR3B,&TCCR4B,&TCCR5B};
	volatile unsigned int *OCRnA[] = {&OCR1A,&OCR3A,&OCR4A,&OCR5A};
	volatile unsigned int *OCRnB[] = {&OCR1B, &OCR3B,&OCR4B,&OCR5B};
	volatile unsigned int *ICRn[]	= {&ICR1, &ICR3,&ICR4,&ICR5};

	ISR(TIMER3_OVF_vect, ISR_ALIASOF(TIMER1_OVF_vect));
	ISR(TIMER3_CAPT_vect, ISR_ALIASOF(TIMER1_CAPT_vect));

	ISR(TIMER4_OVF_vect, ISR_ALIASOF(TIMER1_OVF_vect));
	ISR(TIMER4_CAPT_vect, ISR_ALIASOF(TIMER1_CAPT_vect));

	ISR(TIMER5_OVF_vect, ISR_ALIASOF(TIMER1_OVF_vect));
	ISR(TIMER5_CAPT_vect, ISR_ALIASOF(TIMER1_CAPT_vect));

#elif defined (__AVR_ATmega32U4__) || (__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__) || (__AVR_ATmega128__) ||defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)

	#if !defined buffSize
		#define buffSize 128
	#endif

	volatile byte *TIMSK[] = {&TIMSK1,&TIMSK3};
	volatile byte *TCCRnA[] = {&TCCR1A,&TCCR3A};
	volatile byte *TCCRnB[] = {&TCCR1B, &TCCR3B};
	volatile unsigned int *OCRnA[] = {&OCR1A,&OCR3A};
	volatile unsigned int *OCRnB[] = {&OCR1B, &OCR3B};
	volatile unsigned int *ICRn[]	= {&ICR1, &ICR3};

	ISR(TIMER3_OVF_vect, ISR_ALIASOF(TIMER1_OVF_vect));
	ISR(TIMER3_CAPT_vect, ISR_ALIASOF(TIMER1_CAPT_vect));

#else
	#if !defined (buffSize)
		#define buffSize 64
	#endif

	volatile byte *TIMSK[] = {&TIMSK1};
	volatile byte *TCCRnA[] = {&TCCR1A};
	volatile byte *TCCRnB[] = {&TCCR1B};
	volatile unsigned int *OCRnA[] = {&OCR1A};
	volatile unsigned int *OCRnB[] = {&OCR1B};
	volatile unsigned int *ICRn[]	= {&ICR1};
#endif



#if defined (ENABLE_MULTI)

	#if defined (MODE2)
		byte tt2 = 0;
	#endif
	volatile byte buffer2[2][buffSize], buffCount2 = 0;
	volatile boolean buffEmpty2[2] = {false,false}, whichBuff2 = false, playing2 = 0;
	char volMod2=0;
	volatile boolean paused2 = 0, stopPlay2 = 0, open2 = 0, open1 = 0, load1 = 0, load2 = 0,a,b;
	char *fileName2;
	File tFile;

#endif

//*********** Global Variables ***************
volatile int resolution = 500;
volatile boolean buffEmpty[2] = {false,false}, whichBuff = false, loadCounter=0, playing = 0;
boolean paused = 0, qual = 0, stopPlay = 0, rampUp = 1;
char volMod=0;
volatile byte buffer[2][buffSize], buffCount = 0;
byte tt=0;

File sFile;


//********** Functions used in all modes *****

void TMRpcm::quality(boolean q){
	if(!playing){qual = q;}
}

void TMRpcm::timerSt(){
	*ICRn[tt] = resolution;
	*TCCRnA[tt] = _BV(WGM11) | _BV(COM1A1) | _BV(COM1B0) | _BV(COM1B1); //WGM11,12,13 all set to 1 = fast PWM/w ICR TOP
	*TCCRnB[tt] = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
  #if defined (MODE2)
  	*ICRn[tt2] = resolution;
	*TCCRnA[tt2] = _BV(WGM11) | _BV(COM1A1) | _BV(COM1B0) | _BV(COM1B1); //WGM11,12,13 all set to 1 = fast PWM/w ICR TOP
	*TCCRnB[tt2] = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
  #endif
}

boolean TMRpcm::wavInfo(char* filename){

  //check for the string WAVE starting at 8th bit in header of file
  File xFile = SD.open(filename);
  if(!xFile){return 0;}
  xFile.seek(8);
  char wavStr[] = {'W','A','V','E'};
  for (int i =0; i<4; i++){
	  if(xFile.read() != wavStr[i]){ Serial.println("WAV File Error"); break; }
  }

    xFile.seek(24);
    SAMPLE_RATE = xFile.read();
    SAMPLE_RATE = xFile.read() << 8 | SAMPLE_RATE;

    //verify that Bits Per Sample is 8 (0-255)
    xFile.seek(34); unsigned int dVar = xFile.read();
    dVar = xFile.read() << 8 | dVar;
    if(dVar != 8){Serial.print("Wrong BitRate"); xFile.close(); return 0;}
    xFile.close(); return 1;
}


void TMRpcm::stopPlayback(){
	playing = 0;
	*TIMSK[tt] &= ~( _BV(ICIE1) | _BV(TOIE1) );
	if(sFile){sFile.close();}
	#if defined (ENABLE_MULTI)
		playing2 = 0;
		if(tFile){tFile.close();}
	#endif

}

void TMRpcm::pause(){
	paused = !paused;
	if(!paused && playing){
		*TIMSK[tt] |= ( _BV(ICIE1) | _BV(TOIE1) );
	}else if(paused && playing){
		*TIMSK[tt] &= ~( _BV(TOIE1) );
	}
}

void TMRpcm::setPin(){
	disable();
	pinMode(speakerPin,OUTPUT);
	switch(speakerPin){
		case 5: tt=1; break; //use TIMER3
		case 6: tt=2; break;//use TIMER4
		case 46:tt=3; break;//use TIMER5
		default:tt=0; break; //useTIMER1 as default
	}
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

//***************************************************************************************
//********************** Functions for single track playback ****************************

#if !defined (ENABLE_MULTI) //Below section for normal playback of 1 track at a time




void TMRpcm::play(char* filename){

  if(speakerPin != lastSpeakPin){ setPin(); lastSpeakPin=speakerPin;}
  stopPlayback();

  if(!wavInfo(filename) ){ return; }//verify its a valid wav file
  sFile = SD.open(filename);

  if(sFile){
	playing = 1; paused = 0;
    sFile.seek(44); //skip the header info

	if(SAMPLE_RATE > 45050 ){ SAMPLE_RATE = 24000; Serial.print("SampleRate Too High");}


    if(qual){resolution = 8 * (1000000/SAMPLE_RATE);}
    else{ resolution = 16 * (1000000/SAMPLE_RATE);
	}

    byte tmp = sFile.read();

    if(rampUp){
		*OCRnA[tt] = 0; *OCRnB[tt] = resolution;
		timerSt();
		rampUp = 0;
		for(int i=0; i < resolution; i++){
			*OCRnB[tt] = constrain(resolution-i,0,resolution);
			delayMicroseconds(50);

		}
	}

	byte mod;
	if(volMod > 0){ mod = *OCRnA[tt] >> volMod; }else{ mod = *OCRnA[tt] << (volMod*-1); }
	if(tmp > mod){
		for(int i=0; i<buffSize; i++){ mod = constrain(mod+1,mod, tmp); buffer[0][i] = mod; }
		for(int i=0; i<buffSize; i++){ mod = constrain(mod+1,mod, tmp); buffer[1][i] = mod; }
	}else{
		for(int i=0; i<buffSize; i++){ mod = constrain(mod-1,tmp ,mod); buffer[0][i] = mod; }
		for(int i=0; i<buffSize; i++){ mod = constrain(mod-1,tmp, mod); buffer[1][i] = mod; }
	}

    whichBuff = 0; buffEmpty[0] = 0; buffEmpty[1] = 0; buffCount = 0;

    noInterrupts();
	timerSt();
    *TIMSK[tt] = ( _BV(ICIE1) | _BV(TOIE1) );
    interrupts();


  }else{Serial.println("Read fail"); }

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


ISR(TIMER1_CAPT_vect){


  // The first step is to disable this interrupt before manually enabling global interrupts.
  // This allows this interrupt vector (COMPB) to continue loading data while allowing the overflow interrupt
  // to interrupt it. ( Nested Interrupts )
  // TIMSK1 &= ~_BV(ICIE1);
 //Then enable global interupts before this interrupt is finished, so the music can interrupt the buffering
  //sei();


	  boolean a = !whichBuff;
	  if(buffEmpty[a]){
		*TIMSK[tt] &= ~(_BV(ICIE1));
		sei();
		if(sFile.read((byte*)buffer[a],buffSize) < buffSize){
		  	playing = 0;
		  	*TIMSK[tt] &= ~( _BV(ICIE1) | _BV(TOIE1) );
		  	if(sFile){sFile.close();}
	  	}else{
			buffEmpty[a] = 0;
			*TIMSK[tt] |= _BV(ICIE1);
		}
	  }


}


ISR(TIMER1_OVF_vect){

  if(qual){loadCounter = !loadCounter;if(loadCounter){ return; }}

  if(volMod < 0 ){
    *OCRnA[tt] = *OCRnB[tt] = buffer[whichBuff][buffCount] >> (volMod*-1);
  }else{
	*OCRnA[tt] = *OCRnB[tt] = buffer[whichBuff][buffCount] << volMod;
  }
  ++buffCount;

  if(buffCount >= buffSize){
      buffCount = 0;
      buffEmpty[whichBuff] = true;
      whichBuff = !whichBuff;
  }

}




void TMRpcm::disable(){
  playing = 0;
  *TIMSK[tt] &= ~( _BV(ICIE1) | _BV(TOIE1) );
  if(sFile){sFile.close();}
  int current = *OCRnA[tt];
  for(int i=0; i < resolution; i++){
	*OCRnB[tt] = constrain((current + i),0,resolution);
	*OCRnA[tt] = constrain((current - i),0,resolution);
	delayMicroseconds(50);
  }
  rampUp = 1;
  *TCCRnA[tt] = *TCCRnB[tt] = 0;
}


boolean TMRpcm::isPlaying(){
	return playing;
}






//***************************************************************************************
//********************** Functions for dual track playback ****************************
#else //Else Dual






byte hold = 0;

void TMRpcm::play(char* filename, boolean which){

  if(speakerPin != lastSpeakPin){
	  #if defined (MODE2)
	  setPins();
	  #else
	  setPin();
	  #endif
	  lastSpeakPin=speakerPin;
  }

  byte dual = 0;

    if(rampUp){
		*OCRnA[tt] = 0; *OCRnB[tt] = resolution;
		#if defined (MODE2)
			*OCRnA[tt2] = 0; *OCRnB[tt2] = resolution;
		#endif
		timerSt();
		rampUp = 0;
		for(int i=0; i < resolution; i++){
			*OCRnB[tt] = constrain(resolution-i,0,resolution);
			#if defined (MODE2)
				*OCRnB[tt2] = constrain(resolution-i,0,resolution);
			#endif
			delayMicroseconds(50);

		}


	}

  if((!playing && !playing2) || (playing && !playing2 && !which) || (playing2 && !playing && which) ){
	  stopPlayback();
	  if(!wavInfo(filename) ){ return; }//verify its a valid wav file
	  if(!which){
	  	sFile = SD.open(filename,FILE_READ);
	  }else{
	    tFile = SD.open(filename,FILE_READ);
  	  }
  	  	if(SAMPLE_RATE > 45050 ){ SAMPLE_RATE = 24000; Serial.print("SampleRate Too High");}

	    if(qual){resolution = 8 * (1000000/SAMPLE_RATE);}
        else{ resolution = 16 * (1000000/SAMPLE_RATE);
	  }

  }else{
	  if(!which){ playing = 0; sFile.close(); dual = 1; }
	  if(which ){ playing2 = 0; tFile.close(); dual = 1; }

  }


 if(!dual){

	if(!which){
		//playing = 1;
		paused = 0;
		if(sFile){
	    	ramp(0);
		}else{Serial.println("Read fail"); return; }
	}else{
		//playing2 = 1;
		paused = 0;
		if(tFile){
		    ramp(1);
		}else{Serial.println("Read fail"); return; }
	}


    noInterrupts();
    timerSt();
    *TIMSK[tt] = ( _BV(ICIE1) | _BV(TOIE1) );
    //if(!once){ *OCRnA[tt] = 0; *OCRnB[tt] = resolution; once = 1;}
	interrupts();

  }

  //if(!which){playing = 1;}else{playing2 = 1;}
  if(dual){
	  //while(buffCount > 0){}

	*TIMSK[tt] &= ~(_BV(ICIE1));
	if(!which){
		if(sFile){sFile.close();}
		sFile = SD.open(filename);
		if(sFile){ ramp(0); hold = 1; playing = 1; }else{ Serial.println("Dual Read fail"); return; }//open1 = 1; fileName2 = filename;  }
	}else
	if(which){
		if(tFile){tFile.close();}
		tFile = SD.open(filename);
		if(tFile){ ramp(1); hold = 2; playing2 = 1; }else{Serial.println("Dual Read fail"); return; } //open1 = 1; fileName2 = filename;  }
	}
	*TIMSK[tt] |= (_BV(ICIE1));
	//interrupts();
  }else{
	if(!which){playing = 1;}
	else{ playing2 = 1; }
  }



}

void TMRpcm::ramp(boolean wBuff){


	  byte tmp;
	  byte mod;

	#if !defined (MODE2)
	  if(!wBuff){
		  sFile.seek(44);
	  	  tmp = sFile.read();
		  if(volMod >= 0){ mod = *OCRnA[tt] >> volMod; }else{ mod = *OCRnA[tt] << (volMod*-1); }
		if(tmp > mod){
			for(byte i=0; i<buffSize; i++){ mod = constrain(mod+1,mod, tmp); buffer[0][i] = mod; }
		}else{
			for(byte i=0; i<buffSize; i++){ mod = constrain(mod-1,tmp ,mod); buffer[0][i] = mod; }
		}
		whichBuff = 0; buffEmpty[0] = 0;buffCount = 0; //buffEmpty[1] = 0;
	  }else{
		  tFile.seek(44);
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
		  sFile.seek(44);
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
		  tFile.seek(44);
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



ISR(TIMER1_CAPT_vect){
	//if(playing || playing2){

	b = !whichBuff2; a = !whichBuff;
	if(buffEmpty[a] || buffEmpty2[b]){
		*TIMSK[tt] &= ~(_BV(ICIE1));
		sei();

		if(buffEmpty[a] ){

			if(sFile.read((byte*)buffer[a],buffSize) < buffSize){
				playing = 0;
				if(sFile){sFile.close();}
  			}
			buffEmpty[a] = 0;

		}

		if(buffEmpty2[b] ){
			if(tFile.read((byte*)buffer2[b],buffSize) < buffSize){
				playing2 = 0;
				if(tFile){tFile.close();}
		  	}
			buffEmpty2[b] = 0;

	    }
	if(!playing && !playing2){  *TIMSK[tt] &= ~( _BV(ICIE1) | _BV(TOIE1) );}
	else{ 	*TIMSK[tt] |= ( _BV(ICIE1) ); }
	}
}



ISR(TIMER1_OVF_vect){
if(qual){ loadCounter = !loadCounter;if(loadCounter){ return; }}

    if(playing){

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
	}

	if(playing2){

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
	}

}


void TMRpcm::stopPlayback(boolean which){
	if(!playing && !playing2){
		*TIMSK[tt] &= ~( _BV(ICIE1) | _BV(TOIE1) );
	}
	if(!which){ playing = 0; if(sFile){sFile.close();} }else{ playing2 = 0; if(tFile){tFile.close();}}


}

void TMRpcm::disable(){
  playing = 0; playing2 = 0;
  *TIMSK[tt] &= ~( _BV(ICIE1) | _BV(TOIE1) );
  if(sFile){sFile.close();}
  if(tFile){tFile.close();}
  int current = *OCRnA[tt];
  int currentB = *OCRnB[tt];
  for(int i=0; i < resolution; i++){
	*OCRnB[tt] = constrain((current + i),0,resolution);
	*OCRnA[tt] = constrain((currentB - i),0,resolution);
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
  rampUp = 1;
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
