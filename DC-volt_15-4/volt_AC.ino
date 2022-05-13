unsigned long lastmillis = millis();

float kWh = 0;

void myTimerEvent() {
     Voltage = getVPP();
 VRMS = (Voltage/2.0) *0.707; // sq root
 AmpsRMS = (VRMS * 1000)/mVperAmp;
 float Wattage = (220*AmpsRMS)-135; //Observed 18-20 Watt when no load was connected, so substracting offset value to get real consumption.
 Serial.print(AmpsRMS);
 Serial.println(" Amps RMS ");
 Serial.print(Wattage); 
 Serial.println(" Watt ");//tinh dong AC


 
  kWh = kWh + emon.apparentPower * (millis() - lastmillis) / 3600000000.0;
  yield();
    emon.calcVI(20, 2000);
    Serial.print("Vrms: ");
    Serial.print(emon.Vrms, 2);
    Serial.print("V");
    Serial.print("\tIrms: ");
    Serial.print(emon.Irms, 4);
    Serial.print("A");
    Serial.print("\tPower: ");
    Serial.print(emon.apparentPower, 4);
    Serial.print("W");
    Serial.print("\tkWh: ");
  
    Serial.print(kWh, 4);
    Serial.println("kWh");
    lastmillis = millis();
    if ( emon.Vrms>=100&&Wattage<50){
      message=message4;
      if(checkACho==0){
      send_mail("Ho mach AC");
      comment("Canh Bao","Ho mach AC","0");
      checkACho=1;
      }
    }else if(emon.Vrms<=10&&Wattage<50){
      message=" ";
    }
    else{
      
      message="Cong suat:"+String(Wattage)+"W";
    }
    
}
