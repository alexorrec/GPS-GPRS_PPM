#include <TinyGPS++.h>

#define SERIAL_BUFFER_SIZE 256
#define SERIAL1_BUFFER_SIZE 256

TinyGPSPlus gps;

long int previousMillis = -60000;
long int  interval = 60000; //INVIO DATI OGNI MIN
String lat, lng, alt, spd;

void setup(){
  //Serial Monitor
  Serial.begin(9600);
  
  //SIM Start
  Serial1.begin(2400);
  
  //GPS Start
  Serial2.begin(9600);

  Serial.println("SERIALS STARTED...");
  delay(500);

  //Clean the input buffer
  while(Serial1.available() > 0) Serial1.readString();
  
  execute_AT("AT");
  execute_AT("AT+CFUN=1");
  delay(500);
  while(Serial1.available() > 0) Serial1.readString();

  /*Serial.println("Checking GPRS...");
  boolean is_connected = false;
  while(is_connected == false){
    is_connected = check_GPRS();
    delay(1000);
  }*/

  Serial.println("Checking GPS...");
  boolean has_fix = false;
  while(has_fix==false){
    has_fix = gps_has_fix();
  }
  Serial.println("GPS Ready!");
}

void loop(){

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval){
    if(gps_has_fix()){
      previousMillis = currentMillis;
      post_mySQL();
    }else
      Serial.println("GPS NOT AVAILABLE...");
  }
}

boolean gps_has_fix(){
  for (unsigned long start = millis(); millis() - start < 2000;){
      while (Serial2.available()){
        if (gps.encode(Serial2.read())){
          lat = get_latitude();
          lng = get_longitude();
          alt = get_altitude();
          spd = get_speed();
          if(lat.equals(String("0.000000"))){
            return false;
          }else
            return true;
        }
      }
  }
}

String get_latitude(){
  return String(gps.location.lat(), 6);
}

String get_longitude(){
  return String(gps.location.lng(), 6);
}

int get_altitude(){
  return gps.altitude.meters();
}

int get_speed(){
  return gps.speed.kmph();
}

boolean check_GPRS(){
  execute_AT("AT+CPIN?");
  execute_AT("AT+CSTT=\"TM\",\"\",\"\"");
  String x = execute_AT("AT+CIICR");
  //execute_AT("AT+CIFSR");
  if(x.equals(String("OK"))){
    Serial.println("Connected!");
    return true;
  }
  else{
    Serial.println("Network not Ready...");
    return false;
  }
}

String get_url(){

  String url = "\"";
  url += "https://gsmgpsppm.000webhostapp.com/gpsdata.php?lat=";
  url += lat;
  url += "&lng=";
  url += lng;
  url += "&alt=";
  url += alt;
  url += "&spd=";
  url += spd;
  url += "\"";
  
  return url;
}

void post_mySQL(){
  
  String url = get_url();
  execute_AT("AT+CFUN=1");
  execute_AT("AT+CGATT=1");
  execute_AT("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  execute_AT("AT+SAPBR=3,1,\"APN\",\"TM\"");
  execute_AT("AT+SAPBR=3,1,\"USER\",\"\"");
  execute_AT("AT+SAPBR=3,1,\"PWD\",\"\"");
  execute_AT("AT+SAPBR=0,1");
  execute_AT("AT+SAPBR=1,1");
  execute_AT("AT+SAPBR=2,1");
  execute_AT("AT+HTTPINIT");
  execute_AT("AT+HTTPSSL=1");
  execute_AT("AT+HTTPPARA=\"CID\",1");
  execute_AT("AT+HTTPPARA=\"URL\","+url);
  execute_AT("AT+HTTPACTION=0");
  delay(6000);
  execute_AT("AT+HTTPREAD");
  execute_AT("AT+HTTPTERM");
  execute_AT("AT+CIPSHUT");
}

String execute_AT(String AT){
  boolean has_answered = false;
  String response;
  Serial1.flush();
  delay(500);
  //Clean the input buffer
  while(Serial1.available() > 0) Serial1.read();
  
  Serial1.println(AT);
  
  do{
    if(Serial1.available() > 0){
      has_answered = true;
      response = Serial1.readString();
    }
        
  }while(has_answered == false);

  //Clean the input buffer
  while(Serial.available() > 0) Serial.readString();
  
  Serial.println(response);
  
  Serial1.flush();
  Serial.flush();
  return response;
}
