/*Library by TMRh20 2012-2013*/

#include <SD.h>
#include <TMRpcm.h>

const byte buffSize = 144; //note: there are 2 sound buffers. This will require (soundBuff*2) memory free
volatile byte buffer[2][buffSize], buffCount = 0;
volatile int resolution = 500, bcnt[2];
volatile boolean buffEmpty[2] = {false,false}, whichBuff = false, loadCounter=0, playing = 0;
unsigned int tt=0;
char volMod=0;
boolean paused = 0, qual = 0, stopPlay = 0;

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)
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


#else
	volatile byte *TIMSK[] = {&TIMSK1};
	volatile byte *TCCRnA[] = {&TCCR1A};
	volatile byte *TCCRnB[] = {&TCCR1B};
	volatile unsigned int *OCRnA[] = {&OCR1A};
	volatile unsigned int *OCRnB[] = {&OCR1B};
	volatile unsigned int *ICRn[]	= {&ICR1};
#endif


File sFile;


void TMRpcm::setPin(){

	disable();
	pinMode(speakerPin,OUTPUT);
	switch(speakerPin){
		case 5: tt=1; break; //use TIMER3
		case 6: tt=2; break;//use TIMER4
		case 46:tt=3; break;//use TIMER5
		default:tt=0; break; //useTIMER1 as default
	}
}

void TMRpcm::quality(boolean q){
	if(!playing){qual = q;}
}

void TMRpcm::play(char* filename){

  if(speakerPin != lastSpeakPin){ setPin(); lastSpeakPin=speakerPin;}
  stopPlayback();

  if(!wavInfo(filename) ){ return; }//verify its a valid wav file
  sFile = SD.open(filename);

  if(sFile){
	playing = 1; paused = 0;
    sFile.seek(44); //skip the header info

	if(SAMPLE_RATE > 35050 ){ SAMPLE_RATE = 24000; Serial.print("SampleRate Too High");}


    if(qual){resolution = 8 * (1000000/SAMPLE_RATE);}
    else{ resolution = 16 * (1000000/SAMPLE_RATE);
	}

	unsigned int tmp = 0;
	tmp = sFile.read();
	unsigned int mod = *OCRnA[tt];
	int mVal;
	if(tmp > mod){ mVal = 3;
	}else{         mVal = -3;}

	for(int i=0; i<buffSize; i++){ mod = constrain(mod+mVal,mod,tmp); buffer[0][i] = mod; }
	for(int i=0; i<buffSize; i++){ mod = constrain(mod+mVal,mod,tmp); buffer[1][i] = mod; }
    whichBuff = 0; buffEmpty[0] = 0; buffEmpty[1] = 0; buffCount = 0;

    noInterrupts();
    *ICRn[tt] = resolution;
    *TCCRnA[tt] = _BV(WGM11) | _BV(COM1A1) | _BV(COM1B0) | _BV(COM1B1); //WGM11,12,13 all set to 1 = fast PWM/w ICR TOP
    *TCCRnB[tt] = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
    *TIMSK[tt] = ( _BV(ICIE1) | _BV(TOIE1) );
    interrupts();


  }else{Serial.println("Read fail"); }

}



void TMRpcm::pause(){
	paused = !paused;
}


void TMRpcm::volume(int upDown){

  if(upDown){
	  volMod++; min(volMod,3);
  }else{
	  volMod--; max(volMod, -4);
  }

}

void TMRpcm::setVolume(int vol) {

    vol -= 4;
    min(vol,3);
    max(vol,-4);
    volMod = vol;

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
			bcnt[a] = sFile.read((byte*)buffer[a],buffSize);
		buffEmpty[a] = 0;
	  }

  if( playing && !paused){
		  *TIMSK[tt] |= ( _BV(ICIE1) | _BV(TOIE1) );
  }else
  if(paused && playing){*TIMSK[tt] &= ~( _BV(TOIE1) ); }

}


ISR(TIMER1_OVF_vect){

  if(qual == 1){ loadCounter = !loadCounter; if(loadCounter){return;}   }

  if(volMod < 0 ){
    *OCRnA[tt] = *OCRnB[tt] = buffer[whichBuff][buffCount] >> (volMod*-1);
  }else{
	*OCRnA[tt] = *OCRnB[tt] = buffer[whichBuff][buffCount] << volMod;
  }
  buffCount++;

  if(buffCount >= bcnt[whichBuff]){
	  if(sFile.available() <= buffSize){
	  	playing = 0;
	  	*TIMSK[tt] &= ~( _BV(ICIE1) | _BV(TOIE1) );
	  	if(sFile){sFile.close();}
	  }
      buffCount = 0;
      buffEmpty[whichBuff] = true;
      whichBuff = !whichBuff;
  }

}


void TMRpcm::stopPlayback(){
	playing = 0;
	*TIMSK[tt] &= ~( _BV(ICIE1) | _BV(TOIE1) );
	if(sFile){sFile.close();}

}

void TMRpcm::disable(){
  playing = 0;
  if(sFile){sFile.close();}
  *TIMSK[tt] &= ~( _BV(ICIE1) | _BV(TOIE1) );
  *TCCRnB[tt] &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12) );
  *TCCRnA[tt] = *TCCRnB[tt] = 0;

}


boolean TMRpcm::isPlaying(){
	return playing;
}

