#include <Wire.h>
#include <Servo.h>
#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;

double temp;
Servo horizontal;
Servo vertical;

void sendData();
void receiveData(int byteCount);
void receiveArrayData(int numByte);
double GetTemp(void);

void setup() {
 pinMode(13, OUTPUT);
 Serial.begin(9600);
 pinMode(9,OUTPUT);
 pinMode(10,OUTPUT);
 Serial.println("Beggining communications");
 //horizontal.attach(9);
 //vertical.attach(10);

 Wire.begin(SLAVE_ADDRESS);

 // define callbacks for i2c communication
 //Wire.onReceive(receiveData);
 Wire.onReceive(receiveArrayData);
 Wire.onRequest(sendData);
}

void loop() {
 delay(100);
 temp = GetTemp();
}

// callback for received data
void receiveData(int byteCount){
 Serial.println("in recieveData");
 while(Wire.available()) {
  number = Wire.read();
  Serial.println(number);
  if (number == 1 ){
   if (state == 0){
    digitalWrite(13, HIGH); // set the LED on
    state = 1;
   } else{
    digitalWrite(13, LOW); // set the LED off
    state = 0;
   }
  }

  if(number==2) {
   number = (int)temp;
  }
 }
}


int count = 0;
const int arrayLen = 3;
int intArray[arrayLen];
void receiveArrayData(int numByte){

  int currentInt = Wire.read();
  intArray[count] = currentInt;
  count++;

  if (count == arrayLen){
    count = 0;
  }

  if (intArray[1] < 67){
    intArray[1] = 67;
  }

  analogWrite(10,intArray[1]);
  analogWrite(9,intArray[2]);
  if (intArray[0] ==1){
    digitalWrite(13,HIGH);
  }
  if (intArray[0]==0){
    digitalWrite(13,LOW);
  }
}


// callback for sending data
void sendData(){
 Wire.write(number);
}

// Get the internal temperature of the arduino
double GetTemp(void){
 unsigned int wADC;
 double t;
 ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
 ADCSRA |= _BV(ADEN); // enable the ADC
 delay(20); // wait for voltages to become stable.
 ADCSRA |= _BV(ADSC); // Start the ADC
 while (bit_is_set(ADCSRA,ADSC));
 wADC = ADCW;
 t = (wADC - 324.31 ) / 1.22;
 return (t);
}
