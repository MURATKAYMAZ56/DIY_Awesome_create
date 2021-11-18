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
WiFiClientSecure client;
DynamicJsonDocument jsonObject(2048);
const char* ssid = "Hotel Apeldoorn"; // Vul hier jouw eigen netwerknaam (ssid) in
const char* password = ""; // Vul hier jouw eigen netwerkwachtwoord in
const String server = "api.coindesk.com"; // Het adres van de server
const String url = "/v1/bpi/currentprice.json"; // De url waar de informatie te vinden is
void setup() {
if (!display.begin(SSD1306_SWITCHCAPVCC, SCHERM_ADRES)) {
// Deze eeuwigdurende lus zorgt ervoor dat de applicatie niet verder gaat bij een fout
for (;;);
}
maakVerbindingMetWifi();
client.setInsecure(); // Gebruik geen beveiligde verbinding
}
void loop() {
gegevensOphalenEnWeergeven(); // Haal de tijd op van de server en geef deze weer op de
delay(1000 * 60); // Wacht 1 minuut (60000 milliseconden)
}
// Deze functie maakt verbinding met het netwerk
void maakVerbindingMetWifi() {
display.clearDisplay(); // Maak het scherm leeg
display.setTextColor(WHITE); // Schrijf witte letters
display.setTextSize(1); // Zet het tekstformaat op 1
display.setCursor(0, 0); // Begin met schrijven op positie 0,0
display.println("Verbinden met wifi"); // Schrijf tekst naar het scherm
display.display(); // Toon de teksten op het scherm
WiFi.mode(WIFI_STA); // Type wifi verbinding instellen
WiFi.begin(ssid, password); // Gebruik WiFi.begin om verbinding te maken
while (WiFi.status() != WL_CONNECTED) { // Herhalen tot er verbinding is
display.print("."); // Schrijf een punt op het scherm
display.display(); // Toon de teksten op het scherm
delay(500); // Wacht een halve seconde
}
display.clearDisplay(); // Maak het scherm leeg
display.setCursor(0, 0); // Begin met schrijven op positie 0,0
display.println("Verbonden met wifi"); // Schrijf tekst naar het scherm
display.print("SSID: "); // Schrijf tekst naar het scherm
display.println(WiFi.SSID()); // Schrijf het SSID naar het scherm
display.print("IP: "); // Schrijf tekst naar het scherm
display.println(WiFi.localIP()); // Schrijf het ip-adres naar het scherm
display.display(); // Toon de teksten op het scherm
}
// Deze functie haalt gegevens op van de server en geeft deze weer op de display
void gegevensOphalenEnWeergeven() {
// Uitvoeren als er een verbinding gemaakt is met de server
bool verbindingGeslaagd = client.connect(server, 443);
// Uitvoeren als er een fout is opgetreden
if (!verbindingGeslaagd) {
display.clearDisplay(); // Maak het scherm leeg
display.setTextSize(1); // Zet het tekstformaat op 1
display.setCursor(0, 0); // Begin met schrijven op positie 0,0 (links boven)
display.print("Kan geen verbinding maken met server"); // Schrijf tekst naar het scherm
display.display(); // Toon de tekst op het scherm
}
// Een website opvragen bij de server
client.println("GET https://" + server + url + " HTTP/1.0");
client.println("Host: " + server);
client.println("Connection: close");
client.println();
// Wachten tot de server klaar is om de gegevens te versturen
while (client.connected()) {
String line = client.readStringUntil('\n');
if (line == "\r") {
break;
}
}
String gegevens = client.readString(); // De gegevens van de server uitlezen
deserializeJson(jsonObject, gegevens); // De gegevens omzetten naar een JSON object
// De huidige prijs uitlezen
float huidigePrijs = jsonObject["bpi"]["EUR"]["rate_float"].as<float>();
display.clearDisplay(); // Maak het scherm leeg
display.setCursor(16, 27); // Begin met schrijven op positie 16,27
display.setTextSize(2); // Zet het tekstformaat op 2
display.print(huidigePrijs); // Schrijf de huidige prijs gegevens naar het scherm
display.display(); // Toon de tekst op het scherm
client.stop(); // Stop de verbinding met de server
}