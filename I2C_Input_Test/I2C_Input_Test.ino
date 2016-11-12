#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
#define SLAVE_ADDRESS 0x04

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const int bodyFrontLeft = 4;  const int middleFrontLeft = 5;  const int legFrontLeft = 6 ;
const int bodyFrontRight = 0; const int middleFrontRight = 1; const int legFrontRight = 2;
const int bodyBackRight = 12; const int middleBackRight = 13; const int legBackRight = 14;
const int bodyBackLeft = 8;   const int middleBackLeft = 9;   const int legBackLeft = 10;

void setAllStraight();
void sendData();
void receiveArrayData(int numByte);
int limiter(int input);
void Yp( int t );
void turnLeft(int inc);
void turnRight(int inc);
void turnRight(int inc);
void backward(int inc);
void forward(int inc);
int limiter(int input);
void setAllStraight();
void angle(int OutputLine, int Angle);

int bodyOffset = 25;
int middleOffset = 15;
int legOffSet = 15;
//----------------------------------------------------//
int bodyBackRightCenterValue = 95 + bodyOffset;         int bodyBackRightCurrentValue =  bodyBackRightCenterValue;      // back right middle
int middleBackRightCenterValue = 165 - middleOffset;    int middleBackRightCurrentValue =  middleBackRightCenterValue; // back right middle
int legBackRightCenterValue = 80 - legOffSet;           int legBackRightCurrentValue  = legBackRightCenterValue;       // back right leg
//----------------------------------------------------//

//----------------------------------------------------//
int bodyFrontRightCenterValue = 90 - bodyOffset;        int bodyFrontRightCurrentValue = bodyFrontRightCenterValue;     // front right middle
int middleFrontRightCenterValue = 180 - middleOffset;   int middleFrontRightCurrentValue = middleFrontRightCenterValue; // front right middle
int legFrontRightCenterValue = 40 - legOffSet;          int legFrontRightCurrentValue = legFrontRightCenterValue ;      // front right leg
//----------------------------------------------------//

//----------------------------------------------------//
int bodyFrontLeftCenterValue = 90 + bodyOffset;         int bodyFrontLeftCurrentValue = bodyFrontLeftCenterValue;       // front left body
int middleFrontLeftCenterValue = 180 - middleOffset;    int middleFrontLeftCurrentValue = middleFrontLeftCenterValue; // front left middle
int legFrontLeftCenterValue = 80 - legOffSet;           int legFrontLeftCurrentValue = legFrontLeftCenterValue;         // front left leg
//----------------------------------------------------//

//----------------------------------------------------//
int bodyBackLeftCenterValue = 90 - bodyOffset;          int bodyBackLeftCurrentValue = bodyBackLeftCenterValue;         //back left middle
int middleBackLeftCenterValue = 180 - middleOffset;     int middleBackLeftCurrentValue = middleBackLeftCenterValue;   //back left middle
int legBackLeftCenterValue = 40 - legOffSet;            int legBackLeftCurrentValue = legBackLeftCenterValue;           // back left leg
//----------------------------------------------------//



void setup() {  
  Serial.begin(9600);
  Serial.println("Beggining setup()");
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(2000);
  pinMode(13,OUTPUT);
  Wire.begin(4);                // join i2c bus with address #8
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveArrayData);
  Wire.onRequest(sendData);
  Serial.println("Finished setup()");
}



const int turnRightMaxInc = 9;
int turnRightInc = 1;

const int turnLeftMaxInc = 9;
int turnLeftInc = 1;

const int stepForwardMaxInc = 9;
int stepForwardInc = 1;

const int stepBackwardIncMax = 9;
int stepBackwardInc = 1;

// currState values
// 0= stop, 1=forward, 2=right, 3=backward, 4=left 
int currState = 0;
int ledPin = 13;

// [ ledState,autonomousState, horizontalVideoValue, verticalVideoValue, xMovement, yMovement]
const int numValues = 6;
int incomingValues[] = {0,0,90,90,90,90};
int counter = 0;

void loop() {

  // incomingValues = [ ledState,autonomousState, horizontalVideoValue, verticalVideoValue, xMovement, yMovement]

  if (incomingValues[4] > 90){
    currState = 2;
  }
  if (incomingValues[4] < 90){
    currState = 4;
  }

  if (incomingValues[5] > 90){
    currState = 1;
  }
  if (incomingValues[5] < 90){
    currState = 3;
  }

  if (incomingValues[5] == incomingValues[4] == 0){
    currState = 0;
  }
  
  switch (currState) {
    
        case 1:
          forward(stepForwardInc);
          stepForwardInc++;
          if (stepForwardInc == stepForwardMaxInc+1){ stepForwardInc =1; }
          turnRightInc=1;
          turnLeftInc=1;
          stepBackwardInc=1;
          
          break;
    
        case 2:
          turnRight(turnRightInc);
          turnRightInc++;
          if (turnRightInc == turnRightMaxInc+1){ turnRightInc =1; }
          stepForwardInc=1;
          turnLeftInc=1;
          stepBackwardInc=1;
          
          break;
        
        case 3:
          backward(stepBackwardInc);
          stepBackwardInc++;
          if (stepBackwardInc == stepBackwardIncMax+1){ stepBackwardInc =1; }
          stepForwardInc=1;
          turnLeftInc=1;
          turnRightInc=1;
          break;
        
        case 4:
          turnLeft(turnLeftInc);
          turnLeftInc++;
          if (turnLeftInc == turnLeftMaxInc+1){ turnLeftInc =1; }
          stepForwardInc=1;
          turnRightInc=1;
          stepBackwardInc=1;
          break;
        
        default:
          setAllStraight();
      }
  
    delay(100);
}
  


void receiveArrayData(int numByte){
  int currentInt = Wire.read();
  Serial.println("incoming int: "+ String(currentInt));
  
  if (counter == numValues-1){
    counter = 0;
  }

  incomingValues[counter] = currentInt;
  counter++;
  currState = currentInt;
}



void sendData(){
  Serial.println("in sendData");
  Wire.write(0); 
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


//leg number, angle
void angle(int OutputLine, int Angle) {
  pwm.setPWM(OutputLine, 0, returnMappedValue(Angle)); // center front right MIDDLE
}

int returnMappedValue(int value) {
  value = map(value, 0, 180, SERVOMIN, SERVOMAX);
  return value;
}



