#include <Servo.h>
Servo pan;    // create servo object to control the yaw servo
Servo tilt;  // create servo object to control the pitch servo

#define RESOLUTION 1
#define DELAY 10
#define MIN_DIFF 30
#define STEP_DIVIDER 30

int pinPhotoResistorS = A1;
int pinPhotoResistorN = A2;
int pinPhotoResistorE = A3;
int pinPhotoResistorW = A0;

int panAngle = 90;
int tiltAngle = 90;



void setup() {
  pinMode(pinPhotoResistorN, INPUT);
  pinMode(pinPhotoResistorS, INPUT);
  pinMode(pinPhotoResistorE, INPUT);
  pinMode(pinPhotoResistorW, INPUT);
  Serial.begin(9600);
  pan.attach(2);   // attaches the servo on pin 2 to the servo object
  tilt.attach(4);  // attaches the servo on pin 3 to the servo object
}

void loop() {
  // put your main code here, to run repeatedly:
  int lightIntensityS = analogRead(pinPhotoResistorS);
  int lightIntensityN = analogRead(pinPhotoResistorN);
  int lightIntensityE = analogRead(pinPhotoResistorE);
  int lightIntensityW = analogRead(pinPhotoResistorW);
  Serial.println("S N E W");
  Serial.print(lightIntensityS);
  Serial.print(" ");
  Serial.print(lightIntensityN);
  Serial.print(" ");
  Serial.print(lightIntensityE);
  Serial.print(" ");
  Serial.println(lightIntensityW);

  int SNdirectionDiff = lightIntensityS - lightIntensityN;
  if (abs(SNdirectionDiff) >= MIN_DIFF)
    tiltAngle = max(0, min(90, tiltAngle + SNdirectionDiff/STEP_DIVIDER));

  
  int EWdirectionDiff = lightIntensityE - lightIntensityW;
  if (abs(EWdirectionDiff) >= MIN_DIFF)
    panAngle = max(0, min(180, panAngle + EWdirectionDiff/STEP_DIVIDER));

  pan.write(panAngle);
  tilt.write(tiltAngle);

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

  /*Check left/right and center movement*/
//  Serial.println("Pan Control");
//  for(int i=90* RESOLUTION;  i<180 * RESOLUTION; i++) { pan.write((float) i / RESOLUTION); delay(DELAY); }
//  for(int i=180* RESOLUTION;  i> 0 * RESOLUTION; i--) { pan.write((float) i / RESOLUTION); delay(DELAY); }
//  for(int i=0* RESOLUTION;  i<90 * RESOLUTION; i++) { pan.write((float) i / RESOLUTION); delay(DELAY); }
//  
//  /*Check up/down and center movement*/
//  Serial.println("Tilt Control");
//  //for(int i=90;  i<145; i++) { tilt.write(i); delay(25); }
//  for(int i=90 * RESOLUTION; i > 0;   i--) { tilt.write((float) i / RESOLUTION); delay(DELAY); }
//  for(int i=0 * RESOLUTION; i < 90 * RESOLUTION; i++) { tilt.write((float) i / RESOLUTION); delay(DELAY); }

  delay(1000);
}
