#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#include <Wire.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
#define NUMFLAKES 10
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
#define CHANNEL 54
#define PIPE 0xF0F0F0F0F0

RF24 radio(9,10);

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Adafruit_SSD1306 display(OLED_RESET);

const int bodyFrontLeft = 4;  const int middleFrontLeft = 5;  const int legFrontLeft = 6 ;
const int bodyFrontRight = 0; const int middleFrontRight = 1; const int legFrontRight = 2;
const int bodyBackRight = 12; const int middleBackRight = 13; const int legBackRight = 14;
const int bodyBackLeft = 8;   const int middleBackLeft = 9;   const int legBackLeft = 10;

void foward(int inc);
void setAllStraight();
void turnLeft(int inc);

int bodyOffset = 25;
int middleOffset = 15;
int legOffSet = 15;
//----------------------------------------------------//
int bodyBackRightCenterValue = 95 + bodyOffset;         int bodyBackRightCurrentValue =  bodyBackRightCenterValue;      // back right middle
int middleBackRightCenterValue = 165 - middleOffset;    int middleBackRightCurrentValue =  middleBackRightCenterValue; // back right middle
int legBackRightCenterValue = 80 - legOffSet;                        int legBackRightCurrentValue  = legBackRightCenterValue;       // back right leg
//----------------------------------------------------//

//----------------------------------------------------//
int bodyFrontRightCenterValue = 90 - bodyOffset - 20;   int bodyFrontRightCurrentValue = bodyFrontRightCenterValue;     // front right middle
int middleFrontRightCenterValue = 180 - middleOffset;   int middleFrontRightCurrentValue = middleFrontRightCenterValue; // front right middle
int legFrontRightCenterValue = 40 - legOffSet;                       int legFrontRightCurrentValue = legFrontRightCenterValue ;      // front right leg
//----------------------------------------------------//

//----------------------------------------------------//
int bodyFrontLeftCenterValue = 90 + bodyOffset;         int bodyFrontLeftCurrentValue = bodyFrontLeftCenterValue;       // front left body
int middleFrontLeftCenterValue = 180 - middleOffset;    int middleFrontLeftCurrentValue = middleFrontLeftCenterValue; // front left middle
int legFrontLeftCenterValue = 80 - legOffSet;                       int legFrontLeftCurrentValue = legFrontLeftCenterValue;         // front left leg
//----------------------------------------------------//

//----------------------------------------------------//
int bodyBackLeftCenterValue = 90 - bodyOffset;          int bodyBackLeftCurrentValue = bodyBackLeftCenterValue;         //back left middle
int middleBackLeftCenterValue = 180 - middleOffset;     int middleBackLeftCurrentValue = middleBackLeftCenterValue;   //back left middle
int legBackLeftCenterValue = 40 - legOffSet;                         int legBackLeftCurrentValue = legBackLeftCenterValue;           // back left leg
//----------------------------------------------------//



void setup() {

  beginListen();
  Serial.begin(57600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.print("Hello, world!");
  display.display();

  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  pinMode(8, OUTPUT);

  delay(2000);
  setAllStraight();

  digitalWrite(8,HIGH);
  delay(100);
  digitalWrite(8,LOW);
  delay(100);
  digitalWrite(8,HIGH);
  delay(100);
  digitalWrite(8,LOW);
  delay(100);
  digitalWrite(8,HIGH);
  delay(100);
  digitalWrite(8,LOW);
  delay(100);
  digitalWrite(8,HIGH);
  delay(100);
  digitalWrite(8,LOW);
}


const int turnRightMaxInc = 9;
int turnRightInc = 1;

const int turnLeftMaxInc = 9;
int turnLeftInc = 1;

const int stepMaxInc = 9;
int stepInc = 1;

const int turnBackwardIncMax = 9;
int turnBackwardInc = 1;

const int forwardChar = 87;
const int backwardChar = 98;
const int leftChar = 97;
const int rightChar = 68;



void loop() {
  
  mamba();

  
  delay(1000);
  setAllStraight();
  delay(1000);
  
  
  /*
  byte value = (char) receiveValue();
  if(value!=0) {
    if (value == leftChar){
      if (turnLeftInc != turnLeftMaxInc+1){  
        turnLeft(turnLeftInc);
        turnLeftInc ++;
      }
      else{
        turnLeftInc = 1;  
      }
    }

    if (value == forwardChar){
      if (stepInc != stepMaxInc+1){  
        forward(stepInc);
        stepInc ++;
      }
      else{
        stepInc = 1;  
      }
    }
    
    if (value == rightChar){
      if (turnRightInc != turnRightMaxInc+1){  
        turnRight(turnRightInc);
        turnRightInc ++;
      }
      else{
        turnRightInc = 1;  
      }
    }

    if (value == backwardChar){
      if (turnBackwardInc != turnBackwardIncMax+1){  
        turnRight(turnBackwardInc);
        turnBackwardInc ++;
      }
      else{
        turnBackwardInc = 1;  
      }
    }
    
    Serial.print("Got value: ");
    Serial.println(value);
  }
*/

}



int middleFinalStepOffset = 37;
int middleMidStepOffset = 30;
int legMidStepOffset = 40;
int bodyFinalOffset = 45;
int delayTime = 10;


void Yp( int t ){

  double middleTheta = middleFinalStepOffset * sin( 3.141592 * t );
  angle(middleBackRight, middleBackRightCenterValue + (int) middleTheta );
 
}







void turnLeft(int inc) {
  switch (inc) {
    case 1:
      for (int i = 0; i < middleMidStepOffset + 5; i++) {
        angle(middleBackRight, middleBackRightCenterValue + i);
        delay(delayTime);
      }
      for (int i = 0; i < legMidStepOffset; i++) {
        angle(legBackRight, legBackRightCenterValue + i);
        delay(delayTime);
      }
      for (int i = 0; i < bodyFinalOffset; i++) {
        angle(bodyBackRight, bodyBackRightCenterValue + i);
        delay(delayTime);
      }
      break;

    case 2:
      for (int i = middleMidStepOffset + 5; i > 0 ; i--) {
        angle(middleBackRight,  middleBackRightCenterValue + middleMidStepOffset + 5 - (middleMidStepOffset + 5 - i) );
        delay(delayTime);
      }
      for (int i = legMidStepOffset; i > 0; i--) {
        angle(legBackRight, limiter(legBackRightCenterValue + legMidStepOffset - (legMidStepOffset - i )));
        delay(delayTime);
      }
      break;

    case 3:
      for (int i = 0; i < middleMidStepOffset + 5; i++) {
        angle( middleFrontRight,  middleFrontRightCenterValue + i );
        delay(delayTime);
      }
      for (int i = 0; i < legMidStepOffset; i++) {
        angle( legFrontRight,     legFrontRightCenterValue + i );
        delay(delayTime);
      }
      for (int i = 0; i < bodyFinalOffset; i++) {
        angle( bodyFrontRight,    bodyFrontRightCenterValue + i );
        delay(delayTime);
      }
      break;

    case 4:
      for (int i = middleMidStepOffset + 5; i > 0 ; i--) {
        angle( middleFrontRight,  middleFrontRightCenterValue + middleMidStepOffset + 5 - (middleMidStepOffset + 5 - i) );
        delay(delayTime);
      }
      for (int i = legMidStepOffset; i > 0; i--) {
        angle( legFrontRight,     limiter(legFrontRightCenterValue + legMidStepOffset - (legMidStepOffset - i )));
        delay(delayTime);
      }
      break;

    case 5:
      for (int i = 0; i < middleMidStepOffset + 5; i++) {
        angle(middleFrontLeft, middleFrontLeftCenterValue + i);
        delay(delayTime);
      }
      for (int i = 0; i < legMidStepOffset; i++) {
        angle(legFrontLeft, legFrontLeftCenterValue + i);
        delay(delayTime);
      }
      for (int i = 0; i < bodyFinalOffset; i++) {
        angle(bodyFrontLeft, bodyFrontLeftCenterValue + i);
        delay(delayTime);
      }
      break;

    case 6:
      for (int i = middleMidStepOffset + 5; i > 0 ; i--) {
        angle(middleFrontLeft,  middleFrontLeftCenterValue + middleMidStepOffset + 5 - (middleMidStepOffset + 5 - i) );
        delay(delayTime);
      }
      for (int i = legMidStepOffset; i > 0; i--) {
        angle(legFrontLeft, limiter(legFrontLeftCenterValue + legMidStepOffset - (legMidStepOffset - i )));
        delay(delayTime);
      }
      break;

    case 7:
      for (int i = 0; i < middleMidStepOffset + 5; i++) {
        angle( middleBackLeft,  middleBackLeftCenterValue + i );
        delay(delayTime);
      }
      for (int i = 0; i < legMidStepOffset; i++) {
        angle( legBackLeft,     legBackLeftCenterValue + i );
        delay(delayTime);
      }
      for (int i = 0; i < bodyFinalOffset; i++) {
        angle( bodyBackLeft,    bodyBackLeftCenterValue + i );
        delay(delayTime);
      }
      break;

    case 8:
      for (int i = middleMidStepOffset + 5; i > 0 ; i--) {
        angle( middleBackLeft,  middleBackLeftCenterValue + middleMidStepOffset + 5 - (middleMidStepOffset + 5 - i) );
        delay(delayTime);
      }
      for (int i = legMidStepOffset; i > 0; i--) {
        angle( legBackLeft,     limiter( legBackLeftCenterValue + legMidStepOffset - (legMidStepOffset - i )));
        delay(delayTime);
      }
      break;

    default:
      setAllStraight();
      break;
  }

}


void turnRight(int inc) {

  switch (inc) {
    case 1:
      for (int i = 0; i < middleMidStepOffset + 5; i++) {
        angle(middleBackRight, middleBackRightCenterValue + i);
        delay(delayTime);
      }
      for (int i = 0; i < legMidStepOffset; i++) {
        angle(legBackRight, legBackRightCenterValue + i);
        delay(delayTime);
      }
      for (int i = 0; i > -bodyFinalOffset; i--) {
        angle(bodyBackRight, bodyBackRightCenterValue + i);
        delay(delayTime);
      }
      break;

    case 2:
      for (int i = middleMidStepOffset + 5; i > 0 ; i--) {
        angle(middleBackRight,  middleBackRightCenterValue + middleMidStepOffset + 5 - (middleMidStepOffset + 5 - i) );
        delay(delayTime);
      }
      for (int i = legMidStepOffset; i > 0; i--) {
        angle(legBackRight, limiter(legBackRightCenterValue + legMidStepOffset - (legMidStepOffset - i )));
        delay(delayTime);
      }
      break;

    case 3:
      for (int i = 0; i < middleMidStepOffset + 5; i++) {
        angle( middleFrontRight,  middleFrontRightCenterValue + i );
        delay(delayTime);
      }
      for (int i = 0; i < legMidStepOffset; i++) {
        angle( legFrontRight,     legFrontRightCenterValue + i );
        delay(delayTime);
      }
      for (int i = 0; i > -bodyFinalOffset; i--) {
        angle( bodyFrontRight,    bodyFrontRightCenterValue + i );
        delay(delayTime);
      }
      break;

    case 4:
      for (int i = middleMidStepOffset + 5; i > 0 ; i--) {
        angle( middleFrontRight,  middleFrontRightCenterValue + middleMidStepOffset + 5 - (middleMidStepOffset + 5 - i) );
        delay(delayTime);
      }
      for (int i = legMidStepOffset; i > 0; i--) {
        angle( legFrontRight,     limiter(legFrontRightCenterValue + legMidStepOffset - (legMidStepOffset - i )));
        delay(delayTime);
      }
      break;

    case 5:
      for (int i = 0; i < middleMidStepOffset + 5; i++) {
        angle(middleFrontLeft, middleFrontLeftCenterValue + i);
        delay(delayTime);
      }
      for (int i = 0; i < legMidStepOffset; i++) {
        angle(legFrontLeft, legFrontLeftCenterValue + i);
        delay(delayTime);
      }
      for (int i = 0; i > -bodyFinalOffset; i--) {
        angle(bodyFrontLeft, bodyFrontLeftCenterValue + i);
        delay(delayTime);
      }
      break;

    case 6:
      for (int i = middleMidStepOffset + 5; i > 0 ; i--) {
        angle(middleFrontLeft,  middleFrontLeftCenterValue + middleMidStepOffset + 5 - (middleMidStepOffset + 5 - i) );
        delay(delayTime);
      }
      for (int i = legMidStepOffset; i > 0; i--) {
        angle(legFrontLeft, limiter(legFrontLeftCenterValue + legMidStepOffset - (legMidStepOffset - i )));
        delay(delayTime);
      }
      break;

    case 7:
      for (int i = 0; i < middleMidStepOffset + 5; i++) {
        angle( middleBackLeft,  middleBackLeftCenterValue + i );
        delay(delayTime);
      }
      for (int i = 0; i < legMidStepOffset; i++) {
        angle( legBackLeft,     legBackLeftCenterValue + i );
        delay(delayTime);
      }
      break;

    case 8:
      for (int i = 0; i > -bodyFinalOffset; i--) {
        angle( bodyBackLeft,    bodyBackLeftCenterValue + i );
        delay(delayTime);
      }
      for (int i = middleMidStepOffset + 5; i > 0 ; i--) {
        angle( middleBackLeft,  middleBackLeftCenterValue + middleMidStepOffset + 5 - (middleMidStepOffset + 5 - i) );
        delay(delayTime);
      }
      for (int i = legMidStepOffset; i > 0; i--) {
        angle( legBackLeft,     limiter( legBackLeftCenterValue + legMidStepOffset - (legMidStepOffset - i )));
        delay(delayTime);
      }
      break;

    default:
      setAllStraight();
      break;
  }

}





void backward(int inc) {

  switch (inc) {
    case 1:
      // BACK RIGHT
      for (int i = 0; i < middleMidStepOffset + 5; i++) {
      angle(middleBackRight, middleBackRightCenterValue + i);
      delay(delayTime);
      }
      for (int i = 0; i < legMidStepOffset; i++) {
      angle(legBackRight, legBackRightCenterValue + i);
      delay(delayTime);
      }
      for (int i = 0; i < bodyFinalOffset; i++) {
      angle(bodyBackRight, bodyBackRightCenterValue + i);
      delay(delayTime);
      }
      break;

    case 2:
      for (int i = middleMidStepOffset + 5; i > 0 ; i--) {
      angle(middleBackRight,  middleBackRightCenterValue + middleMidStepOffset + 5 - (middleMidStepOffset + 5 - i) );
      delay(delayTime);
      }
      for (int i = legMidStepOffset; i > 0; i--) {
      angle(legBackRight, limiter(legBackRightCenterValue + legMidStepOffset - (legMidStepOffset - i )));
      delay(delayTime);
      }
      break;

    case 3: 
        // FRONT RIGHT LEG EXTENDS
        for (int i = 0; i < middleMidStepOffset; i++) {
        angle(middleFrontRight, middleFrontRightCenterValue + i);
        delay(delayTime);
        }
        for (int i = 0; i < legMidStepOffset; i++) {
        angle(legFrontRight, legFrontRightCenterValue + i);
        delay(delayTime);
        }

        break;
      
      case 4:
        for (int i = 0; i < bodyFinalOffset; i++) {
        angle( bodyFrontRight , bodyFrontRightCenterValue + i );
        delay(delayTime);
        }
        for (int i = -middleMidStepOffset; i < middleMidStepOffset; i++) {
        angle(middleFrontRight, middleFrontRightCenterValue - i);
        delay(delayTime);
        }
        break;

    case 5:
      // BODY CONDENSES
      for (int i = middleMidStepOffset; i > 0 ; i--) {
      
      // Front right
      angle(middleFrontRight, limiter( middleFrontRightCenterValue - i ) );
      angle(legFrontRight, limiter( legFrontRightCenterValue + legMidStepOffset - (legMidStepOffset - map(i, 0, middleMidStepOffset, 0, legMidStepOffset))  ));
      angle(bodyFrontRight, limiter(bodyFrontRightCenterValue + bodyFinalOffset - (bodyFinalOffset - map(i, 0, middleMidStepOffset, 0, bodyFinalOffset )    )));
      
      // Back left
      angle(middleBackLeft, middleBackLeftCenterValue - (middleMidStepOffset - i));
      angle(legBackLeft, limiter( legBackLeftCenterValue + legMidStepOffset  - map(i, 0, middleMidStepOffset, 0, legMidStepOffset )));
      
      // back right
      angle(bodyBackRight, limiter(bodyBackRightCenterValue + bodyFinalOffset - (bodyFinalOffset - map(i, 0, middleMidStepOffset, 0, bodyFinalOffset))));
      
      // front right
      angle(bodyFrontLeft, limiter(bodyFrontLeftCenterValue + bodyFinalOffset - ( map(i, 0, middleMidStepOffset, 0, bodyFinalOffset))));
      
      delay(delayTime);
      }
      break;

    case 6:
      
      // BACK LEFT
      for (int i = 0; i < middleMidStepOffset + 5; i++) {
      angle(middleBackLeft, middleBackLeftCenterValue + i);
      delay(delayTime);
      }
      for (int i = 0; i < bodyFinalOffset; i++) {
      angle(bodyBackLeft, bodyBackLeftCenterValue - i);
      delay(delayTime);
      }
      break;

    case 7: 
      for (int i = legMidStepOffset; i > 0 ; i--) {
      angle(legBackLeft, limiter( legBackLeftCenterValue + legMidStepOffset - (legMidStepOffset - i) )  );
      delay(delayTime);
      }
      for (int i = middleMidStepOffset; i > 0 ; i--) {
      angle(middleBackLeft, limiter( middleBackLeftCenterValue - middleMidStepOffset + (middleMidStepOffset - i) )  );
      delay(delayTime);
      }
      break;

    case 8:
      // FRONT LEFT
      for (int i = 0; i < middleMidStepOffset + 5; i++) {
      angle( middleFrontLeft, middleFrontLeftCenterValue + i);
      delay(delayTime);
      }
      for (int i = 0; i < legMidStepOffset; i++) {
      angle(legFrontLeft, legFrontLeftCenterValue + i);
      delay(delayTime);
      }
      break;

    default:
      for (int i = 0; i < bodyFinalOffset; i++) {
      angle( bodyFrontLeft, bodyFrontLeftCenterValue - i );
      delay(delayTime);
      }
      for (int i = middleMidStepOffset + 5; i > 0 ; i--) {
      angle(middleFrontLeft,  middleFrontLeftCenterValue + middleMidStepOffset + 5 - (middleMidStepOffset + 5 - i) );
      delay(delayTime);
      }
      for (int i = legMidStepOffset; i > 0; i--) {
      angle(legFrontLeft, limiter(legFrontLeftCenterValue + legMidStepOffset - (legMidStepOffset - i )));
      delay(delayTime);
      }
      break;
  }
}














void forward(int inc) {

  switch (inc) {
    case 1:
      // BACK RIGHT
      for (int i = 0; i < middleMidStepOffset + 5; i++) {
      angle(middleBackRight, middleBackRightCenterValue + i);
      delay(delayTime);
      }
      for (int i = 0; i < legMidStepOffset; i++) {
      angle(legBackRight, legBackRightCenterValue + i);
      delay(delayTime);
      }
      for (int i = 0; i < bodyFinalOffset; i++) {
      angle(bodyBackRight, bodyBackRightCenterValue + i);
      delay(delayTime);
      }
      break;

    case 2:
      for (int i = middleMidStepOffset + 5; i > 0 ; i--) {
      angle(middleBackRight,  middleBackRightCenterValue + middleMidStepOffset + 5 - (middleMidStepOffset + 5 - i) );
      delay(delayTime);
      }
      for (int i = legMidStepOffset; i > 0; i--) {
      angle(legBackRight, limiter(legBackRightCenterValue + legMidStepOffset - (legMidStepOffset - i )));
      delay(delayTime);
      }
      break;

    case 3: 
        // FRONT RIGHT LEG EXTENDS
        for (int i = 0; i < middleMidStepOffset; i++) {
        angle(middleFrontRight, middleFrontRightCenterValue + i);
        delay(delayTime);
        }
        for (int i = 0; i < legMidStepOffset; i++) {
        angle(legFrontRight, legFrontRightCenterValue + i);
        delay(delayTime);
        }

        break;
      
      case 4:
        for (int i = 0; i < bodyFinalOffset; i++) {
        angle( bodyFrontRight , bodyFrontRightCenterValue + i );
        delay(delayTime);
        }
        for (int i = -middleMidStepOffset; i < middleMidStepOffset; i++) {
        angle(middleFrontRight, middleFrontRightCenterValue - i);
        delay(delayTime);
        }
        break;

    case 5:
      // BODY CONDENSES
      for (int i = middleMidStepOffset; i > 0 ; i--) {
      
      // Front right
      angle(middleFrontRight, limiter( middleFrontRightCenterValue - i ) );
      angle(legFrontRight, limiter( legFrontRightCenterValue + legMidStepOffset - (legMidStepOffset - map(i, 0, middleMidStepOffset, 0, legMidStepOffset))  ));
      angle(bodyFrontRight, limiter(bodyFrontRightCenterValue + bodyFinalOffset - (bodyFinalOffset - map(i, 0, middleMidStepOffset, 0, bodyFinalOffset )    )));
      
      // Back left
      angle(middleBackLeft, middleBackLeftCenterValue - (middleMidStepOffset - i));
      angle(legBackLeft, limiter( legBackLeftCenterValue + legMidStepOffset  - map(i, 0, middleMidStepOffset, 0, legMidStepOffset )));
      
      // back right
      angle(bodyBackRight, limiter(bodyBackRightCenterValue + bodyFinalOffset - (bodyFinalOffset - map(i, 0, middleMidStepOffset, 0, bodyFinalOffset))));
      
      // front right
      angle(bodyFrontLeft, limiter(bodyFrontLeftCenterValue + bodyFinalOffset - ( map(i, 0, middleMidStepOffset, 0, bodyFinalOffset))));
      
      delay(delayTime);
      }
      break;

    case 6:
      
      // BACK LEFT
      for (int i = 0; i < middleMidStepOffset + 5; i++) {
      angle(middleBackLeft, middleBackLeftCenterValue + i);
      delay(delayTime);
      }
      for (int i = 0; i < bodyFinalOffset; i++) {
      angle(bodyBackLeft, bodyBackLeftCenterValue - i);
      delay(delayTime);
      }
      break;

    case 7: 
      for (int i = legMidStepOffset; i > 0 ; i--) {
      angle(legBackLeft, limiter( legBackLeftCenterValue + legMidStepOffset - (legMidStepOffset - i) )  );
      delay(delayTime);
      }
      for (int i = middleMidStepOffset; i > 0 ; i--) {
      angle(middleBackLeft, limiter( middleBackLeftCenterValue - middleMidStepOffset + (middleMidStepOffset - i) )  );
      delay(delayTime);
      }
      break;

    case 8:
      // FRONT LEFT
      for (int i = 0; i < middleMidStepOffset + 5; i++) {
      angle( middleFrontLeft, middleFrontLeftCenterValue + i);
      delay(delayTime);
      }
      for (int i = 0; i < legMidStepOffset; i++) {
      angle(legFrontLeft, legFrontLeftCenterValue + i);
      delay(delayTime);
      }
      break;

    default:
      for (int i = 0; i < bodyFinalOffset; i++) {
      angle( bodyFrontLeft, bodyFrontLeftCenterValue - i );
      delay(delayTime);
      }
      for (int i = middleMidStepOffset + 5; i > 0 ; i--) {
      angle(middleFrontLeft,  middleFrontLeftCenterValue + middleMidStepOffset + 5 - (middleMidStepOffset + 5 - i) );
      delay(delayTime);
      }
      for (int i = legMidStepOffset; i > 0; i--) {
      angle(legFrontLeft, limiter(legFrontLeftCenterValue + legMidStepOffset - (legMidStepOffset - i )));
      delay(delayTime);
      }
      break;
  }
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




int delay_time = 10;
void mamba() {
  for (int i = 90; i > 30; i--) {
    angle( bodyBackLeft, i);
    angle( bodyBackRight, i);
    angle( bodyFrontLeft, i);
    angle( bodyFrontRight, i);
    delay(4);
  }

  for (int i = 30; i < 140; i++) {
    angle( bodyBackLeft, i);
    angle( bodyBackRight, i);
    angle( bodyFrontLeft, i);
    angle( bodyFrontRight, i);
    delay(delay_time);
  }

  for (int i = 140; i > 30; i--) {

    angle( bodyBackLeft, i);
    angle( bodyBackRight, i);
    angle( bodyFrontLeft, i);
    angle( bodyFrontRight, i);
    delay(delay_time);
  }


}




void setAllStraight() {

  angle( bodyFrontLeft, bodyFrontLeftCenterValue);
  angle( legFrontLeft, legFrontLeftCenterValue);
  angle( middleFrontLeft, middleFrontLeftCenterValue);

  angle( bodyBackRight, bodyBackRightCenterValue);
  angle( legBackRight, legBackRightCenterValue);
  angle( middleBackRight, middleBackRightCenterValue);

  angle( bodyFrontRight, bodyFrontRightCenterValue);
  angle( legFrontRight, legFrontRightCenterValue);
  angle( middleFrontRight, middleFrontRightCenterValue);

  angle( bodyBackLeft, bodyBackLeftCenterValue);
  angle( legBackLeft, legBackLeftCenterValue);
  angle( middleBackLeft, middleBackLeftCenterValue);
}



void angle(int OutputLine, int Angle) {
  pwm.setPWM(OutputLine, 0, returnMappedValue(Angle)); // center front right MIDDLE
}

int returnMappedValue(int value) {
  value = map(value, 0, 180, SERVOMIN, SERVOMAX);
  return value;
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
  while(!success) {
    success = radio.write(toWrite, 1);
  }
}

byte receiveValue() {
  byte readData[1];
  if(radio.available()) {
    radio.read(readData, 1);
    return readData[0];
  }
  return 0;
}

