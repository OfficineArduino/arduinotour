/*
  Arduino Emoncms client (sensor)
  www.openenergymonitor.om
  www.emoncms.org
  based on Yun HTTP Client
  
   created by Mirco Piccin on April 2014 for ArduinoTour IoT Workshop 
   @ WeMake / Milan
 */
 
#include <TinkerKit.h>
#include <Bridge.h>
#include <HttpClient.h>

TKThermistor temp(I0);
TKLightSensor light(I1);

String apikey = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  Serial.begin(9600);
  while(!Serial);
}

void loop() {
  
  int lightValue = light.read();
  float tempValue = temp.readCelsius();
  
  HttpClient client;
  client.get("http://emoncms.org/input/post.json?json={temp:" + String(tempValue) + ",light:" + String(lightValue) + "}&node=1&apikey=" + apikey);
  
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
//  Serial.flush();
  
  delay(10000);
}


