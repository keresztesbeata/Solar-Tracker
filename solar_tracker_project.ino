#include <Servo.h>
Servo pan;    // create servo object to control the yaw servo
Servo tilt;  // create servo object to control the pitch servo

#define RESOLUTION 1
#define DELAY 10
#define MIN_DIFF 30
#define STEP_DIVIDER 30
#define ANALOG_READ_DELAY 100

#define NO_MEASUREMENTS 5

int pinPhotoResistorS = A1;
int pinPhotoResistorN = A2;
int pinPhotoResistorE = A3;
int pinPhotoResistorW = A0;

#define MIN_TILT 5
#define MAX_TILT 85

int panPin = 2;
int tiltPin = 4;

int panAngle = 90, prevPanAngle = 0;
int tiltAngle = 90, prevTiltAngle = 0;

struct LightIntensityData {
  int S = 0;
  int N = 0;
  int E = 0;
  int W = 0;
};

void setup() {

  // setup pins for photoresistors
  pinMode(pinPhotoResistorN, INPUT);
  pinMode(pinPhotoResistorS, INPUT);
  pinMode(pinPhotoResistorE, INPUT);
  pinMode(pinPhotoResistorW, INPUT);

  // setup Serial for debugging
  Serial.begin(9600);

  // setup pins for servo motors
  pan.attach(panPin);   // attaches the servo on pin 2 to the servo object
  tilt.attach(tiltPin);  // attaches the servo on pin 3 to the servo object
}

void loop() {
  // reading the photoresistor values...
  LightIntensityData lightIntData = readLightIntensities();

  // printing the photoresistor values
  Serial.println("S N E W");
  Serial.print(lightIntData.S);
  Serial.print(" ");
  Serial.print(lightIntData.N);
  Serial.print(" ");
  Serial.print(lightIntData.E);
  Serial.print(" ");
  Serial.println(lightIntData.W);

  // compute new angles
  int SNdirectionDiff = lightIntData.S - lightIntData.N;
  if (abs(SNdirectionDiff) >= MIN_DIFF) {
    prevTiltAngle = tiltAngle;
    tiltAngle = max(MIN_TILT, min(MAX_TILT, tiltAngle + SNdirectionDiff/STEP_DIVIDER));
  } else {
    prevTiltAngle = tiltAngle;
  }
  
  int EWdirectionDiff = lightIntData.E - lightIntData.W;
  if (abs(EWdirectionDiff) >= MIN_DIFF) {
    prevPanAngle = panAngle;
    panAngle = max(0, min(180, panAngle + EWdirectionDiff/STEP_DIVIDER));
  } else {
    prevPanAngle = panAngle;
  }

  // move motors to the new angles, if a change occured
  if (panAngle != prevPanAngle) {
    pan.write(panAngle);
  }
  if (tiltAngle != prevTiltAngle) {
    tilt.write(tiltAngle);
  }

  // display data for debugging purposes
  Serial.println("Intensity diffs");
  Serial.print("S-N=");
  Serial.print(SNdirectionDiff);
  Serial.print("; W-E=");
  Serial.println(EWdirectionDiff);
  Serial.print("Motor angles: ");
  Serial.print("PanAngle=");
  Serial.print(panAngle);
  Serial.print("; tiltAngle=");
  Serial.println(tiltAngle);
}

LightIntensityData readLightIntensities() {
  LightIntensityData result;

  int lightIntensityS, lightIntensityN, lightIntensityE, lightIntensityW;
  
  for (int i = 0; i < NO_MEASUREMENTS; i++) {
    lightIntensityS = analogRead(pinPhotoResistorS);
    delay(ANALOG_READ_DELAY);
    lightIntensityN = analogRead(pinPhotoResistorN);
    delay(ANALOG_READ_DELAY);
    lightIntensityE = analogRead(pinPhotoResistorE);
    delay(ANALOG_READ_DELAY);
    lightIntensityW = analogRead(pinPhotoResistorW);
    delay(ANALOG_READ_DELAY);

    result.S += lightIntensityS;
    result.N += lightIntensityN;
    result.E += lightIntensityE;
    result.W += lightIntensityW;
  }

  result.S /= NO_MEASUREMENTS;
  result.E /= NO_MEASUREMENTS;
  result.W /= NO_MEASUREMENTS;
  result.N /= NO_MEASUREMENTS;

  return result;
}
