#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif
#include <Wire.h>
#include "ESP32_MailClient.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
// REPLACE WITH YOUR NETWORK CREDENTIALS

#define emailSenderAccount    "ghuyniot@gmail.com"    
#define emailSenderPassword   "weekndcold1"
#define emailRecipient        "nguyenkhaikh321@gmail.com"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "NodeWifi32 Test"
#define RXD2 16
#define TXD2 17
#define LED 2
// The Email Sending data object contains config and data to send
SMTPData smtpData;
// Replace with your network credentials
const char *ssid     = "Sinh Vien Home";
const char *password = "06117370";
const char* host = "http://dighuyniot.tech";
String get_host = "http://dighuyniot.tech";
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 3000000; 
int check=0;
char buff[10];
char str1='1';
char str0='0';
char str3='3';
char str2='2';
char str5='5';
char str4='4';
char str7='7';
char str6='6';
volatile byte indx;
const char* serverName = "http://dighuyniot.tech/demo/postdemo1.php";
const char* serverName1 = "http://dighuyniot.tech/demo/alert/get_datarequest/postdemo1.php";
String apiKeyValue = "tPmAT5Ab3j7F9";
//////////////////hen gio
String time_status;
String humid_status;
int value1_status;
int value2_status;
int value3_status;
int solan=0;
int ktra=0;
//const unsigned long periodBom = 30000; 
unsigned long bat;
unsigned long tat;
unsigned long current;
unsigned long start;

//
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
//////////////
void sendCallback(SendStatus info);
WiFiServer server(80);

void setup() {
  // Initialize Serial Monitor
// Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
    Serial.begin(115200);
    //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
    pinMode(LED,OUTPUT);
  WiFi.begin(ssid, password);
    //starting the server
    server.begin();
    startMillis = millis();
    //////////hengio
    // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  //GMT +7 = 25200
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(25200);
    /////////////
  
}
boolean checkArrays(char arrayA[],char arrayB, long numItems) {

    boolean same = true;
    long i = 0;
    while(i<numItems ) { 
      same = arrayA[i] == arrayB;
      i++;
      if(same){return same;}
    }
    return same;
  }
  void post_btn(String value,String device){
  //Check WiFi connection status
    WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + value
                           + "&value2=" + device + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
    
    if (httpResponseCode>0) {
      String respone=http.getString();
      Serial.println(respone);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  
 //Send an HTTP POST request every 30 seconds
}
void get_send_mail(String value, String noidung)
  {
    
    WiFiClient client = server.available();
 
        HTTPClient http;
        String url = get_host+"/demo/mail/get_status.php?value="+value;
        
        http.begin(url);
        Serial.println(url);
        //GET method
        int httpCode = http.GET();
        String payload = http.getString();
        Serial.println("sssssssss");
        Serial.println(payload);
        payload.trim();
        int result=atoi(payload.c_str());
        
        if(value=="value1" )
          {
           value1_status=result;
           if( result>=100 && check==0){
           check=1;
           send_mail(noidung);
           }
          }
        else if(value=="value2" )
          {
           value2_status=result;
           if( result>=100 && check==0){
           check=1;
           send_mail(noidung);
           }
          }
        else if (value=="value3" )
          {
           value3_status=result;
           if( result>=100 && check==0){
           check=1;
           send_mail(noidung);
           }
          }
       
        http.end();
       
  
  }
  void send_mail(String noidung)
  {
    // Set the SMTP Server Email host, port, account and password
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
  // Set the sender name and Email
  smtpData.setSender("SmartGarden", emailSenderAccount);
  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");
  // Set the subject
  smtpData.setSubject(emailSubject);
  // Set the message with HTML format
  smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>"+noidung+"</p></div>", true);
  // Set the email message in text format (raw)
  //smtpData.setMessage("Hello World! - Sent from NodeWifi32 board", false);
  // Add recipients, you can add more than one recipient
  smtpData.addRecipient(emailRecipient);
  //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");
  smtpData.setSendCallback(sendCallback);
  //Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
  //Clear all data from Email object to free memory
  smtpData.empty();
  }
  // Callback function to get the Email sending status
void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());
  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
}
void get_device_status(String device_name)
  {
    
    WiFiClient client = server.available();
 
        HTTPClient http;
        String url = get_host+"/demo/control/get_status.php?device_name="+device_name;
        
        http.begin(url);
        Serial.println(url);
        //GET method
        int httpCode = http.GET();
        String payload = http.getString();
        Serial.println("sssssssss");
        Serial.println(payload);
        payload.trim();
        if(payload=="onl1")
        {
          Serial2.println("1"); 
        }
        else if (payload=="offl1")
        {
          Serial2.println("2");    
        }
        else if (payload=="onl2")
        {
          Serial2.println("3");    
        }
        else if (payload=="offl2")
        {
          Serial2.println("4");    
        }
        else if (payload== "onp1")
        {
          Serial2.println("5");    
        }
        else if (payload== "offp1")
        {
          Serial2.println("6");    
        }
        else if (payload== "onp2")
        {
          Serial2.println("7");    
        }
        else if (payload=="offp2")
        {
          Serial2.println("8");    
        }
   
          http.end();
  
  }
int get_solan_status(String device_name)
  {
    
    WiFiClient client = server.available();
 
        HTTPClient http;
        String url = get_host+"/demo/alert/get_datarequest/get_status.php?device_name="+device_name;
        
        http.begin(url);
        Serial.println(url);
        //GET method
        int httpCode = http.GET();
        String payload = http.getString();
        Serial.println("sssssssss");
        Serial.println(payload);
        payload.trim();
         http.end();
      return atoi(payload.c_str());
  }  
void post_solan(int solan, String bom){
  //Check WiFi connection status
    WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName1);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(solan)
                           + "&value2=" + bom + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
    
    if (httpResponseCode>0) {
      String respone=http.getString();
      Serial.println(respone);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  
 //Send an HTTP POST request every 30 seconds
}
void get_status_time(String value){
    WiFiClient client = server.available();
        
        HTTPClient http;
        String url = get_host+"/demo/alert/get_datarequest/get_status_time.php?bom="+value;
        http.begin(url);
        Serial.println(url);
        //GET method
        int httpCode = http.GET();
        String payload = http.getString();
        Serial.println("sssssssss");
        Serial.println(payload);
        payload.trim();
        time_status=payload;
        http.end();
}
int get_status_Bom(String value){
    WiFiClient client = server.available();
         
        HTTPClient http;
        String url = get_host+"/demo/alert/get_datarequest/get_status_timeBom.php?bom="+value;
        http.begin(url);
        Serial.println(url);
        //GET method
        int httpCode = http.GET();
        String payload = http.getString();
        Serial.println("sssssssss");
        Serial.println(payload);
        payload.trim();
        int timeTotal=(payload[7]-'0')+(payload[6]-'0')*10+(payload[4]-'0')*60+(payload[3]-'0')*600;
        http.end(); 
        return timeTotal*1000;
}
void loop() {
  timeClient.update();
  String a = timeClient.getFormattedTime();
  if(a=="00:00:00"){
    solan=0;
   post_solan(solan,"light1");
   get_status_time("1");
  }
  get_status_time("1");
  solan=get_solan_status("light1");
  get_send_mail("value1","Nhiet Do Qua Cao");
   get_send_mail("value3","Do Am Qua Cao");
   //if(time_status==a&&solan==0&&value3_status<73){
   if(time_status==a&&solan==0){
    solan++;
    post_solan(solan,"light1");
    post_btn("onl1","light1");
    bat=millis();
    ktra=1;
   }
 int periodBom=get_status_Bom("1");
   if(millis()-bat>periodBom&&ktra==1){
     post_btn("offl1","light1");
     Serial.println("tat"); 
     ktra=0;
   }
////////////////////////////////gửi dữ liệu liên tục
  for(int i=0;i<10;i++){
  if(Serial2.available()>0){
    char c=Serial2.read();
    if(indx<sizeof buff){
      buff[indx++]=c;
      Serial.println(c);
      if(c=='x'){
        Serial.println(buff);
      if(checkArrays(buff,str1,10))
      {
         post_btn("onl1","light1");
      }
      else if(checkArrays(buff,str0,10))
      {
         post_btn("offl1","light1");
      }
       if(checkArrays(buff,str3,10))
      {
         post_btn("onl2","light2");
      }
      else if(checkArrays(buff,str2,10))
      {
         post_btn("offl2","light2");
      }
       if(checkArrays(buff,str5,10))
      {
         post_btn("onp1","pump1");
      }
      else if(checkArrays(buff,str4,10))
      {
         post_btn("offp1","pump1");
      }
      if(checkArrays(buff,str7,10))
      {
         post_btn("onp2","pump2");
      }
     else if(checkArrays(buff,str6,10))
      {
        post_btn("offp2","pump2");    
      }
        indx=0;
      }
    }else{indx=0;}
   }
  }
  get_device_status("light1");
  get_device_status("light2");
  get_device_status("pump1");
  get_device_status("pump2");
   currentMillis = millis();
   if(currentMillis-startMillis>= period){
    check=0;
    startMillis = currentMillis;
   }
  if(current-start>= 300000){
    solan=get_solan_status("light1");
    start = current;
   }
 }
