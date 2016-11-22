/*
 * Author:  Jeremy Morgan
 * Date:    11/22/16
 * Notes:   Current design will not work due to insufficient memory on board. Solution is to do calculations in real time in loop, rather than store data and then analyze. 
 *          If real time analysis is not possible, transition sonar system to raspberry pi.
 */

#include <NewPing.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const int ScannerServoFrontLeft =   7 ;
const int ScannerServoFrontRight =  3; 
const int ScannerServoBackLeft =    11; 
const int ScannerServoBackRight =   15;

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
#define SLAVE_ADDRESS 0x05

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


int distances[360]; 
int frontLeftDistances[180];
int frontRightDistances[180];
int backLeftDistances[180];
int backRightDistances[180];

const double distanceFromCenterToServo = 16.5;

double get_B_Length(double a, double c, double bAngleDeg);

void setup() {
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  Serial.begin(9600);
  
  resetValues();
}

void loop() {

  for (int i = 0; i < 180; i++){
    angle(ScannerServoFrontLeft,  i);
    angle(ScannerServoFrontRight, i);
    angle(ScannerServoBackLeft,   i);
    angle(ScannerServoBackRight,  i);
    
    frontLeftDistances[i]   =  FrontLeft.ping_cm();
    frontRightDistances[i]  =  FrontRight.ping_cm();
    backLeftDistances[i]    =  BackLeft.ping_cm();
    backRightDistances[i]   =  BackRight.ping_cm();
  }

  // FRONT RIGHT CENTER VALUES
  for (int i = 0; i < 45; i ++){
    
      int FrontRightSonarReading = frontRightDistances[i];
      double SonarReadingDistanceFromCenter = get_B_Length( FrontRightSonarReading, distanceFromCenterToServo, 90+i);
      double AngleFromCenter = getA_Angle(FrontRightSonarReading, SonarReadingDistanceFromCenter, distanceFromCenterToServo);
      int translatedAngle = getValidAngle( (int) (45 - AngleFromCenter));

      // The angle is overlapping with quadrant 4
      if (translatedAngle < 0){
        // the angle has not been set before
        if (distances[translatedAngle] == -1){
          distances[i] = SonarReadingDistanceFromCenter;
        }
        else{
            distances[translatedAngle] = (int) ((distances[translatedAngle] + SonarReadingDistanceFromCenter)/2);
        }
      }
      else {
        distances[i] = SonarReadingDistanceFromCenter;
      }
  }

  for (int i = 45; i < 135; i ++){
      int FrontRightSonarReading = frontRightDistances[i];
      double SonarReadingDistanceFromCenter = get_B_Length( FrontRightSonarReading, distanceFromCenterToServo, 90+i);
      double AngleFromCenter = getA_Angle(FrontRightSonarReading, SonarReadingDistanceFromCenter, distanceFromCenterToServo);
      int translatedAngle = getValidAngle( (int) (45 - AngleFromCenter));
      
  }



  for (int i = 0; i < 360; i++){
    Serial.print("Angle "+String(i)+": "+String(distances[i]));  
  }
  Serial.println();
  delay(1000);
}

int getValidAngle(int angle){

  if (angle < 0){
    while (angle<0){
      angle = angle + 360;
    }  
    return angle;
  }
  else {
    return angle % 360;  
  }
}

double getA_Angle(int a, int b, int c){

  double top = pow(a,2);
  top = top - pow(b,2) ;
  top = top - pow(c,2);
  double bottom = (-2 * b * c);
  double frac = top/bottom;
  double ret = acos(frac) ;
  ret = ret * (57296/1000);
  return ret;
}

double get_B_Length(double a, double c, double bAngleDeg){
   double bAngleRad = bAngleDeg * 1000 / 57296;
   double squareRooted = pow(a,2) + pow (c,2) ;
   squareRooted = squareRooted - ( 2 * a * c * cos(bAngleRad)   );
   double retVal = sqrt(squareRooted);
   return retVal;
}

int midVal(int x1, int x2){
  return  (int) ( x1 + (x2-x1)/2 ); 
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
  pwm.setPWM(OutputLine, 0, limiter(returnMappedValue(Angle))); // center front right MIDDLE
}

int returnMappedValue(int value) {
  value = map(value, 0, 180, SERVOMIN, SERVOMAX);
  return value;
}

void resetValues(){
   for (int i = 0 ; i < 360; i ++){
    distances[i] = -1;
  }
  for (int i = 0 ; i < 180; i ++){
    frontLeftDistances[i]   = -1;
    frontRightDistances[i]  = -1;
    backLeftDistances[i]    = -1;
    backRightDistances[i]   = -1;
  }

}


