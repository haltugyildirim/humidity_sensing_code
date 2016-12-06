#include "DHT.h"

#define DHT1PIN 4
#define DHT2PIN 7

#define DHT1TYPE DHT11
#define DHT2TYPE DHT11

DHT dht1(DHT1PIN, DHT1TYPE); 
DHT dht2(DHT2PIN, DHT2TYPE);

void setup() { 
  Serial.begin(9600); 
  Serial.println("Chamber Outside Humidity-DHT11"); 
   
  dht1.begin(); 
   dht2.begin(); 
}

void loop() { 

  delay(2000);
  
  float h2 = dht2.readHumidity(); 
  float t2 = dht2.readTemperature();

  if (isnan(t2) || isnan(h2)) {
    Serial.println("Failed to read from DHT #2");
  } else { 
    Serial.print(h2); 
    Serial.print(" \t"); 
  } 
  Serial.println(); 
}
