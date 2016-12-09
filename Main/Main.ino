#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <math.h>
#include <SPI.h>
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

double R1 = 11.5;  //length of curved leg portion of bot
double R2 = 6.5;   //length of junction between middle and leg servos
const float pi = 3.1415926; //pi, **is there a pi constant?**

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

//max and min body servo range limits
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
int middleFrontRightMax = 183;
int middleFrontLeftMax = 196;
int middleBackRightMax = 203;
int middleBackLeftMax = 180;

//bodyX positions; if leg and middle were fully extended, setting body servos
//to this would form a big X. Hence the name.
int bodyFrontRightX = bodyFrontRightCenterValue + bodyOffset;
int bodyFrontLeftX = bodyFrontLeftCenterValue - bodyOffset;
int bodyBackRightX = bodyBackRightCenterValue - bodyOffset;
int bodyBackLeftX = bodyBackLeftCenterValue + bodyOffset;

double lastMiddleSet = 0; //jank coding stuff to get the values
                          //    that legToPoint() set the servos too on its last call
double lastLegSet = 0;    //couldnt figure out how to return two vars easily
                          //    so this was next best option

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  //setAllStraight();



  delay(2000);
  walkSetupV2();
  delay(2000);

  int speed = 300;
  int i = 1;
  while (true) {
    delay(speed);
    walkForwardV2(1);
    delay(speed);
    walkForwardV2(2);
    delay(speed);
    walkForwardV2(3);
    delay(speed);
    walkForwardV2(4);
    delay(speed);
    walkForwardV2(5);
    delay(speed);
    walkForwardV2(6);
    i = i + 1;
  }
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
void loop() {}




int middleFinalStepOffset = 37;
int middleMidStepOffset = 30;
int legMidStepOffset = 40;
int bodyFinalOffset = 45;
int delayTime = 10;
void Yp( int t ) {
  double middleTheta = middleFinalStepOffset * sin( 3.141592 * t );
  angle(middleBackRight, middleBackRightCenterValue + (int) middleTheta );
}

//Theoretically would be called everytime before walkForwardV2
//        however that isn't really necessary just puts the
//        bot into a nice starting position (AA)
void walkSetupV2() {
  bodyX();
}




// Is designed to be a "longstep" gait for the bot. The gait breaks into 6 parts
//however it can be split into two groups of 3 steps that mirror eachother.
//step one is the backside leg steps forward and its leg/middle angle return to CV
//step two is forwardside leg extends and steps forward
//step three the forwardside contracts while the oppositebackside leg extends
//     simulateously the backside and opposite side front body servos swivel back
//     sending the bot diagonally forward.
//steps one through three then repeat for the other side
//
// *inc* tells the method which step to perform. (AA)
void walkForwardV2(int inc) {
  switch (inc) {
    case 1: //fix so left Lifts before movement
      angle(bodyBackLeft, anglePos(bodyBackLeft, 1, 0));
      angle(legBackLeft, legBackLeftCenterValue);
      angle(middleBackLeft, middleBackLeftCenterValue);
      break;
    case 2:
      angle(bodyFrontLeft, anglePos(bodyFrontLeft, 2, 0));
      angle(middleFrontLeft, middleFrontLeftMax);
      angle(legFrontLeft, 130);
      delay(200);
      legToPoint(10, 6.5, middleFrontLeft, legFrontLeft);
      break;
    case 3:
      for (int i = 1; i <= 100; i++) {
        //BODYSLIDE

        //Front Left retracts in
        angle(middleFrontLeft, lastMiddleSet + ((middleFrontLeftCenterValue - lastMiddleSet) * (i * .01)));
        angle(legFrontLeft, lastLegSet + ((legFrontLeftCenterValue - lastLegSet) * (i * .01)));

        //Back Right pushes forward
        angle(middleBackRight, middleBackRightCenterValue - ((middleBackRightCenterValue - lastMiddleSet) * (i * .01)));
        angle(legBackRight, legBackRightCenterValue - ((legBackRightCenterValue - lastLegSet) * (i * .01)));

        //Back Left rotates back
        angle(bodyBackLeft, anglePos(bodyBackLeft, 1, 0) - ((anglePos(bodyBackLeft, 1, 0) - bodyBackLeftX) * (i * .01)));

        //Front Right rotates backb *****************CHECK THIS*************  potentially should be: anglePos(bodyFrontRight,3,0) - (())
        angle(bodyFrontRight,  bodyFrontRightX - (( bodyFrontRightCenterValue + bodyOffset - anglePos(bodyFrontRight, 3, 0)) * (i * .01)));
      }
      break;
    case 4: //backright bumps up
      angle(bodyBackRight, anglePos(bodyBackRight, 1, 0));
      angle(legBackRight, legBackRightCenterValue);
      angle(middleBackRight, middleBackRightCenterValue);
      break;
    case 5: //front right long steps forward
      angle(bodyFrontRight, anglePos(bodyFrontRight, 2, 0));
      angle(middleFrontRight, middleFrontRightMax);
      angle(legFrontRight, 100);
      delay(200);
      legToPoint(10, 6.5, middleFrontRight, legFrontRight);
      break;
    case 6: //BODYSLIDE
      for (int i = 0; i <= 100; i++) {
        //Front Right retracts in
        angle(middleFrontRight, lastMiddleSet + ((middleFrontRightCenterValue - lastMiddleSet) * (i * .01)));
        angle(legFrontRight, lastLegSet + ((legFrontRightCenterValue - lastLegSet) * (i * .01)));

        //Back Left extends out
        angle(middleBackLeft, middleBackLeftCenterValue - ((middleBackLeftCenterValue - lastMiddleSet) * (i * .01)));
        angle(legBackLeft, legBackLeftCenterValue - ((legBackLeftCenterValue - lastLegSet) * (i * .01)));

        //Front Left rotates back
        angle(bodyFrontLeft,  bodyFrontLeftCenterValue + (( bodyFrontLeftCenterValue - anglePos(bodyFrontLeft, 3, 0)) * (i * .01)));

        //Back Right Rotates back
        angle(bodyBackRight, anglePos(bodyBackRight, 1, 0) - ((anglePos(bodyBackRight, 1, 0) - (bodyBackRightX)) * (i * .01)));
      }
      break;
    default:
      Serial.println("Invalid Number given to walkForwardV2()");
      break;
  }
}


//takes in a position number from 1-4, and returns proper servo angle for that pos.
// position 1 is max ahead, and position 4 is max behind, whatever that is for the given servo
// i is used to modify that position number. any i inputed could as number of degrees added to where the pos would be
//      however i is still on the 0-180 scale is it affects pre-map.
//
//*bodyServo* the body servo to be set
//*pos* the postion to be set to, should be from 1-4
//*i* modifies the pos, should be from 0-180 *exeptions may apply* (AA)
double anglePos(int bodyServo, int pos, int i) {
  if(pos>4 | pos<1){
    Serial.println("Error in anglePos()");
    return 0;
  }
  pos = pos - 1;
  if (bodyServo == bodyFrontRight) {
    return mapfr(180 - (60 * pos) + i);
  }
  else if (bodyServo == bodyFrontLeft) {
    return mapfl((60 * pos) + i);
  }
  else if (bodyServo == bodyBackLeft) {
    return mapbl((60 * pos) + i);
  }
  else if (bodyServo == bodyBackRight) {
    return mapbr(180 - (60 * pos) + i);
  }
  else {
    Serial.println("Error in anglePos()");
    return 0;
  }
}

//takes input *value* from 0-180 and maps it for the front right body servo pointing straight ahead to the max point back
//180 is Straight ahead
//0 points back  (AA)
int mapfr(int value) {
  value = map(value, 0, 180, bodyFrontRightMin, bodyFrontRightForwardFace);
  return value;
}


//takes input *value* from 0-180 and maps it for the front left body servo pointing straight ahead to the max point back
//0 is straight ahead
//180 points back  (AA)
int mapfl(int value) {
  value = map(value, 0, 180, bodyFrontLeftForwardFace, bodyFrontLeftMax);
  return value;
}


//takes input *value* from 0-180 and maps it for the back left body servo pointing straight behind to the max point ahead
//0 is max ahead
//180 is straight back  (AA)
int mapbl(int value) {
  
  value = map(value, 0, 180, bodyBackLeftMin, bodyBackLeftRearFace);
  return value;
}


//takes input *value* from 0-180 and maps it for the back left body servo pointing straight behind to the max point ahead
//180 is max ahead
// 0 is straight back  (AA)
int mapbr(int value) {
  value = map(value, 0, 180, bodyBackRightRearFace, bodyBackRightMax);
  return value;
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
    delay(delay_time);
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


// method is used to calibrate servos. For loop allows you to step through
// angles and watch/count live.  (AA)
void calibration() {
  angle(bodyBackLeft, bodyBackLeftRearFace);
  angle( legBackLeft, legBackLeftFullExtend - 73);
  delay(1000);
  angle( middleBackLeft, middleBackLeftCenterValue - 30);
  for (int i = 0; i < 180; i = i + 10)
  {
    delay(1000);
    angle( legBackLeft, legBackLeftFullExtend - i);
  }

  //angle( bodyFrontLeft, bodyFrontLeftCenterValue-(2*bodyOffset)-20);
}


//Sets body servo values so that legs points straight forward and back
//leg and middle servos are set to CVs  (AA)
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
//      *needs help most of the time to get up, will work better with stronger servos.  (AA)
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
// robot lies totally flat and then gets up into a ready postion
//
//*delayNum* how long you want bot to wait between coming up from full down  (AA)
void pushup(int delayNum) {
  delay(delayNum);
  extend();
  delay(delayNum);
  setAllStraight();
}


//sets body servos so that bot forms an X
//leg and middle servos are set to CVs
void bodyX() {
  angle( bodyFrontLeft, bodyFrontLeftX);
  angle( legFrontLeft, legFrontLeftCenterValue);
  angle( middleFrontLeft, middleFrontLeftCenterValue);

  angle( bodyBackRight, bodyBackRightX);
  angle( legBackRight, legBackRightCenterValue);
  angle( middleBackRight, middleBackRightCenterValue);

  angle( bodyFrontRight, bodyFrontRightX);
  angle( legFrontRight, legFrontRightCenterValue);
  angle( middleFrontRight, middleFrontRightCenterValue);

  angle( bodyBackLeft, bodyBackLeftX);
  angle( legBackLeft, legBackLeftCenterValue);
  angle( middleBackLeft, middleBackLeftCenterValue);

}


//robot lies as flat as possible. Leg and middle servos are set to full extention
//body servos are set to X postions  (AA)
void extend() {
  angle( bodyFrontLeft, bodyFrontLeftX);
  angle( legFrontLeft, legFrontLeftCenterValue - legOffset);
  angle( middleFrontLeft, middleFrontLeftCenterValue - middleOffset);

  angle( bodyBackRight, bodyBackRightX);
  angle( legBackRight, legBackRightCenterValue - legOffset);
  angle( middleBackRight, middleBackRightCenterValue - middleOffset);

  angle( bodyFrontRight, bodyFrontRightX);
  angle( legFrontRight, legFrontRightCenterValue - legOffset);
  angle( middleFrontRight, middleFrontRightCenterValue - middleOffset);

  angle( bodyBackLeft, bodyBackLeftX);
  angle( legBackLeft, legBackLeftCenterValue - legOffset);
  angle( middleBackLeft, middleBackLeftCenterValue - middleOffset);
}


//Sets body servo values so that legs points straight left and right(aka CVs)
//leg and middle servos are set to CVs  (AA)
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


//moves tip of leg to an xy point
//
//*x* horozontal distance to middle servo nut, must be POSITVE
//*y* vertical distance from the ground to the middle servo   **NEED TO CHECK IF CAN BE NEG**
//*servoMiddle* the middle servo, should be a pair with given leg servo
//*servoLeg* the leg servo, should be a pair with given middle servo  (AA)
void legToPoint(double x, double y, int servoMiddle, int servoLeg) {
  double p = sqrt(x * x + y * y);  //straight line distance to (x,y) point.
  double innerFull = acos(((R1 * R1) - (p * p) - (R2 * R2)) / (-2 * R2 * p));  //law of cosines, is angle between p and R2 (Rad)
  double theta = acos(((p * p) - (R2 * R2) - (R1 * R1)) / (-2 * R2 * R1));     //law of cosines again, is leg angle (Rad)
  double b = atan(y / x); //angle between the horozontal and p
  double phi = innerFull - b; //angle between R2 and horozontal
  double middleAngle = getMiddleMax(servoMiddle) - (90 - (phi * 180 / pi)); //middle angle, now in Deg.
  double legAngle = theta * 180 / pi;   //leg angle converted to deg.


  //**Should prob calibrate this individually for each servo but future Andrew problem**
  angle(servoMiddle, middleAngle );   
  angle(servoLeg, legAngle);
  lastMiddleSet = middleAngle;  //sets found true angles to global vars
  lastLegSet = legAngle;        //^^^
}


//returns the straight vertical angle value of a given middle servo;
//
//*servoMiddle* the MIDDLE servo in quesiton  (AA)
int getMiddleMax(int servoMiddle) {
  if (servoMiddle == middleFrontRight) {
    return middleFrontRightMax;
  }
  else if (servoMiddle == middleFrontLeft) {
    return middleFrontLeftMax;
  }
  else if (servoMiddle == middleBackRight) {
    return middleBackRightMax;
  }
  else if (servoMiddle == middleBackLeft) {
    return middleBackLeftMax;
  }
  else {
    Serial.println("Error in getMiddleMax()");
    return 0;
  }
}


//leg number, angle
void angle(int OutputLine, int Angle) {
  pwm.setPWM(OutputLine, 0, returnMappedValue(Angle)); // center front right MIDDLE
  //Serial.println(Angle);
}


int returnMappedValue(int value) {
  value = map(value, 0, 180, SERVOMIN, SERVOMAX);
  return value;
}

/*TODOS:
 * Check if legTOPoint() will work for negative values
 * Optimize walk function for less sliding. Make sure that on the weight shift forward back leg actually pushes
 * Hook up to pi and take commands from there
 * Make turning functions
 *      potentially include a curving walk
 * Make robot able to walk backwards or side to side.
 */

