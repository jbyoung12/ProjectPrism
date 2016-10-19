#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define CHANNEL 54
#define PIPE 0xF0F0F0F0F0
RF24 radio(9, 10);


const int LED1 = 9;
const int LED2 = 6;
const int LED3 = 5;

const int JSLUPDOWN = A0; //  max: 945, min: 12, mid: 499
const int JSLRL = A1; // max:964 , min:0 , mid: 491
const int JSRUPDOWN = A2; // max:975 , min:509 , mid: 491
const int JSRRL = A3; // max: , min: , mid: 490

void setup() {

  pinMode(JSLUPDOWN, INPUT);
  pinMode(JSLRL, INPUT);
  pinMode(JSRUPDOWN, INPUT);z
  pinMode(JSRRL, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  beginTransmit();
  Serial.begin(57000);
  Serial.println("Beggining transmissions!");
}

const int left = 97;
const int right = 68;
const int forward = 87;
const int backward = 98;
const int nuller = 0;
int val = nuller;

void loop() {

  if (analogRead(JSLUPDOWN) > 600){
    val = forward; 
  }
  if (analogRead(JSLUPDOWN) < 400){
    val = backward; 
  }
  else if (analogRead(JSRRL) > 600) {
    val = left; 
  }
  else if (analogRead(JSRRL) <400){
    val = right;  
  }
  else {
    val = nuller; 
   }
  
  byte value = (char) val;
  Serial.print("value: ");
  Serial.println(value);
  sendValue(value);
  Serial.println("Transmission successful");
  Serial.println();

}

void beginTransmit() {
  radio.begin();
  radio.setAutoAck(true);
  radio.setChannel(CHANNEL);
  radio.openWritingPipe(PIPE);
}

void beginListen() {
  radio.begin();
  radio.setAutoAck(true);
  radio.setChannel(CHANNEL);
  radio.openReadingPipe(1, PIPE);
  radio.startListening();
}

void sendValue(byte toSend) {
  boolean success = false;
  byte toWrite[1] = {toSend};
  while (!success) {
    success = radio.write(toWrite, 1);
  }
}

byte receiveValue() {
  byte readData[1];
  if (radio.available()) {
    radio.read(readData, 1);
    return readData[0];
  }
  return 0;
}

