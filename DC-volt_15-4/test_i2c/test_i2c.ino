#include <Wire.h>
 #include "Adafruit_INA219.h"
#include <LiquidCrystal_I2C.h> // LCD
Adafruit_INA219 ina219;
#define SDA_2 33
#define SCL_2 32
int lcdColumns = 16;
int lcdRows = 2;
// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
void setup() {
  Serial.begin(115200);

   Wire.begin(); //uses default SDA and SCL and 100000HZ freq
  Wire1.begin(SDA_2, SCL_2);
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  if (! ina219.begin(&Wire1)) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  Serial.println();
}
void LCD(){
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("Hello, World!");
  Serial.print("Hello, World!");
  delay(1000);
  // clears the display to print new message
  lcd.clear();
  // set cursor to first column, second row
  lcd.setCursor(0,1);
  delay(1000);
  lcd.clear(); 
}
void loop() { 
 
  LCD();
   float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;
  for(int i=0;i<10;i++){
  shuntvoltage += ina219.getShuntVoltage_mV();
  busvoltage += ina219.getBusVoltage_V();
  current_mA += ina219.getCurrent_mA();
  power_mW += ina219.getPower_mW();
  loadvoltage += busvoltage + (shuntvoltage / 1000);
  }
  busvoltage=busvoltage/10;
  shuntvoltage=shuntvoltage/10;
  loadvoltage=loadvoltage/10;
  current_mA=current_mA/10;
  power_mW=power_mW/10;
   Serial.print(busvoltage); Serial.print("\t"); 
  Serial.print(shuntvoltage); Serial.print("\t");
  Serial.print(loadvoltage); Serial.print("\t");
  Serial.print(current_mA); Serial.print("\t");
  Serial.println(power_mW);
  delay(5000);
}
