

void setRadio(){
   radio.begin();

  radio.enableDynamicPayloads();
  radio.enableAckPayload();
  radio.setChannel(53);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS); //or RF24_1MBPS
  radio.setCRCLength(RF24_CRC_8);
  radio.setRetries(4,15);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening(); //NEED to start listening after opening a reading pipe  
  radio.printDetails();

}
