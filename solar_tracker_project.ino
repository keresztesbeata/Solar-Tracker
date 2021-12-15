#include <Servo.h>
Servo pan;    // create servo object to control the yaw servo
Servo tilt;  // create servo object to control the pitch servo

#define RESOLUTION 1
#define DELAY 10
#define MIN_DIFF 30
#define STEP_DIVIDER 30
#define ANALOG_READ_DELAY 100

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
  int lightIntensityS = analogRead(pinPhotoResistorS);
  delay(ANALOG_READ_DELAY);
  int lightIntensityN = analogRead(pinPhotoResistorN);
  delay(ANALOG_READ_DELAY);
  int lightIntensityE = analogRead(pinPhotoResistorE);
  delay(ANALOG_READ_DELAY);
  int lightIntensityW = analogRead(pinPhotoResistorW);
  delay(ANALOG_READ_DELAY);

  // printing the photoresistor values
  Serial.println("S N E W");
  Serial.print(lightIntensityS);
  Serial.print(" ");
  Serial.print(lightIntensityN);
  Serial.print(" ");
  Serial.print(lightIntensityE);
  Serial.print(" ");
  Serial.println(lightIntensityW);

  // compute new angles
  int SNdirectionDiff = lightIntensityS - lightIntensityN;
  if (abs(SNdirectionDiff) >= MIN_DIFF) {
    prevTiltAngle = tiltAngle;
    tiltAngle = max(MIN_TILT, min(MAX_TILT, tiltAngle + SNdirectionDiff/STEP_DIVIDER));
  } else {
    prevTiltAngle = tiltAngle;
  }
  
  int EWdirectionDiff = lightIntensityE - lightIntensityW;
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
