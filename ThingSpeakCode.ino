//#include <Adafruit_Circuit_Playground.h>
//#include <Adafruit_CircuitPlayground.h>

#include <Adafruit_Sensor.h>

#include <DHT.h>
#include <DHT_U.h>

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <stdlib.h>
//#include <DHT.h>

#define DEBUG true
#define DHTPIN 7                 
#define SSID "AndroidAP0388"     // "SSID-WiFiname"
#define PASS "rgad9976" // "password"
#define IP "184.106.153.149"      // thingspeak.com ip

String msg = "GET /update?key=GEZTZAQ3SWG5CYGW"; //Your ThingSpeak API key
SoftwareSerial esp8266(8,9); //Rx,Tx
LiquidCrystal lcd(12,11,5,4,3,2);
//DHT dht;

 #define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

//Variables
int chk;
float temp;
int hum;
int sensorValue;
int pResistor;
int error;
void setup()
{
  lcd.begin(16, 2);
  lcd.print("Smart Garden");
  delay(100);
  lcd.setCursor(0,1);
  lcd.print("Connecting...");
  Serial.begin(115200); 
  esp8266.begin(115200);
  Serial.println("AT");
  esp8266.println("AT");

  delay(1000);

  if(esp8266.find("OK"))
  {
    connectWiFi();
  }
}

void loop()
{
  lcd.clear();
  //lcd.autoscroll(); 
  start: //label
    error=0;
    int chk = dht.read(DHTPIN);
    temp = dht.readTemperature();
    hum = dht.readHumidity();
    sensorValue = analogRead(A0); 
    pResistor = analogRead(A1);
    lcd.setCursor(0, 0);
    lcd.print("T= ");
    lcd.print(temp);
    lcd.print("C");
    lcd.print(" ");
    lcd.print("M= ");
    lcd.print(sensorValue);
    lcd.print(" ");
    delay (100);
    lcd.setCursor(0, 1);
    lcd.print("H= ");
    lcd.print(hum);
    lcd.print("%");
    lcd.print(" ");
    lcd.print("L= ");
    lcd.print(pResistor);
    lcd.print(" ");
    delay(1000);
    updateTemp();
    //Resend if transmission is not completed
    if (error==1)
    {
      goto start; //go to label "start"
    } 
  delay(3600);
}

void updateTemp()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(2000);
  if(esp8266.find("Error"))
  {
    return;
  }
  cmd = msg ;
  cmd += "&field1=";    //field 1 for temperature
  cmd += temp;
  cmd += "&field2=";  //field 2 for humidity
  cmd += hum;
  cmd += "&field3=";    //field 3 for moisture 
  cmd += sensorValue;
  cmd += "&field4=";  //field 4 for light
  cmd += pResistor;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  esp8266.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  esp8266.println(cmd.length());
  if(esp8266.find(">"))
  {
    Serial.print(cmd);
    esp8266.print(cmd);
  }
  else
  {
    Serial.println("AT+CIPCLOSE");
    esp8266.println("AT+CIPCLOSE");
    //Resend...
    error=1;
  }

}
boolean connectWiFi()
{
  Serial.println("AT+CWMODE=3");
  esp8266.println("AT+CWMODE=3");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(5000);
  if(esp8266.find("OK"))
  {
    return true;
  }
  else
  {
    return false;
  }
}
