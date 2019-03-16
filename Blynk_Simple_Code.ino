#include <SimpleTimer.h>

#include <Adafruit_Sensor.h>

#include <DHT.h>
#include <DHT_U.h>

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
//#include <SimpleTimer.h>
//#include <dht.h>


char auth[] = " 82bb34c4ecd14bdd9abae800906d96e1"; //Blynk Authentication code
char ssid[] = "AndroidAP0388"; //SSID Name
char pass[] = "rgad9976"; //Password

#include <SoftwareSerial.h>
SoftwareSerial EspSerial(8,9); // RX, TX

#define ESP8266_BAUD 115200
//#define IPAddress "192.168.1.100" 

ESP8266 wifi(&EspSerial);

#define DHT11_PIN 7          
#define DHTTYPE DHT11 
DHT dht(DHT11_PIN, DHTTYPE);
//dht DHT;
SimpleTimer timer;
void sendSensor()
{
  int chk = dht.read(DHT11_PIN);
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  int s = analogRead(A0); 
  int p = analogRead(A1);

  if (isnan(h) || isnan(t) || isnan(s) || isnan(p)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, h);
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, s);
  Blynk.virtualWrite(V3, p);
}

void setup()
{
  
  Serial.begin(115200);

 
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
 // Blynk.begin(auth, wifi, ssid, pass, "blynk-cloud.com", 8442);
// Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8442);
 Blynk.email( "SMART GARDEN", "Your garden is online.");

  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates SimpleTimer
}
