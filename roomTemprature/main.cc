#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#define SCHERM_BREEDTE 128 // OLED display breedte, in pixels
#define SCHERM_HOOGTE 64 // OLED display hoogte, in pixels
#define OLED_RESET -1
#define SCHERM_ADRES 0x3C
Adafruit_SSD1306 display(SCHERM_BREEDTE, SCHERM_HOOGTE, &Wire, OLED_RESET);
// GPIO where the DS18B20 is connected to
const int oneWireBus = D4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);


void setup() {
if (!display.begin(SSD1306_SWITCHCAPVCC, SCHERM_ADRES)) {
// Deze eeuwigdurende lus zorgt ervoor dat de applicatie niet verder gaat bij een fout
for (;;);
}
// Start the DS18B20 sensor
  sensors.begin();
}
void loop() {
display.clearDisplay(); // Maak het scherm leeg
display.setTextColor(WHITE); // Schrijf witte letters
display.setTextSize(1); // Zet het tekstformaat op 1
display.setCursor(0, 0); // Begin met schrijven op positie 0,0
sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
display.print( "Room temprature is : ");
display.print( temperatureC); // Schrijf de huidige temperatuur naar het scherm
display.print(" C"); // Schrijf tekst naar het scherm
display.display(); // Toon de tekst op het scherm
}