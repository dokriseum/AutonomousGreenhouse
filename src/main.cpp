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
const int RELAY_PIN = A7;

/**
 * vordefinierte statische Werte
 */

#define maxValueTemp 30.5
#define maxValueAirHumidity 35
#define maxValueSoilHumidity 267
#define sensorSoilHumidityDry 595 // 595 // value for dry sensor
#define sensorSoilHumidityWet 239 // value for wet sensor
#define valueRefresh 256
/**
 * festgelegte einstellbare Werte
 */
 
int zahler;
float valueAirHumidity;
float valueTempCelsius;
float valueTempFahrenheit;
int valueSoilHumidity;
int valuePercentageSoilHumididy;
int sensorSoilHumidityDryTest;

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
  pinMode(RELAY_PIN, OUTPUT);
  dht.begin(); // initialize the sensor
  zahler = 1;
  valueAirHumidity = 0;
  valueTempCelsius = 0;
  valueTempFahrenheit = 0;
  valueSoilHumidity = 0;
  valuePercentageSoilHumididy = 0;
  sensorSoilHumidityDryTest = 400;
}

void readTempAirHumidity()
{
  // read humidity
  valueAirHumidity = dht.readHumidity();
  // read temperature as Celsius
  valueTempCelsius = dht.readTemperature();
  // read temperature as Fahrenheit
  valueTempFahrenheit = dht.readTemperature(true);
}

void readSoilHumidity()
{
  valueSoilHumidity = analogRead(pinSoilHumidity); // Auslesen des aktuellen Sensorwertes
  valuePercentageSoilHumididy = map(valueSoilHumidity, sensorSoilHumidityWet, sensorSoilHumidityDry, 100, 0); 
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
  //The function isnan() returns 1 if the argument \a __x represents a "not-a-number" (NaN) object, otherwise 0.
  if (isnan(valueAirHumidity) || isnan(valueTempCelsius) || isnan(valueTempFahrenheit))
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
    Serial.print(" / ");
    Serial.print(valuePercentageSoilHumididy);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Air Humidity: ");
    Serial.print(valueAirHumidity);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(valueTempCelsius);
    Serial.print("°C ~ ");
    Serial.print(valueTempFahrenheit);
    Serial.println("°F");
  }
}

// The loop function is called in an endless loop
void loop()
{
  
  delay(valueRefresh); // kurze Pause

  readSoilHumidity();
  readTempAirHumidity();

  // 267 Bodenfeuchtigkeit
  if (valueSoilHumidity >= sensorSoilHumidityDryTest)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(RELAY_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(RELAY_PIN, LOW);
  }

  printSerial();
  checkZahler();
}