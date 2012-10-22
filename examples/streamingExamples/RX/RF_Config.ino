


void rfCfg(){
 
  radio.begin();
  
  radio.enableDynamicPayloads();
  radio.enableAckPayload();
  radio.setChannel(53);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS); //or RF24_1MBPS
  radio.setCRCLength(RF24_CRC_8);
  radio.setRetries(5,13);
  radio.stopListening();
  radio.openWritingPipe(pipe); //com9
  radio.startListening();  
  radio.printDetails(); 

  
}
