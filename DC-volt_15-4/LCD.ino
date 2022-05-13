int lcdColumns = 16;
int lcdRows = 2;
// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
//I2C CỦA lcd MẶC ĐỊNH 21,22
void setupLCD_isr(){
  pinMode(4, INPUT_PULLUP);
  attachInterrupt(4, isr, FALLING);
  Wire.begin();
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
}

void LCD(String message,String message2){
  lcd.clear();
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print(message);
  // clears the display to print new message
 
  // set cursor to first column, second row
  lcd.setCursor(0,1);
  lcd.print(message2);
  
  //lcd.clear(); 
}
