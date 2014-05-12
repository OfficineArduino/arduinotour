/*
  Arduino generic sensor (plain/text) for Paraimpu 
  Based on Xively sensor client with Strings

 This sketch connects an analog sensor to Paraimpu,
 using an Arduino Yún.

 created by Mirco Piccin on April 2014 for ArduinoTour IoT Workshop 
 @ WeMake / Milan
 */
 
#include <TinkerKit.h>
#include <Process.h>

const unsigned long postingInterval = 30000;  //delay between updates to xively.com
unsigned long lastRequest = 0;      // when you last made a request
String dataString = "";

TKPotentiometer pot(I0);

void setup() {
  // start serial port:
  Bridge.begin();
  Serial.begin(9600);

  while (!Serial);  
  Serial.println("Paraimpu client");

  lastRequest = millis();
}

void loop() {
  // get a timestamp so you can calculate reading and sending intervals:
  long now = millis();

  // if the sending interval has passed since your
  // last connection, then connect again and send data:
  if (now - lastRequest >= postingInterval) {
    updateData();
    sendData();
    lastRequest = now;
  }
  Serial.println(pot.read());
  delay(2);
}

void updateData() {
  int potValue = pot.read();
  if (potValue > 600) {
    dataString = "IN";
  } else if (potValue < 400) {
    dataString = "OUT";
  } else {
    dataString = "";
  }
}

// this method makes a HTTP connection to the server:
void sendData() {
  if (dataString != "") {
    // form the string for the URL parameter:
    String url = "http://api.paraimpu.com/data/new";

    // Send the HTTP POST request

    Process paraimpu;
    Serial.print("\n\nSending data... ");
    paraimpu.begin("curl");
    paraimpu.addParameter("-k");
    paraimpu.addParameter("--request");
    paraimpu.addParameter("POST");
    paraimpu.addParameter("--data");
    paraimpu.addParameter("{\"token\":\"xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx\", \"content-type\":\"text/plain\", \"data\":\"" + dataString + "\"}");
    paraimpu.addParameter(url);
    paraimpu.run();
    Serial.println("done!");

    // If there's incoming data from the net connection,
    // send it out the Serial:
    while (paraimpu.available() > 0) {
      char c = paraimpu.read();
      Serial.write(c);
    }
  }
}




