#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h> //BME280 library
#include <Adafruit_MAX31855.h> //MAX31855 library

#define BME_SCK 13 //BME SCK connection
#define BME_MISO 12 //BME MISO connection
#define BME_MOSI 11 //BME MOSI connection
#define BME_CS 10 ////BME CS connection
#define MAXDO   5 //MAX31855 DO connection
#define MAXCS   6 //MAX31855 CS connection
#define MAXCLK  7 //MAX31855 CLK connection

Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK); //initialize the BME280
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO); //initialize the Thermocouple

const uint32_t trhStep = 1000UL; //Waiting time
unsigned long trhMillis = 0; //Time interval tracking

void setup() {
  Serial.begin(9600);
  delay(500); //wait for MAX chip to stabilize

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  unsigned long curMillis = millis(); // Get current time
  if (curMillis - trhMillis >= trhStep) {      // Time for new measurements?
    Serial.print(bme.readHumidity()); //Get Humidity in %
    Serial.print(" \t");
    Serial.print(bme.readTemperature()); //Get Chamber Temperature in C*
    Serial.print(" \t");
    Serial.print(bme.readPressure() / 100.0F); //Get Pressure in hPa
    Serial.print(" \t");
    Serial.print(thermocouple.readCelsius()); //Get Thermocouple Temp. in C*
    Serial.println();
    trhMillis = curMillis;
  }

}
