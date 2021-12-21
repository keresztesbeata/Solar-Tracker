#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

enum Mode {
  sensorMode,
  spaceMappingMode,
  lowPowerMode
};

Mode mode = sensorMode;

// Battery
byte battery[] = {
  B01110,
  B01010,
  B11011,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111
};

// compass: left, up, down arrows
byte up[] = {
 B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
  B00000
};

byte right[] = {
  B00000,
  B00000,
  B00000,
  B00100,
  B00110,
  B11111,
  B00110,
  B00100
};


byte left[] = {
  B00000,
  B00000,
  B00000,
  B00100,
  B01100,
  B11111,
  B01100,
  B00100
};

//sensor
byte sensor[] = {
  B00000,
  B01110,
  B10001,
  B00100,
  B01010,
  B00000,
  B00100,
  B01110
};

int batteryAddr = 0;
int sensorAddr = 1;
int upAddr = 2;
int leftAddr = 3;
int rightAddr = 4;

int changeModeBtn = A0;

void setup() {
  lcd.begin(16, 2);
  lcd.createChar(batteryAddr, battery);
  lcd.createChar(sensorAddr, sensor);
  lcd.createChar(upAddr, up);
  lcd.createChar(leftAddr, left);
  lcd.createChar(rightAddr, right);
}

void loop() {
  changeMode();
  display();
}

void changeMode() {
  Mode previousMode = mode;
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
  if(previousMode != mode) {
    lcd.clear();
  }
}

void display() {
  lcd.setCursor(0,1);
  switch(mode) {
    case sensorMode : {
      lcd.print ("sensors ");
      lcd.write (byte(sensorAddr));
      break;
    }
    case spaceMappingMode : {
      lcd.print ("space map ");
      lcd.write (byte(leftAddr));
      lcd.write (byte(upAddr));
      lcd.write (byte(rightAddr));
      break; 
    }
    case lowPowerMode : {
      lcd.print ("low power ");
      lcd.write (byte(batteryAddr));
      break;
    }
  }
}
