/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Wire.h>
#include "Arduino.h"
#include "DHT.h"
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <EEPROM.h>
#define DHTPIN 2
#define DHTTYPE DHT11
#define pinSoilHumidity A0
#define RELAY_PIN A7

LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 20 column and 4 rows

/**
 * vordefinierte statische Werte
 */

#define maxValueTemp 30.5
#define maxValueAirHumidity 35
#define maxValueSoilHumidity 267
#define sensorSoilHumidityDry 595 // 595 // value for dry sensor
#define sensorSoilHumidityWet 239 // value for wet sensor
#define valueRefresh 1000
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

int iColumn;
int iRow;

/**
 *
 */

DHT dht(DHTPIN, DHTTYPE);

// Set LED_BUILTIN if it is not defined by Arduino framework
// #define LED_BUILTIN 13

void displaySetup()
{
  iColumn = 2;
  iRow = 1;
  // lcd.begin(20, 4);
  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  // lcd.setCursor(1, 1);
  // lcd.print("TEMP");
  // lcd.setCursor(1, 2);
  // lcd.print("HUMI");
  lcd.clear();
  // lcd.setCursor(4, 0);
  // lcd.print("Hackster");
  // Serial.println("Hackster");
  lcd.blink_off();
}

void scannerSetup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial.println("\nI2C Scanner");
}

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
  displaySetup();
  // scannerSetup();
  Serial.println("!!! FINISH !!!");
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

void addZeros()
{
  if (zahler <= 9)
  {
    Serial.print("0000");
  }
  else if (zahler <= 99)
  {
    Serial.print("000");
  }
  else if (zahler <= 999)
  {
    Serial.print("00");
  }
  else if (zahler <= 9999)
  {
    Serial.print("0");
  }
  else if (zahler <= 99999)
  {
    Serial.print("");
  }
  else
  {
    Serial.println("Read Error - illegal value");
  }
}

void printSerial()
{

  // check if any reads failed
  // The function isnan() returns 1 if the argument \a __x represents a "not-a-number" (NaN) object, otherwise 0.
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

    lcd.clear();
    lcd.noBlink();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(valueTempCelsius);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(valueTempFahrenheit);
    lcd.print(" F");

    lcd.setCursor(0, 2);
    lcd.print("Boden: ");
    lcd.print(valueSoilHumidity);

    lcd.setCursor(0, 3);
    lcd.print("Luft: ");
    lcd.print(valueAirHumidity);
    lcd.noBlink();
    //lcd.setCursor(0, 0);
  }
}


void scannerLoop()
{
  byte error, address;
  int Devices;
  Serial.println("Scanning...");
  Devices = 0;
  for (address = 1; address < 127; address++)
  {

    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      Devices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (Devices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
  delay(5000);

  /*
   * Ausgabe:
  Scanning...
  I2C device found at address 0x27  !
  done
  */
}

void productivityLoop()
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
    lcd.backlight();
  }

  printSerial();
  checkZahler();
  //displayLoop();
}

// The loop function is called in an endless loop
void loop()
{
  // scannerLoop();
  productivityLoop();
}

/**
 *
    Serial.print("\"valueSoilHumidity >= sensorSoilHumidityDryTest\" :: ");
    Serial.print("valueSoilHumidity: ");
    Serial.print(valueSoilHumidity);
    Serial.print(" | --- | ");
    Serial.print("sensorSoilHumidityDryTest: ");
    Serial.print(sensorSoilHumidityDryTest);
    Serial.println("");
 */