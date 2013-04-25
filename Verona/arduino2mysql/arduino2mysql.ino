/*
  Arduinotour 2013 @ Verona
 
  Nome Progetto: Arduino 2 MySQL
  Autori: Edoardo Scalafiotti || Andrea Lizzi, Arduino2MySql
  Materiale utilizzato: Arduino Uno, RaspBerry PI, StarterKit.
  Linguaggi di programmazione: Arduino, PHP, SQL, Javascript, Python
  Framework utilizzato: Yii (PHP), JQuery (Javascript)
  Descrizione: connettere Arduino ad un sistema Unix (Linux o Mac) 
  e inviare dati provenienti da un’uscita analogica (come un lettore 
  di temperatura o di luce). Il dato viene quindi salvato in un 
  database MySql
  
  Maggiori informazioni: http://goo.gl/2ZowO
*/

#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 10 on the Arduino
#define ONE_WIRE_BUS 10

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
  if (!sensors.getAddress(insideThermometer, 0)) 
    Serial.println("Unable to find address for Device 0"); 
  sensors.setResolution(insideThermometer, 12);
  
}


void printTemperature(DeviceAddress deviceAddress) // function to print the temperature for a device
{
  float tempC = sensors.getTempC(deviceAddress);
  String a; 
  char temp[10];
  dtostrf(tempC, 2, 1, temp);
  String tempAsString = String(temp);
  //Serial.println("insideThermometer " + tempAsString);
  Serial.println("xyz123 " + tempAsString);
  delay(1000);
}

void loop(void)
{ 
  sensors.requestTemperatures(); // Send the command to get temperatures
  printTemperature(insideThermometer); // Use a simple function to print out the data
  
  delay(1000);
}

