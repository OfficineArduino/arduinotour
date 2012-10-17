/*
  Simple Streetlamp
 */

const int analogInPin = A0;
const int ledPin = 9;

int sensorValue = 0;

void setup() {
  Serial.begin(9600); 
  pinMode(ledPin, OUTPUT);
}

void loop() {
  sensorValue = analogRead(analogInPin);            

  Serial.print("sensor = " );                       
  Serial.print(sensorValue);      
  Serial.print("; led = " );                       
  
  if (sensorValue < 250) {
     Serial.println("ON");                       
     digitalWrite(ledPin, HIGH);
  } else {
     Serial.println("OFF");                           
     digitalWrite(ledPin, LOW);
  }

  delay(2);                     
}





