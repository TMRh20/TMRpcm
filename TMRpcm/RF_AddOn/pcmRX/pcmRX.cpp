

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

#include <pcmRX.h>

unsigned int SAMPLE_RATE = 20000;
unsigned int lstSampleRate = 1;
const int payloadSize = 32;
const int buffSize = 384; // 768max
//int volMod = 1;

//volatile int volModMax = 1;
volatile int intCount = 0;
volatile boolean buffEmpty[3] = {false,false};
volatile boolean whichBuff = 0;
byte buffer[2][buffSize+1];
unsigned int intData = 0;
unsigned int tt = 0;
boolean playing = 0;

int firstTime = 1;

byte tmpArr[33];
volatile boolean loadCount = 0;


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





RF24 radi(0,0);


pcmRX::pcmRX(RF24 radio, int cs, int spkPin){

	csPin = cs;
	radi = radio;
	pinMode(spkPin,OUTPUT);
	switch(spkPin){
		case 5: tt=1; break; //use TIMER3
		case 6: tt=2; break;//use TIMER4
		case 46:tt=3; break;//use TIMER5
		default:tt=0; break; //useTIMER1 as default
	}

}



boolean txSelX(byte sel, int timeoutDelay){
  radi.write(&sel,1);
  if(radi.isAckPayloadAvailable() == 1){ radi.read(&tmpArr,sizeof(tmpArr)); return(1);}
  else{ if(timeoutDelay > 0){ delay(timeoutDelay);}  return(0); }
}


boolean pcmRX::txSel(byte sel, int timeoutDelay){
  radi.write(&sel,sizeof(byte));
  int result = radi.getDynamicPayloadSize();
  if( result == 2 ){  return(1);}
  else{
	  Serial.println(result);
      if(result > 32){

		  SPI.setBitOrder(MSBFIRST);
		  SPI.setDataMode(SPI_MODE0);
		  SPI.setClockDivider(SPI_CLOCK_DIV4);
		  digitalWrite(csPin,LOW);
		  SPI.transfer( FLUSH_RX );
		  digitalWrite(csPin,HIGH);

      }
      if(timeoutDelay > 0){ delay(timeoutDelay);}  return(0);
  }
}

void noTun(){
  playing = 0;
  //*OCRnA[tt] = 1;
  *TIMSK[tt] &= ~( _BV(ICIE1) | _BV(TOIE1) );
  //TIMSK1 = _BV(OCIE1A); //ICR1 = 3000;
  //TCCR1B &= ~(_BV(WGM13) | _BV(CS10) | _BV(CS11) | _BV(CS12));


}

void retryClose(){
  noInterrupts();
  while(!txSelX(6,200)){}
  interrupts();
}


void loadSingleBuffer(boolean wBf){
  int bCount = 0; int fails = 0;
  //if(!playing){return;}

  while(bCount < buffSize){

    if(txSelX(3,0)){
        for(int i=0; i<payloadSize; i++){
          buffer[wBf][bCount] = tmpArr[i]; bCount++;
            //if(bCount > buffSize){ Serial.println("BuffSize needs to be a multiple of 32"); noTun(); return;}
        }
          //radio.powerUp();
    }else{
          if(tmpArr[0] == 4 &&  tmpArr[1] == 3 && tmpArr[2] == 2){
          noTun(); retryClose();
          Serial.println("ebp");
           return;
         }

       if(fails >= 300){Serial.println("fail");noTun(); retryClose(); return;}
          fails++;
    }

    radi.powerUp();
  }
  buffEmpty[wBf] = 0; radi.powerUp();
  return;
}




void pcmRX::startTimer(){
  playing = 1;
  Serial.println("strtd");
  if(SAMPLE_RATE > 22050 ){ SAMPLE_RATE = 18000; Serial.print("SampleRate Too High");}
   unsigned int resolution = 8 * (1000000/SAMPLE_RATE);
   noInterrupts();
       *ICRn[tt] = resolution;
       *TCCRnA[tt] = _BV(WGM11) | _BV(COM1A1) | _BV(COM1B0) | _BV(COM1B1); //WGM11,12,13 all set to 1 = fast PWM/w ICR TOP
       *TCCRnB[tt] = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
       *TIMSK[tt] = ( _BV(ICIE1) | _BV(TOIE1) );
    interrupts();
}

void pcmRX::resumeTimer(){

  playing = 1;
  unsigned int resolution = 8 * (1000000/SAMPLE_RATE);
  *ICRn[tt] = resolution;
  *TIMSK[tt] = ( _BV(ICIE1) | _BV(TOIE1) );
}


ISR(TIMER1_CAPT_vect){

  *TIMSK[tt] &= ~_BV(ICIE1);
  sei();

  if(buffEmpty[0]){
    loadSingleBuffer(0);
  }else
  if(buffEmpty[1] ){
   loadSingleBuffer(1);
  }

  if(playing){  *TIMSK[tt] = (_BV(ICIE1) | _BV(TOIE1) );  }
}



ISR(TIMER1_OVF_vect){


  if(buffEmpty[0] && buffEmpty[1]){  }else{

  loadCount = !loadCount;
  if(loadCount == 0){ return; }

  *OCRnA[tt] = *OCRnB[tt] = buffer[whichBuff][intCount];
  intCount++;

  if(intCount >= buffSize){
      intCount = 0;
      buffEmpty[whichBuff] = true;
      whichBuff = !whichBuff;
  }
  }

}



void pcmRX::pollForMusic(){

  if(!playing){
      int steps = 1;
      //testDataRates2();
    if(steps == 1){
            radi.powerUp();  // << VERY important step for some reason
            delay(5);
            if( txSelX(4,5) ){
              if( tmpArr[0] == 1 && tmpArr[1] == 2 && tmpArr[2] == 3 ){
                steps = 2;  //togLed();//txSelX(5,5); //txSelX(5,50);
              }else{txSelX(5,0); }
            }
    }
    if(steps == 2){
             Serial.println("S2");
             if ( txSel(3,5)){
              radi.read(&intData,sizeof(intData));
              if(intData < 8000 || intData > 22100){Serial.print("st2 "); /*delay(200);Serial.println(intData);*/ return;}
              //Serial.println("strt");
              whichBuff = 0;
              loadSingleBuffer(0); loadSingleBuffer(1);
              buffEmpty[0] = 0; buffEmpty[1] = 0; intCount = 0;
              SAMPLE_RATE = intData;
              if(intData > 20000){SAMPLE_RATE = 20000;}
              //Serial.println("playing");

//              if(timerTracker){  timerTracker=0; startTimer();
//              }else{resumeTimer();}
              if(!playing){ startTimer();
              }else{resumeTimer();}

            }else{ Serial.println("pl2");}
}

  }

}

