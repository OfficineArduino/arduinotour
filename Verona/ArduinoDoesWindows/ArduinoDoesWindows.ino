/*
  Arduinotour 2013 @ Verona
 
  Nome Progetto: Arduino does Windows
  Autori: Gionata Mettifogo e tutto il tavolo 
  Materiale utilizzato: Arduino Uno, StarterKit, Windows PC with VisualStudio
  Linguaggi di programmazione: Arduino, C#
  Framework utilizzato: .NET
  Descrizione: Abbiamo connesso alcuni sensori ad Arduino (luce, temperatura ed 
  un potenziometro). Abbiamo collegato un LCD su Arduino dove è possibile vedere 
  i dati rilevati in tempo reale. Poi abbiamo connesso Arduino al laptop Windows 
  via seriale/USB che legge i valori e li comunica ad OpenEnergyMonitor sul quale 
  possiamo vedere grafici, etc anche remotamente da web o su telefono cellulare.
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int lightSensorPin = A0;    // select the input pin for the light sensor
int powerSensorPin = A1;  // select the input pin for the potentiometer
int temperatureSensor = A5; // select the input pin for the temperature sensor

void setup() 
{
  // will be sending data back to the PC at 9600 baud
  Serial.begin(9600);

  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
}

void loop() 
{
  // read the values from the sensors
  int lightValue = analogRead(lightSensorPin); 
  int powerValue = analogRead(powerSensorPin);

  int temperatureValue = analogRead(temperatureSensor);
  float voltage = ((float)temperatureValue / 1024.0) * 5.0;
  temperatureValue = (int)((voltage - .5) * 100.0);

  char s1[64];
  sprintf(s1, "light|%d|power|%d|temperature|%d\n", lightValue, powerValue, temperatureValue);
  Serial.println(s1);

  sprintf(s1, "lgt %4d", lightValue);
  lcd.setCursor(0, 0);
  lcd.print(s1);

  sprintf(s1, "pwr %4d tmp %2d", (short)powerValue, (short)temperatureValue);
  lcd.setCursor(0, 1);
  lcd.print(s1);

  delay(1000);
}


