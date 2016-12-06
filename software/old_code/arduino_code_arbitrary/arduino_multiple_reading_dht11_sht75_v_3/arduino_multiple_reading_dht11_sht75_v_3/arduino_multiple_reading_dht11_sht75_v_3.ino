#include <Sensirion.h> //SHT75 related library
#include "DHT.h" //DHT11 related library
#include <Wire.h> //BME280 related library
#include <SPI.h> //BME280 and MAX31855 related library
#include <Adafruit_Sensor.h> //BME280 related library
#include <Adafruit_BME280.h> //BME280 related library
#include "Adafruit_MAX31855.h"
unsigned long time;

#define DHT1PIN 4 //Define DHT11 Output
#define DHT2PIN 7 //Define DHT11 Output

#define DHT1TYPE DHT11
#define DHT2TYPE DHT11

//BME280 Define
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

//initialize the BME280
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

//Thermocouple reading define
#define MAXDO   3
#define MAXCS   6
#define MAXCLK  5

// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
#define Serial SerialUSB
#endif

//initialize the DHT11
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
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  dht1.begin();
  dht2.begin();
  Serial.begin(9600);
  //Serial.println("In.(%) In.Tem(*C) Pressure Thermocouple Out.(%) Time");
  pinMode(ledPin, OUTPUT);
  delay(15);                           // Wait >= 11 ms before first cmd
  // Demonstrate blocking calls
  sht.measTemp(&rawData);              // sht.meas(TEMP, &rawData, BLOCK)
  temperature = sht.calcTemp(rawData);
  sht.measHumi(&rawData);              // sht.meas(HUMI, &rawData, BLOCK)
  humidity = sht.calcHumi(rawData, temperature);
  dewpoint = sht.calcDewpoint(humidity, temperature);
  logData();
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
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

      Serial.print(bme.readPressure() / 100.0F);
      double c = thermocouple.readCelsius();
      Serial.print(" \t");
      if (isnan(c)) {
        Serial.println("Something wrong with thermocouple!");
      } else {
        Serial.println(c);
      }
      if (isnan(t2) || isnan(h2)) {
        Serial.println("Failed to read from DHT #1");
      } else {
        //Serial.print(h2);
        //Serial.print(" \t");
        float seconds = curMillis  / 1000.0;
        //Serial.println(seconds);
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
