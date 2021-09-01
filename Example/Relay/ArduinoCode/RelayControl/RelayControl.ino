#include "DHT.h"

const int DHTPIN = 8;       
const int DHTTYPE = DHT11;  
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(13, OUTPUT);       
}

void loop() {
  float h = dht.readHumidity();    
  float t = dht.readTemperature(); 
  
  Serial.print("Nhiet do: ");
  Serial.println(t);
  Serial.print("Do am:   :");
  Serial.println(h);

  if (t > 30){
    digitalWrite(13, HIGH);
  }
  else{
    digitalWrite(13, LOW);
  }

  delay(1000);
}
