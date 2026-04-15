#include <Servo.h> 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 

Servo myservo;

#define ir_enter 2
#define ir_back  4

#define ir_car1 5
#define ir_car2 6
#define ir_car3 7

int S1 = 0, S2 = 0, S3 = 0;
int flag1 = 0, flag2 = 0;
int slot = 3;

void setup() {
  Serial.begin(9600);

  pinMode(ir_car1, INPUT);
  pinMode(ir_car2, INPUT);
  pinMode(ir_car3, INPUT);
  pinMode(ir_enter, INPUT);
  pinMode(ir_back, INPUT);

  myservo.attach(3);
  myservo.write(90);

  lcd.init();
  lcd.backlight();

  Read_Sensor();
  slot = slot - (S1 + S2 + S3);
}

void loop() {
  Read_Sensor();

  // --- Row 0: Empty slots ---
  lcd.setCursor(0, 0);
  lcd.print("Empty: ");
  if (S1 == 0) lcd.print("S1 "); else lcd.print("   ");
  if (S2 == 0) lcd.print("S2 "); else lcd.print("   ");
  if (S3 == 0) lcd.print("S3 "); else lcd.print("   ");

  // --- Row 1: Full slots ---
  lcd.setCursor(0, 1);
  lcd.print("Full:  ");
  if (S1 == 1) lcd.print("S1 "); else lcd.print("   ");
  if (S2 == 1) lcd.print("S2 "); else lcd.print("   ");
  if (S3 == 1) lcd.print("S3 "); else lcd.print("   ");

  // --- Entry Logic ---
  if (digitalRead(ir_enter) == 0 && flag1 == 0) {
    if (slot > 0) {
      flag1 = 1;
      if (flag2 == 0) {
        myservo.write(180);
        slot = slot - 1;
      }
    } else {
      lcd.setCursor(0, 0);
      lcd.print(" Parking Full!  ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      delay(1500);
    }
  }

  // --- Exit Logic ---
  if (digitalRead(ir_back) == 0 && flag2 == 0) {
    flag2 = 1;
    if (flag1 == 0) {
      myservo.write(180);
      slot = slot + 1;
    }
  }

  // --- Close Gate ---
  if (flag1 == 1 && flag2 == 1) {
    delay(1000);
    myservo.write(90);
    flag1 = 0;
    flag2 = 0;
  }

  delay(500); // slow update
}

void Read_Sensor() {
  S1 = (digitalRead(ir_car1) == 0) ? 1 : 0;
  S2 = (digitalRead(ir_car2) == 0) ? 1 : 0;
  S3 = (digitalRead(ir_car3) == 0) ? 1 : 0;
}
