#include "DHT.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int DHTPIN = 8;       
const int DHTTYPE = DHT11;  
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(9600);
  dht.begin(); 
  lcd.init();
  lcd.backlight();       
}
void loop() {
  float h = dht.readHumidity();    
  float t = dht.readTemperature(); 
  lcd.setCursor(0, 0); 
  lcd.print("Nhiet do: ");
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("Do am:   :");
  lcd.print(h);
}
