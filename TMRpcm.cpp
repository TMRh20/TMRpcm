/*Library by TMRh20 2012
  Released into the public domain.*/




 #include <SD.h>

//#include <Arduino.h>
#include <TMRpcm.h>
//#include <pcmRF.h>



const int buffSize = 150; //note: there are 2 sound buffers. This will require (soundBuff*2) memory free

volatile byte buffer[2][buffSize+1];
volatile boolean buffEmpty[2] = {false,false};
volatile boolean whichBuff = false;
volatile int buffCount = 0;
volatile int volModMax = 1;
volatile int loadCounter = 0;
boolean paused = 0;
boolean playing = 0;
int volMod = 3;

File sFile;


TMRpcm::TMRpcm(){
	speakerPin = 11;
	SAMPLE_RATE = 16000;
	pwmMode = 1;
}


void TMRpcm::play(char* filename){

  pinMode(speakerPin, OUTPUT);
  stopPlayback();
  Serial.print("Playing: ");Serial.println(filename);

  if(!wavInfo(filename) ){ return; }//verify its a valid wav file
  if(sFile){sFile.close();}
  sFile = SD.open(filename);

  if(sFile){

//    wavInfo(filename); //verify its a valid wav file

    sFile.seek(44); //skip the header info
    for(int i=0; i<buffSize; i++){ buffer[0][i] = sFile.read(); }
    whichBuff = false; buffEmpty[0] = false; buffEmpty[1] = true;

    startPlayback();

    //Serial.println(cycles);
  }else{Serial.println("failed to open music file"); }

}



void TMRpcm::pause(){

	paused = !paused;
}


void TMRpcm::volume(int upDown){
  switch(upDown){
    case 1: volMod++; volMod = constrain(volMod, 1, volModMax); break;
    case 0: volMod--; volMod = constrain(volMod, 1, volModMax); break;
  }
}



boolean TMRpcm::wavInfo(char* filename){

  //check for the string WAVE starting at 8th bit in header file
  File xFile = SD.open(filename);
  if(!xFile){return 0;}
  xFile.seek(8); String wavStr = "";
    for(int i=0; i<4; i++){ wavStr += char(xFile.read()); }
    if(!wavStr.equalsIgnoreCase("WAVE") ){
      Serial.println("Not a WAV file");
      Serial.println(wavStr);
      xFile.close(); return 0;
    }

    //make sure the Sample rate is below 22000
    xFile.seek(24);
    unsigned int dVar = xFile.read();
    dVar = xFile.read() << 8 | dVar; //read 8bit values into 16-bit integer
    if(dVar > 22000){
      Serial.print(" SampleRate Too High: ");
      Serial.println(dVar); SAMPLE_RATE = 22000;
      Serial.println("Setting SR to 22000");
    }else{
    SAMPLE_RATE = dVar; // Set the sample rate according to the file
    }
    //Serial.print("Sample Rate: "); Serial.println(dVar);

    //verify that Bits Per Sample is 8 (0-255)
    xFile.seek(34); dVar = xFile.read();
    dVar = xFile.read() << 8 | dVar;
    if(dVar != 8){Serial.print("Incorrect Bits Per Sample: "); Serial.println(dVar); xFile.close(); return 0;}
    //Serial.print(" Bits Per Sample: "); Serial.println(dVar);
   xFile.close(); return 1;
}


void buffSD(){


}

ISR(TIMER1_CAPT_vect){

  // The first step is to disable this interrupt before manually enabling global interrupts.
  // This allows this interrupt vector (COMPB) to continue loading data while allowing the overflow interrupt
  // to interrupt it. ( Nested Interrupts )

   TIMSK1 &= ~_BV(ICIE1);

 //Now enable global interupts before this interrupt is finished, so the music can interrupt the buffering
  sei();

  if(sFile.available() < buffSize){
  	playing = 0;
    if(sFile){sFile.close();}
  	  TIMSK1 &= ~( _BV(ICIE1) | _BV(TOIE1) );
  	  OCR1A = 10;
  }else

  if(buffEmpty[0]){
  	for(int i=0; i<buffSize; i++){ buffer[0][i] = sFile.read(); }
    buffEmpty[0] = 0;
  }else
  if(buffEmpty[1]){
  	for(int i=0; i<buffSize; i++){ buffer[1][i] = sFile.read();  }
    buffEmpty[1] = 0;
  }

  if(paused){TIMSK1 = _BV(ICIE1); OCR1A = 10; TIMSK1 &= ~_BV(TOIE1); } //if pausedd, disable overflow vector and leave this one enabled
  else
  if(playing){
  //re-enable this interrupt vector and the overflow vector
  TIMSK1 = ( _BV(ICIE1) | _BV(TOIE1) );
  }

}



ISR(TIMER1_OVF_vect){

  ++loadCounter;
  if(loadCounter == 2){ loadCounter = 0; OCR1A = buffer[whichBuff][buffCount] * volMod; buffCount++; }

  if(buffCount >= buffSize){
      buffCount = 0;
      buffEmpty[whichBuff] = true;
      whichBuff = !whichBuff;
  }
}


void TMRpcm::startPlayback(){

   playing = 1;
   unsigned int modeMultiplier = 0;
   if(pwmMode){modeMultiplier = 8;}else{modeMultiplier = 4;}

   unsigned int resolution = modeMultiplier * (1000000/SAMPLE_RATE); //Serial.println(resolution);
   volModMax = (resolution ) / 248 ;
   volMod = constrain(volModMax-1,1,20);

   noInterrupts();
   ICR1 = resolution;
   OCR1A = 1;

   if(pwmMode){
     TCCR1A = _BV(WGM11) | _BV(COM1A1); //WGM11,12,13 all set to 1 = fast PWM/w ICR TOP
     TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
   }else{
     TCCR1A = _BV(COM1A1);
     TCCR1B = _BV(WGM13) | _BV(CS10);
   }
   TIMSK1 = ( _BV(ICIE1) | _BV(TOIE1) );
   interrupts();

   }




void TMRpcm::stopPlayback(){
  playing = 0;
  if(sFile){sFile.close();}
  TIMSK1 &= ~( _BV(ICIE1) | _BV(TOIE1) );
  OCR1A = 10;

}

void TMRpcm::disable(){
  playing = 0;
  if(sFile){sFile.close();}
  TIMSK1 &= ~( _BV(ICIE1) | _BV(TOIE1) );
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));

}


boolean TMRpcm::isPlaying(){
	return playing;
}

