/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
#define pinSoilHumidity A0

/**
 * vordefinierte statische Werte
 */

#define maxValueTemp 30.5
#define maxValueAirHumidity 35
#define maxValueSoilHumidity 267
#define sensorSoilHumidityDry 400 // 595 // value for dry sensor
#define sensorSoilHumidityWet 239 // value for wet sensor
#define valueRefresh 256
/**
 * festgelegte einstellbare Werte
 */
 
int zahler;
float humi;
float tempC;
float tempF;
int valueSoilHumidity;

/**
 * 
 */

DHT dht(DHTPIN, DHTTYPE);

// Set LED_BUILTIN if it is not defined by Arduino framework
// #define LED_BUILTIN 13

void setup()
{
  // Add your initialization code here
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin(); // initialize the sensor
  zahler = 1;
  humi = 0;
  tempC = 0;
  tempF = 0;
  valueSoilHumidity = 0;
}

void readTempSoilHumidity()
{
  // read humidity
  humi = dht.readHumidity();
  // read temperature as Celsius
  tempC = dht.readTemperature();
  // read temperature as Fahrenheit
  tempF = dht.readTemperature(true);
}

void readAirHumidity()
{
  valueSoilHumidity = analogRead(pinSoilHumidity); // Auslesen des aktuellen Sensorwertes
}

void checkZahler()
{
  zahler++;

  if (zahler == INT8_MAX)
  {
    zahler = 1;
  }
}

void addZeros(){
  if (zahler <= 9) {
    Serial.print("0000");
  } else if (zahler <= 99) {
    Serial.print("000");
  } else if (zahler <= 999) {
    Serial.print("00");
  } else if (zahler <= 9999) {
    Serial.print("0");
  } else if (zahler <= 99999) {
    Serial.print("");
  } else {
    Serial.println("Read Error - illegal value");
  }
}

void printSerial()
{

  // check if any reads failed
  if (isnan(humi) || isnan(tempC) || isnan(tempF))
  {
    Serial.println("Failed to read from DHT sensor!");
  }
  else
  {
    addZeros();

    Serial.print(zahler);
    Serial.print("/");
    Serial.print(INT16_MAX);

    Serial.print("  |  ");

    Serial.print("Soil Humidity: ");
    Serial.print(valueSoilHumidity);
    Serial.print("");

    Serial.print("  |  ");

    Serial.print("Air Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C ~ ");
    Serial.print(tempF);
    Serial.println("°F");
  }
}

// The loop function is called in an endless loop
void loop()
{
  delay(valueRefresh); // kurze Pause

  readAirHumidity();
  readTempSoilHumidity();

  // 267 Bodenfeuchtigkeit
  if (valueSoilHumidity >= sensorSoilHumidityDry)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }

  printSerial();
  checkZahler();

}