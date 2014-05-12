/*
  Arduino Emoncms actuator 
  www.openenergymonitor.om
  www.emoncms.org
  based on Yun HTTP Client
  
  created by Mirco Piccin on April 2014 for ArduinoTour IoT Workshop 
  @ WeMake / Milan
 */
 
#include <TinkerKit.h>
#include <Bridge.h>
#include <HttpClient.h>
 
TKLed strisciaLed(O0);

//update id_to_check with the feed id you want to check
String id_to_check = "xxxxx";
 
void setup() {
  // Bridge takes about two seconds to start up
  // it can be helpful to use the on-board LED
  // as an indicator for when it has initialized
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);
 
  Serial.begin(9600);
 
  while (!Serial); // wait for a serial connection
}
 
void loop() {
  char lettura[5];
  int indice = 0;
 
  // Initialize the client library
  HttpClient client;
 
  // Make a HTTP request:
  client.get("http://emoncms.org/feed/value.json?id=" + id_to_check);
 
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    if (c != '"') {
      lettura[indice] = c;
      indice++;    
    }
  }
  Serial.flush();
 
  float temp = atof(lettura);
  Serial.print(temp);  
 
  if (temp < 30.0) {
      strisciaLed.on();
  } else {
    strisciaLed.off();
  }  
 
  delay(5000);
}
