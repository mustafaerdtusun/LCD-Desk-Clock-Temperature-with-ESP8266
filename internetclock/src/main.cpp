#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <LiquidCrystal_I2C.h>
#include "DHT.h"    
#include <Arduino.h>

#define DHTPIN 12    // GPIO2 NodeMCU kart üzerinde D4 Pini !
#define DHTTYPE DHT11   // Kullandığımız sensörün tipi DHT 11
#define BLYNK_PRINT Serial 



LiquidCrystal_I2C lcd(0x27,16,2);

DHT dht(DHTPIN, DHTTYPE);


const char *ssid     = "wifissid";
const char *password = "wifipassword";

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){


  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  dht.begin();

  lcd.init();
  lcd.backlight();
  

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

void loop() {
  int h = dht.readHumidity();
  int t = dht.readTemperature();  
  Serial.print("Sicaklik:");
  Serial.println(t);
  Serial.print("Nem: ");
  Serial.println(h);

  
  timeClient.update();

  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours()+2);
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());

lcd.setCursor(0,0);

if(timeClient.getHours()+2<10){
  lcd.print("0");
}
lcd.print(timeClient.getHours()+2);
lcd.print(":");
if(timeClient.getMinutes()<10){
  lcd.print("0");
}
lcd.print(timeClient.getMinutes());
lcd.print(":");
if(timeClient.getSeconds()<10){
  lcd.print("0");
}
lcd.print(timeClient.getSeconds());
lcd.print(" ");
lcd.print(daysOfTheWeek[timeClient.getDay()]);


  
  lcd.setCursor(0,1);
  lcd.print(t);
  lcd.print("'C ");
  lcd.setCursor(5,1);
  lcd.print(h);
  lcd.print("%");
  

  delay(100);
}
