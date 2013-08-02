This is an addon receiver library to enable PCM/WAV streaming using NRF24L01+ radio modules.
This code is proof of concept only.

Requirements: 
1 Arduino Mega
1 or more Arduino (Uno,Mega,Nano,etc)
2 or more NRF24L01+ Radio Modules + RF24 Arduino Library
1 SD card for controller

Step 1: Connect everything and test it all individually

To enable receiver:

1. Copy the 'pcmRX' folder to your Arduino library folder.
2. Open and run the RX example under the examples folder

To enable controller:
1. Open TMRpcm.h and uncomment the #define ENABLE_RF line
2. Open up and run one of the two example files in streamingExamples folder

Note: The ENABLE_RF line MUST be commented if not using RF features

The transmitter/streamer was tested using an Arduino Mega, and uses interrupt 5 (Pin 18) connected to the radio module interrupt pin.