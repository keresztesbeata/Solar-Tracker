#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

enum Mode {
  sensorMode,
  spaceMappingMode,
  lowPowerMode
};

Mode mode = sensorMode;
Mode previousMode = mode;

// Battery
byte battery[] = { B01110, B01010, B11011, B10001, B10001, B11111, B11111, B11111};

// compass: left, up, down arrows
byte compass[3][8] = {
  // left arrow
  {B00000, B00000, B00000, B00100, B01100, B11111, B01100, B00100},
  // up arrow
 {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00000},
  // right arrow 
  {B00000, B00000, B00000, B00100, B00110, B11111, B00110, B00100}
};

//sensor
byte sensor[3][8] = {
  {B00000, B00000, B00001, B00010, B00100, B00001, B00010, B00000},
  {B00000, B10101, B00000, B01010, B10001, B00000, B00100, B01110},
  {B00000, B00000, B10000, B01000, B00100, B10000, B01000, B00000}
};

int sensorAddr[] = {0,1,2};
int compassAddr[] = {3,4,5};
int batteryAddr = 6;

int changeModeBtn = A0;

void setup() {
  //--------------> initialize LCD <-----------------
  lcd.begin(16, 2);
  //-----------> create custom characters <----------
  for(int i=0;i<3;i++) {
      lcd.createChar(i, sensor[i]);
  }
  for(int i=0;i<3;i++) {
     lcd.createChar(3+i, compass[i]);
  }
  lcd.createChar(batteryAddr, battery);
}

void loop() {
  onModeChange();
  displayMode();
}

/**
 * Callback function to handle the change of state of the attached pin, which will change the functioning mode of the solar tracker.
 */
void onModeChange() {
  previousMode = mode;
  int input = analogRead (changeModeBtn);
  if (input < 60) {
    mode = sensorMode;
  }
  else if (input >= 400 && input < 600){
    mode = spaceMappingMode;
  }
  else if (input >= 600 && input < 800){
    mode = lowPowerMode;
  }
}

/**
 * Display the current mode on the LCD.
 */
void displayMode() {
  if(previousMode != mode) {
    lcd.clear();
  }
  lcd.setCursor(0,1);
  switch(mode) {
    case sensorMode : {
      lcd.print ("sensors ");
      for(int i = 0; i < 3; i++) {
        lcd.write (byte(sensorAddr[i]));  
      }
      break;
    }
    case spaceMappingMode : {
      lcd.print ("space map ");
      for(int i = 0; i < 3; i++) {
        lcd.write (byte(compassAddr[i]));  
      }
      break; 
    }
    case lowPowerMode : {
      lcd.print ("low power ");
      lcd.write (byte(batteryAddr));
      break;
    }
  }
}
