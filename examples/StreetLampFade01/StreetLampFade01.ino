
/*
  Streetlamp - fade
 */

const int analogInPin = A0;
const int analogOutPin = 9;

int sensorValue = 0;
int outputValue = 0;

void setup() {
  pinMode(analogOutPin, OUTPUT);
}

void loop() {
  sensorValue = analogRead(analogInPin);            

  outputValue = sensorValue / 4;

  analogWrite(analogOutPin, outputValue);           

  delay(2);                     
}



