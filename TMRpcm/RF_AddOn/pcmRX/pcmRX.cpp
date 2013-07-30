





#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

#include <pcmRX.h>

unsigned int SAMPLE_RATE = 20000;
unsigned int lstSampleRate = 1;
const int payloadSize = 32;
const int buffSize = 384; // 768max
int volMod = 1;

volatile int volModMax = 1;
volatile int intCount = 0;
volatile boolean buffEmpty[3] = {false,false};
volatile boolean whichBuff = 0;
byte buffer[2][buffSize+1];
unsigned int intData = 0;
boolean playing = 0;

int firstTime = 1;

byte tmpArr[33];

volatile boolean srX2 = false;
volatile boolean srX15 = false;
volatile boolean r12Toggle = false;
volatile int loadCounter = 0;

RF24 radi(0,0);


pcmRX::pcmRX(RF24 radio, int cs){

	csPin = cs;
	radi = radio;

}



boolean txSelX(byte sel, int timeoutDelay){
  radi.write(&sel,1);
  if(radi.isAckPayloadAvailable() == 1){ radi.read(&tmpArr,sizeof(tmpArr)); return(1);}
  else{ if(timeoutDelay > 0){ delay(timeoutDelay);}  return(0); }
}


boolean pcmRX::txSel(byte sel, int timeoutDelay){
  radi.write(&sel,sizeof(byte));
  int result = radi.getDynamicPayloadSize();
  if( result == 2 ){ return(1);}
  else{
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
  OCR1A = 1;
  TIMSK1 &= ~( _BV(ICIE1) | _BV(TOIE1) );
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
  //TCCR1A = 0; TCCR1B = 0; // Set the timer registers back to 0

  if(SAMPLE_RATE <= 10000){SAMPLE_RATE = SAMPLE_RATE*2; srX2 = true;}else{srX2 = false;}
  if(SAMPLE_RATE > 10000 && SAMPLE_RATE < 13250){SAMPLE_RATE = SAMPLE_RATE*1.5; srX15 = true;}else{srX15 = false;}

//  unsigned int resolution = (8 * (1000000/SAMPLE_RATE)); //FastPWM mode only counts up, so we get higher resolution
   unsigned int modeMultiplier = 0;
   boolean pMode = 1;
   if(pMode){modeMultiplier = 8;}else{modeMultiplier = 4;}

   //int test = 4 * (1000000/SAMPLE_RATE);
   unsigned int resolution = modeMultiplier * (1000000/SAMPLE_RATE); //Serial.println(resolution);
   volModMax = (resolution * 1.5) / 248 ; //no more than 75% PWM duty cycle
   if(volMod > volModMax){ volMod = volModMax; }
   //volMod = volModMax-1;
   volMod = 1;
   noInterrupts();
   ICR1 = resolution;
   OCR1A = 1;

   if(pMode){
     TCCR1A = _BV(WGM11) | _BV(COM1A1); //WGM11,12,13 all set to 1 = fast PWM/w ICR TOP
     TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
   }else{
     TCCR1A = _BV(COM1A1);
     TCCR1B = _BV(WGM13) | _BV(CS10);
   }
   //TIMSK1 &= ~_BV(OCIE1A);
   TIMSK1 = ( _BV(ICIE1) | _BV(TOIE1) );
   interrupts();
}

void pcmRX::resumeTimer(){

  playing = 1;
//  noInterrupts();
//  //TCCR1A = 0; TCCR1B = 0; // Set the timer registers back to 0
//
  if(SAMPLE_RATE != lstSampleRate){
    lstSampleRate = SAMPLE_RATE;
    if(SAMPLE_RATE <= 10000){SAMPLE_RATE = SAMPLE_RATE*2; srX2 = true;}else{srX2 = false;}
    if(SAMPLE_RATE > 10000 && SAMPLE_RATE < 13500){SAMPLE_RATE = SAMPLE_RATE*1.5; srX15 = true;}else{srX15 = false;}

   unsigned int modeMultiplier = 0;
   boolean pMode = 1;
   if(pMode){modeMultiplier = 8;}else{modeMultiplier = 4;}

   //int test = 4 * (1000000/SAMPLE_RATE);
   unsigned int resolution = modeMultiplier * (1000000/SAMPLE_RATE);

  volModMax = (resolution * 1.5) / 248 ; //no more than 75% PWM duty cycle
  if(volMod > volModMax){ volMod = volModMax; }
  //volMod = volModMax-1;
    //ICR1 = resolution;
//  TIMSK1 = ( _BV(OCIE1B) | _BV(TOIE1) );
//  interrupts();
  }
  //TIMSK1 &= ~_BV(OCIE1A);
  TIMSK1 = ( _BV(ICIE1) | _BV(TOIE1) );

}


ISR(TIMER1_CAPT_vect){

  TIMSK1 &= ~_BV(ICIE1);
  sei();

  if(buffEmpty[0]){
    loadSingleBuffer(0);
  }else
  if(buffEmpty[1] ){
   loadSingleBuffer(1);
  }

  //TIMSK1 = ( _BV(OCIE1B) | _BV(TOIE1) );
  //if(!stopped){  TIMSK1 = (_BV(OCIE1B) | _BV(TOIE1) );  }
  if(playing){  TIMSK1 = (_BV(ICIE1) | _BV(TOIE1) );  }
}

void buffE(boolean wh){


}


ISR(TIMER1_OVF_vect){

  //if(buffEmpty[0] == 0 || buffEmpty[1] == 0){
  if(buffEmpty[0] && buffEmpty[1]){  }else{

    ++loadCounter;

    if(srX2){ if(loadCounter == 4){ loadCounter = 0; OCR1A = buffer[whichBuff][intCount] * volMod; intCount++; }
    }else
    if(srX15){ int tmp;
      if(r12Toggle){tmp=4;}else{tmp=2;}
        if(loadCounter == tmp){
          loadCounter = 0;
          r12Toggle = !r12Toggle;
          OCR1A = buffer[whichBuff][intCount] * volMod; intCount++;
        }
    }else{ if(loadCounter == 2){ loadCounter = 0; OCR1A = buffer[whichBuff][intCount] * volMod; intCount++; }} //normal operation

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

