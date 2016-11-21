#include <NewPing.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
#define SLAVE_ADDRESS 0x05

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const int ScannerServoFrontLeft =   7 ;
const int ScannerServoFrontRight =  3; 
const int ScannerServoBackLeft =    11; 
const int ScannerServoBackRight =   15;

int distances[360]; 

#define TRIGGER_PIN_FRONT_RIGHT  2
#define ECHO_PIN_FRONT_RIGHT    3

#define TRIGGER_PIN_FRONT_LEFT  11
#define ECHO_PIN_FRONT_LEFT     10

#define TRIGGER_PIN_BACK_RIGHT  8
#define ECHO_PIN_BACK_RIGHT     9

#define TRIGGER_PIN_BACK_LEFT   7
#define ECHO_PIN_BACK_LEFT      6

#define MAX_DISTANCE 200

NewPing FrontRight( TRIGGER_PIN_FRONT_RIGHT,  ECHO_PIN_FRONT_RIGHT, MAX_DISTANCE);
NewPing FrontLeft(  TRIGGER_PIN_FRONT_LEFT,   ECHO_PIN_FRONT_LEFT,  MAX_DISTANCE);
NewPing BackRight(  TRIGGER_PIN_BACK_RIGHT,   ECHO_PIN_BACK_RIGHT,  MAX_DISTANCE);
NewPing BackLeft(   TRIGGER_PIN_BACK_LEFT,    ECHO_PIN_BACK_LEFT,   MAX_DISTANCE);

void setup() {
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  Serial.begin(9600);
}


// ScannerServoFrontRight is fucked up

void loop() {

      for (int i = 0; i < 180; i ++ ){
        angle(ScannerServoFrontLeft,i);
        angle(ScannerServoFrontRight,i);
        angle(ScannerServoBackLeft,i);
         angle(ScannerServoBackRight,i);
        delay(15);
    }

      for (int i = 180; i > 0; i -- ){
        angle(ScannerServoFrontLeft,i);
        angle(ScannerServoFrontRight,i);
        angle(ScannerServoBackLeft,i);
         angle(ScannerServoBackRight,i);
        delay(15);
    }

  /*
  
    for (int i = 0; i < 180; i ++ ){
        angle(ScannerServoFrontLeft,i);
        Serial.println(FrontLeft.ping_cm());
        delay(15);
    }
    angle(ScannerServoFrontLeft,90);

    delay(2000);

    for (int i = 0; i < 180; i ++ ){
        angle(ScannerServoFrontRight,i);
        Serial.println(FrontRight.ping_cm());
        delay(15);
    }
    
    angle(ScannerServoFrontRight,90);
    delay(2000);


    for (int i = 0; i < 180; i ++ ){
      angle(ScannerServoBackLeft,i);
      Serial.println(BackLeft.ping_cm());
      delay(15);
    }
    angle(ScannerServoBackLeft,90);
    delay(2000);

    for (int i = 0; i < 180; i ++ ){
      angle(ScannerServoBackRight,i);
      Serial.println(BackRight.ping_cm());
      delay(15);
    }
    angle(ScannerServoBackRight,90);
    delay(2000);
*/
}


int limiter(int input) {

  if (input > 180) {
    return 180;
  }
  else if (input < 0) {
    return 0;
  }
  else {
    return input;
  }

}

//leg number, angle
void angle(int OutputLine, int Angle) {
  pwm.setPWM(OutputLine, 0, returnMappedValue(Angle)); // center front right MIDDLE
}

int returnMappedValue(int value) {
  value = map(value, 0, 180, SERVOMIN, SERVOMAX);
  return value;
}




