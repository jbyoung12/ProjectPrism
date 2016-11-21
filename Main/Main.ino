#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <math.h>
#include <SPI.h>
#include <Wire.h>
#define OLED_RESET 4
#define NUMFLAKES 10
#define DELTAY 2a
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
#define CHANNEL 54
#define PIPE 0xF0F0F0F0F0


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const int bodyFrontLeft = 4;  const int middleFrontLeft = 5;  const int legFrontLeft = 6 ;
const int bodyFrontRight = 0; const int middleFrontRight = 1; const int legFrontRight = 2;
const int bodyBackRight = 12; const int middleBackRight = 13; const int legBackRight = 14;
const int bodyBackLeft = 8;   const int middleBackLeft = 9;   const int legBackLeft = 10;

void foward(int inc);
void setAllStraight();
void turnLeft(int inc);

int bodyOffset = 45;
int middleOffset = 75;
int legOffset = -170;
//----------------------------------------------------//
int bodyBackRightCenterValue = 90 - 5 + bodyOffset;         int bodyBackRightCurrentValue =  bodyBackRightCenterValue;  // back right middle
int middleBackRightCenterValue = 90 + 20 + middleOffset;    int middleBackRightCurrentValue =  middleBackRightCenterValue; // back right middle
int legBackRightCenterValue = 180 + 5 + legOffset;                        int legBackRightCurrentValue  = legBackRightCenterValue;     // back right leg
//----------------------------------------------------//

//----------------------------------------------------//
int bodyFrontRightCenterValue = 90 - 7 - bodyOffset;   int bodyFrontRightCurrentValue = bodyFrontRightCenterValue; // front right middle
int middleFrontRightCenterValue = 90 + middleOffset;   int middleFrontRightCurrentValue = middleFrontRightCenterValue; // front right middle
int legFrontRightCenterValue = 180 - 3 + legOffset;                       int legFrontRightCurrentValue = legFrontRightCenterValue ;  // front right leg
//----------------------------------------------------//

//----------------------------------------------------//
int bodyFrontLeftCenterValue = 90 + bodyOffset;         int bodyFrontLeftCurrentValue = bodyFrontLeftCenterValue;     // front left body
int middleFrontLeftCenterValue = 90 + 13 + middleOffset;    int middleFrontLeftCurrentValue = middleFrontLeftCenterValue; // front left middle
int legFrontLeftCenterValue = 180 + legOffset;                       int legFrontLeftCurrentValue = legFrontLeftCenterValue;       // front left leg
//----------------------------------------------------//

//----------------------------------------------------//
int bodyBackLeftCenterValue = 90 - 10 - bodyOffset;          int bodyBackLeftCurrentValue = bodyBackLeftCenterValue;     //back left middle
int middleBackLeftCenterValue = 90 + middleOffset;     int middleBackLeftCurrentValue = middleBackLeftCenterValue; //back left middle
int legBackLeftCenterValue = 180 + legOffset;                         int legBackLeftCurrentValue = legBackLeftCenterValue;         // back left leg
//----------------------------------------------------//

//front and rear dir servo sets
int bodyFrontRightForwardFace = bodyFrontRightCenterValue + (2 * bodyOffset) + 4;
int bodyFrontLeftForwardFace = bodyFrontLeftCenterValue - (2 * bodyOffset) - 5;
int bodyBackRightRearFace = bodyBackRightCenterValue - (2 * bodyOffset) - 3;
int bodyBackLeftRearFace = bodyBackLeftCenterValue + (2 * bodyOffset) + 5;

//Leg FullExtention
int legFrontLeftFullExtend = legFrontLeftCenterValue - legOffset;
int legFrontRightFullExtend = legFrontRightCenterValue - legOffset;
int legBackLeftFullExtend = legBackLeftCenterValue - legOffset;
int legBackRightFullExtand = legBackRightCenterValue - legOffset;

//middle FullExtention
int middleFrontLeftFullExtend =  middleFrontLeftCenterValue - middleOffset;
int middleFrontRightFullExtend = middleFrontRightCenterValue - middleOffset;
int middleBackLeftFullExtend = middleBackLeftCenterValue - middleOffset;
int middleBackRightFullExtend = middleBackRightCenterValue - middleOffset;

//max and min servo ranges limits body
//bodyFrontRight limits
int bodyFrontRightMin = -5;
int bodyFrontRightMax = 180; //didnt push this one all the way, tons of wire in way, could maybe get +5 more
//bodyFrontLeft limits
int bodyFrontLeftMin = -9;
int bodyFrontLeftMax = 175;
//bodyBackRight limits
int bodyBackRightMin = -10;
int bodyBackRightMax = 180;
//bodyBackLeft limits
int bodyBackLeftMax = 187;
int bodyBackLeftMin = -5;

//max servo range limit middle
    //when set to this range, servos point straight up
int middleFrontRightMax =183;
int middleFrontLeftMax =196;
int middleBackRightMax = 203;
int middleBackLeftMax =180;


void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  //setAllStraight();
  //  delay(10000);
  //extend();
  //tiptoes();
  //setForwardRearDir();
  //   test();

   
  delay(2000);
  legToPoint(10,8.569,middleFrontLeft, legFrontLeft);
  delay(3000);
  legToPoint(12,8.569,middleFrontLeft, legFrontLeft);

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


int numCycles = 36;
int currentNumCycles = 0;
void loop() {


  //mamba();


  //  delay(1000);
  //setAllStraight();
  // for (int i = bodyBackRightCenterValue; i<180  ; i++) {
  //        angle(bodyBackRight,  i);
  //        Serial.println(i);
  //        delay(200);
  //
  //      }

  //  delay(1000);

  //  if (currentNumCycles < numCycles)
  //  {
  //    if (stepInc != stepMaxInc + 1) {
  //      forward(stepInc);
  //      stepInc ++;
  //    }
  //    else {
  //      stepInc = 1;
  //    }
  //    currentNumCycles = currentNumCycles + 1;
  //  }
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


void Yp( int t ) {

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










//void forwardV2(int inc) {
//  switch(inc){
//    case 1:
//    //backRight
//
//
//
//    break
//  }
//}



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



void test() {
  angle(bodyBackLeft, bodyBackLeftRearFace);
  angle( legBackLeft, legBackLeftFullExtend - 73);
  delay(1000);
  angle( middleBackLeft, middleBackLeftCenterValue - 30);
  //    for (int i = 0; i<180;i=i+10)
  //    {
  //      delay(1000);
  //      angle( legBackLeft, legBackLeftFullExtend-i);
  //    }



  //angle( bodyFrontLeft, bodyFrontLeftCenterValue-(2*bodyOffset)-20);

}

void setForwardRearDir() {
  angle( bodyFrontLeft, bodyFrontLeftForwardFace);
  angle( legFrontLeft, legFrontLeftCenterValue);
  angle( middleFrontLeft, middleFrontLeftCenterValue);

  angle( bodyBackRight, bodyBackRightRearFace);
  angle( legBackRight, legBackRightCenterValue);
  angle( middleBackRight, middleBackRightCenterValue);

  angle( bodyFrontRight, bodyFrontRightForwardFace);
  angle( legFrontRight, legFrontRightCenterValue);
  angle( middleFrontRight, middleFrontRightCenterValue);

  angle( bodyBackLeft, bodyBackLeftRearFace);
  angle( legBackLeft, legBackLeftCenterValue);
  angle( middleBackLeft, middleBackLeftCenterValue);
}


//robot should have legs extand fully downwards and achieve max possible height
void tiptoes() {
  angle( bodyFrontLeft, bodyFrontLeftCenterValue - bodyOffset);
  angle( legFrontLeft, legFrontLeftCenterValue - legOffset);
  angle( middleFrontLeft, 0);

  angle( bodyBackRight, bodyBackRightCenterValue - bodyOffset);
  angle( legBackRight, legBackRightCenterValue - legOffset);
  angle( middleBackRight, 0);

  angle( bodyFrontRight, bodyFrontRightCenterValue + bodyOffset);
  angle( legFrontRight, legFrontRightCenterValue - legOffset);
  angle( middleFrontRight, 0);

  angle( bodyBackLeft, bodyBackLeftCenterValue + bodyOffset);
  angle( legBackLeft, legBackLeftCenterValue - legOffset);
  angle( middleBackLeft, 0);

}

//robot alternates from extend to set straight.
void pushups(int delayNum) {
  delay(delayNum);
  extend();
  delay(delayNum);
  setAllStraight();
}



void extend() {
  angle( bodyFrontLeft, bodyFrontLeftCenterValue - bodyOffset);
  angle( legFrontLeft, legFrontLeftCenterValue - legOffset);
  angle( middleFrontLeft, middleFrontLeftCenterValue - middleOffset);

  angle( bodyBackRight, bodyBackRightCenterValue - bodyOffset);
  angle( legBackRight, legBackRightCenterValue - legOffset);
  angle( middleBackRight, middleBackRightCenterValue - middleOffset);

  angle( bodyFrontRight, bodyFrontRightCenterValue + bodyOffset);
  angle( legFrontRight, legFrontRightCenterValue - legOffset);
  angle( middleFrontRight, middleFrontRightCenterValue - middleOffset);

  angle( bodyBackLeft, bodyBackLeftCenterValue + bodyOffset);
  angle( legBackLeft, legBackLeftCenterValue - legOffset);
  angle( middleBackLeft, middleBackLeftCenterValue - middleOffset);
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
  Serial.println(middleFrontRightCenterValue);

  angle( bodyBackLeft, bodyBackLeftCenterValue);
  angle( legBackLeft, legBackLeftCenterValue);
  angle( middleBackLeft, middleBackLeftCenterValue);
}

double R1 = 11.5;
double R2 = 6.5;
const float pi = 3.1415926;
void legToPoint(double x, double y, int servoMiddle, int servoLeg) {
  double p = sqrt(x*x + y*y);
  double innerFull = acos(((R1*R1)-(p*p) - (R2*R2)) / (-2 * R2 * p));
  Serial.print("innnerFull: ");
  Serial.println(innerFull);
  double theta = acos(((p*p) - (R2*R2) - (R1*R1)) / (-2 * R2 * R1));
  double b = atan(y/x);
  double phi = innerFull - b;
  Serial.print("Theta: ");
  Serial.println(theta*180/pi);
  Serial.print("Phi: ");
  Serial.println(phi*180/pi);
  double middleAngle = getMiddleMax(servoMiddle) -(90-(phi*180/pi));
  double legAngle = theta*180/pi;
  Serial.print("Middle Angle: ");
  Serial.println(middleAngle);
  Serial.print("leg angle: ");
  Serial.println(legAngle);
  angle(servoMiddle, middleAngle );
  angle(servoLeg, legAngle);
}

int getMiddleMax(int servoMiddle){
  if(servoMiddle == middleFrontRight){
    return middleFrontRightMax;
  }
  else if(servoMiddle == middleFrontLeft){
    return middleFrontLeftMax;
  }
  else if(servoMiddle == middleBackRight){
    return middleBackRightMax;
  }
  else if(servoMiddle == middleBackLeft){
    return middleBackLeftMax;
  }
  else{
    Serial.println("Error in getMiddleMax()");
    return 0;
  }
}


double xFn(double theta,double phi,double x){
  double sol = R2*cos(phi) + R1*sin(theta-90+phi)-x;
  return sol;
}
int sgn(double input){
  if (input>0){
    return 1;
  }
  else if (input<0){
    return -1;
  }
  else{
   // Serial.print("Error in sgn: ");
    //Serial.println(input);
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



