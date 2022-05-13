#include "EmonLib.h"   //https://github.com/openenergymonitor/EmonLib
#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include "ESP32_MailClient.h"
 #include "Adafruit_INA219.h"
#include "EmonLib.h"
#include <LiquidCrystal_I2C.h> // LCD
 
EnergyMonitor emon;

#define vCalibration 40 //AC_VOLT_SENSOR
#define pinVoltAC 36 //chân volt AC
#define ANALOG_IN_PIN 34//chân DC voltage
//ina 219 mặc định chân 21,22
#define emailSenderAccount    "ghuyniot@gmail.com"    
#define emailSenderPassword   "weekndcold1"
#define emailRecipient        "nguyenkhaikh321@gmail.com"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "NodeWifi32 Test"
#define SDA_2 33 //I2C CỦA INA
#define SCL_2 32
 SMTPData smtpData;
#define LED 2 
String message1="Che do DC";
String message2= "Che do AC";
String message;
String message3="Ngan Mach";
String message4= "Ho Mach";
boolean pressed = true; //trạng thái nút nhấn ngắt
//////////DC volt///////////

// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;
 
// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0; 
 
// Float for Reference Voltage
float ref_voltage = 5.0;
 
// Integer for ADC value
int adc_value = 0;
///////////////////////////
String apiKeyValue = "tPmAT5Ab3j7F9";
Adafruit_INA219 ina219;
int check=0; //So cho 
int checkDCngan=0;
int checkDCho=0;
int checkACho=0;
 //Access point credentials
const char* ssid = "Sinh Vien Home";
const char* password = "06117370";
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 3000000; 
 WiFiServer server(80);  // open port 80 for server connection
void sendCallback(SendStatus info);

void IRAM_ATTR isr(){
  if (pressed){
    pressed=false;
  }else{pressed=true;}
}

void setup() {
  emon.voltage(pinVoltAC, vCalibration, 1.7); //ACvolt chân ra
    pinMode(LED,OUTPUT);
    pinMode(35, INPUT);//ACcurent
  WiFi.begin(ssid, password);
  setupLCD_isr();
    //starting the server
    server.begin();
    startMillis = millis();
    Wire1.begin(SDA_2, SCL_2);
    Serial.begin(9600);
  if (! ina219.begin(&Wire1)) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  delay(2000);
  }

void dodienap(){//do dien dap DC///////////////////////////////////////////////////////////////////////
  ////////////////////do dong
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
  ////////////////////////////////////DO AP
  // Read the Analog Input
   adc_value = analogRead(ANALOG_IN_PIN);
   
   // Determine voltage at ADC input
   adc_voltage  = (adc_value * ref_voltage) / 4096.0; 
   
   // Calculate voltage at divider input
   in_voltage = adc_voltage / (R2/(R1+R2)) ; 
   
   // Print results to Serial Monitor to 2 decimal places
  Serial.print("Input Voltage = ");
  Serial.println(in_voltage, 2);

  
    if (in_voltage==0 && loadvoltage<10 ){
      message=message3;
      if(check==0){
      send_mail("Ngan mach DC");
      comment("Mach hong","Ngan mach","0");
      check=1;
      }
    }else if (in_voltage!=0 &&loadvoltage<10){
      message=message4;
    if(checkDCho==0){
    comment("Mach hong","Ho mach","0");
    checkDCho=1;
    send_mail("Ho mach");
    }
    }else{
      message="Cong Suat"+String(power_mW)+"W";
    }
    
  delay(1000);
}

void loop() {
  
if(pressed){
   
   LCD(message1,message);
   delay(1000);
   digitalWrite(LED,HIGH);   
   currentMillis = millis();
   dodienap();
   if(currentMillis-startMillis>= period){
    check=0;
    checkDCngan=0;
    checkDCho=0;
    startMillis = currentMillis;
   }

  
}else{
  LCD(message2,message);
   myTimerEvent();
   digitalWrite(LED,LOW); 
   currentMillis = millis();
    if(currentMillis-startMillis>= period){
    checkACho=0;
    startMillis = currentMillis;
   }
   
}
}
