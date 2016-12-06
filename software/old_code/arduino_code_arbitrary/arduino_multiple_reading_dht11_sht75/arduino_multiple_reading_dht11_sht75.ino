#include <Sensirion.h>
#include "DHT.h"
unsigned long time;

#define DHT1PIN 4
#define DHT2PIN 7

#define DHT1TYPE DHT11
#define DHT2TYPE DHT11

DHT dht1(DHT1PIN, DHT1TYPE);
DHT dht2(DHT2PIN, DHT2TYPE);

const uint8_t dataPin =  9;              // SHT serial data
const uint8_t sclkPin =  8;              // SHT serial clock
const uint8_t ledPin  = 13;              // Arduino built-in LED
const uint32_t TRHSTEP   = 1000UL;       // Sensor query period
const uint32_t BLINKSTEP =  250UL;       // LED blink period

Sensirion sht = Sensirion(dataPin, sclkPin);

uint16_t rawData;
float temperature;
float humidity;
float dewpoint;

byte ledState = 0;
byte measActive = false;
byte measType = TEMP;

unsigned long trhMillis = 0;             // Time interval tracking
unsigned long blinkMillis = 0;

void setup() {
  dht1.begin();
  dht2.begin();
  Serial.begin(9600);
  Serial.println("In.(%) In.Tem(*C) Out.(%) Time");
  pinMode(ledPin, OUTPUT);
  delay(15);                           // Wait >= 11 ms before first cmd
  // Demonstrate blocking calls
  sht.measTemp(&rawData);              // sht.meas(TEMP, &rawData, BLOCK)
  temperature = sht.calcTemp(rawData);
  sht.measHumi(&rawData);              // sht.meas(HUMI, &rawData, BLOCK)
  humidity = sht.calcHumi(rawData, temperature);
  dewpoint = sht.calcDewpoint(humidity, temperature);
  logData();
}

void loop() {



  unsigned long curMillis = millis();          // Get current time

  // Rapidly blink LED.  Blocking calls take too long to allow this.
  if (curMillis - blinkMillis >= BLINKSTEP) {  // Time to toggle the LED state?
    ledState ^= 1;
    digitalWrite(ledPin, ledState);
    blinkMillis = curMillis;
  }

  // Demonstrate non-blocking calls
  if (curMillis - trhMillis >= TRHSTEP) {      // Time for new measurements?
    measActive = true;
    measType = TEMP;
    sht.meas(TEMP, &rawData, NONBLOCK);      // Start temp measurement
    trhMillis = curMillis;
  }
  if (measActive && sht.measRdy()) {           // Note: no error checking
    if (measType == TEMP) {                  // Process temp or humi?
      measType = HUMI;
      temperature = sht.calcTemp(rawData); // Convert raw sensor data
      sht.meas(HUMI, &rawData, NONBLOCK);  // Start humidity measurement

      float h2 = dht2.readHumidity();
      float t2 = dht2.readTemperature();


      Serial.print(humidity);
      Serial.print(" \t");
      Serial.print(temperature);
      Serial.print(" \t");

      if (isnan(t2) || isnan(h2)) {
        Serial.println("Failed to read from DHT #1");
      } else {
        Serial.print(h2);
        Serial.print(" \t");
        float seconds = curMillis  / 1000.0;
        Serial.println(seconds);
      }
    }
    else {
      measActive = false;
      humidity = sht.calcHumi(rawData, temperature); // Convert raw sensor data
      dewpoint = sht.calcDewpoint(humidity, temperature);
      logData();
    }
  }
}

void logData() {
}
