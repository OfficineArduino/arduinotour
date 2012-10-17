/*
  Basic Traffic Lights
 */

void setup() {                
  // initialize the digital pin as an output.

  //green led
  pinMode(7, OUTPUT);     
  //yellow led
  pinMode(8, OUTPUT);     
  //red led
  pinMode(9, OUTPUT);     
}

void loop() {
  // turn the green LED on
  digitalWrite(7, HIGH);
  //...and wait for 5 seconds
  delay(5000);
  
  // turn the green LED off
  digitalWrite(7, LOW);  
  // ...the yellow LED on
  digitalWrite(8, HIGH);
  //...and wait for 1 second
  delay(1000);

  // turn the yellow LED off
  digitalWrite(8, LOW);  
  // ...the red LED on
  digitalWrite(9, HIGH);
  //...and wait for 5 seconds
  delay(5000);

  // turn the red LED off
  digitalWrite(9, LOW);  
}
