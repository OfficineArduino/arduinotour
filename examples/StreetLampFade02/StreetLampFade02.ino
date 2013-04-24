/*
  Streetlamp - fade #02 
 */

const int analogInPin = A0;
const int analogOutPin = 9;

int sensorValue = 0;       
int outputValue = 0;       

void setup() {
  Serial.begin(9600); 
}

void loop() {
  sensorValue = analogRead(analogInPin);            

  // map it to the range of the analog out:
  outputValue = 255 - map(sensorValue, 0, 1023, 0, 255);  

  // change the analog out value:
  analogWrite(analogOutPin, outputValue);           

  // print the results to the serial monitor:
  Serial.print("sensor = " );                       
  Serial.print(sensorValue);      
  Serial.print("\t output = ");      
  Serial.println(outputValue);   

  delay(2);                     
}


