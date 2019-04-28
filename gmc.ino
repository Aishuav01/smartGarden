#include <DHT.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#define DHTPIN 4                       // Digital Pin 4
#define DHTTYPE DHT11     // We are Using DHT11
int VAL_PROBE = A0;       // Analog pin 0
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
String apiKey = "ZI2KJYRUQEXHTRYH";    // Edit this API key according to your Account
String Host_Name = "chinnu";         // Edit Host_Name
String Password = "21071998";          // Edit Password
SoftwareSerial ser(2,3);              // RX, TX
int i;
int j;
DHT dht(DHTPIN, DHTTYPE);              // Initialising Pin and Type of DHT
int potPin = A1;    // select the input pin for the potentiometer

int val = 0;       // variable to store the value coming from the sensor

void setup() {
  lcd.begin(16, 2);  
  lcd.print("Welcome");
  delay(1000); 
  lcd.clear();
   lcd.print("Tem: Hum:  Mo:");                       
   Serial.begin(115200);                  // enable software serial
  ser.begin(115200);                     // reset ESP8266
  ser.println("AT+RST");               // Resetting ESP8266
  ser.println("AT+CWMODE=3");
  dht.begin();                        // Enabling DHT11
  
char inv ='"';
String cmd = "AT+CWJAP";
       cmd+= "=";
       cmd+= inv;
       cmd+= Host_Name;
       cmd+= inv;
       cmd+= ",";
       cmd+= inv;
       cmd+= Password;
       cmd+= inv;
ser.println(cmd);                    // Connecting ESP8266 to your WiFi Router
Serial.println(cmd);                           // AT+CWJAP="Host_Name","Password"
ser.println("AT+CIFSR");
  pinMode(13,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);

}


void loop() {
  

  val = analogRead(potPin);    // read the value from the sensor

  int i,t,h,m;
  
  if(val<200)
  i=1;
  else if(val>200 && val<800)
  i=2;
  else
  i=3;
  switch(i)
  {
    case 1:{
      t=35;
      h=15;
      m=30;
      break;
      }
      case 2:{
        t=40;
        h=10;
        m=20;
        break;
      }
      case 3:{
        t=10;
        h=10;
        m=20;
        break;
      }
    }
   if(i!=j)
   {
    lcd.clear();
    lcd.print("mode=");
     lcd.setCursor(0, 1);
     lcd.print(i);
     delay(5000);
     lcd.clear();
     lcd.print("Tem: Hum:  Mo:");    
   }    
  Serial.print("mode=");
  Serial.println(i);
  int moisture = analogRead(VAL_PROBE); 
  int humidity =  dht.readHumidity();             // Reading Humidity Value
  int temperature = dht.readTemperature();  // Reading Temperature Value
    Serial.print("temperature =");
 
  Serial.println(temperature);
  Serial.print("Humidity =");
   Serial.println(humidity);
   moisture=(100-((moisture/1023.00)*100));
    if((moisture < m && humidity < h && temperature > t) ||(moisture < m  && temperature > t) || (moisture < m)) {
        digitalWrite(13,HIGH);
    } else   {
        digitalWrite(13,LOW);
    }
  Serial.print("moisture =");
  Serial.println(moisture);
  
  
  if(humidity > h) {
        digitalWrite(6,HIGH);
        
    } else   {
        digitalWrite(6,LOW);
    }
    
   if(temperature > t) {
        digitalWrite(5,HIGH);
    } else   {
        digitalWrite(5,LOW);
    }
  String state1=String(temperature);                 // Converting them to string 
  String state2=String(humidity);              // as to send it through URL
  String state3=String(moisture);
  String cmd = "AT+CIPSTART=\"TCP\",\"";          // Establishing TCP connection
  cmd += "184.106.153.149";                       // api.thingspeak.com
  cmd += "\",80";                                 // port 80
  ser.println(cmd);
  Serial.println(cmd);                            // AT+CIPSTART="TCP",184.106.153.149,"80"
  
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  String getStr = "GET /update?api_key= https://api.thingspeak.com/update?api_key=ZI2KJYRUQEXHTRYH&field1=0";         // prepare GET string
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(state1);                       // Temperature Data
  getStr +="&field2=";
  getStr += String(state2);                       // Humidity Data
   getStr +="&field3=";
  getStr += String(state3);
  getStr += "\r\n\r\n";
  
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());                // Total Length of data
  ser.println(cmd);
  Serial.println(cmd);
  if(ser.find(">")){\
    ser.print(getStr);
    Serial.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");                  // closing connection
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.setCursor(5,1);
  lcd.print(humidity);
  lcd.setCursor(10,1);
  lcd.print(moisture);
  j=i;
  delay(15000);                                  // Update after every 15 seconds
}
