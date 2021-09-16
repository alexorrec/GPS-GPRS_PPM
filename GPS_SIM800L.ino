#include <SomeSerial.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;

long int previousMillis = -60000;
long int  interval = 60000; //INVIO DATI OGNI MIN
String lat, lng;
int alt, spd;

void setup(){
  //Serial Monitor
  Serial.begin(9600);
  
  //SIM Start
  Serial1.begin(9600);
  
  //GPS Start
  Serial2.begin(9600);

  Serial1.write(27);
  Serial1.write(27);
  Serial1.write(27);

  Serial.println("SERIALS STARTED...");
  
  execute_AT("AT");
  execute_AT("AT+CFUN=1");
  execute_AT("AT+CPIN?");
  execute_AT("AT+CSTT=\"ibox.tim.it\",\"\",\"\"");
  execute_AT("AT+CIICR");
  while(Serial1.available() > 0) Serial1.readString();

  
  Serial.println("Checking GPRS...");
  boolean is_connected = false;
  while(is_connected == false){
    is_connected = check_GPRS();
    delay(1000);
  }

  
  //Serial.println("Checking GPS...");
  boolean has_fix = false;
  while(has_fix==false){
    Serial.println("Checking GPS...");
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
  execute_AT("AT+CSTT=\"ibox.tim.it\",\"\",\"\"");
  String x = execute_AT("AT+CIICR");
  //Serial.println("TEST Stampa:"+ x);
  //execute_AT("AT+CIFSR");
  if(x == "OK"){
    Serial.println("Connected!");
    execute_AT("AT+CIPSHUT");
    return true;
  }
  else{
    Serial.println("Network not Ready...");
    return false;
  }
}

String get_url(){
  String url = "\"";
  url += "https://gsmgpsppm.000webhostapp.com/server_configs/gpsdata.php?lat=";
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
  execute_AT("AT+SAPBR=3,1,\"APN\",\"ibox.tim.it\"");
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
  delay(10000);
  Serial1.write(27);
  Serial1.write(27);
  Serial1.write(27);
  Serial1.print("\r\n");
  execute_AT("AT+HTTPREAD");
  execute_AT("AT+HTTPTERM");
  execute_AT("AT+CIPSHUT");
}

String execute_AT(String AT){
  Serial1.write(27);
  Serial1.write(27);
  Serial1.write(27);
  Serial1.print("\r\n");
  
  boolean has_answered = false;
  String response;
  Serial1.flush();
  
  Serial1.println(AT);
  
  do{
    if(Serial1.available() > 0){
      has_answered = true;
      response = Serial1.readString();
    }
        
  }while(has_answered == false);
  
  Serial.println(response);
  
  Serial1.flush();
  Serial.flush();
  Serial1.write(27);
  Serial1.write(27);
  Serial1.write(27);
  Serial1.print("\r\n");
  return response;
}
