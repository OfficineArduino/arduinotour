/*
 stupidBot
 created 22 sept 2012
 by pitusso
 
 This example code is in the public domain.
 
 */

// These constants won't change.  They're used to give names
// to the pins used:
const int sharp01Pin = A0;  // DX Analog input pin that the 1st sharp is attached to
const int sharp02Pin = A1;  // RX Analog input pin that the 2nd sharp is attached to

const int motor1Pin = 9; // Analog output pin that the MOSFET is attached to
const int motor2Pin = 10;  // Analog outpu pin that the MOSFET is attached to

int sharp01Value = 0;        // value read from the 1st sharp
int sharp02Value = 0;        // value read from the 2st sharp

int speedMotor01 = 255;
int speedMotor02 = 255;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  pinMode(motor1Pin, OUTPUT);
  pinMode(motor2Pin, OUTPUT);
}

void loop() {
  //run motors
  analogWrite(motor1Pin, speedMotor01);
  analogWrite(motor2Pin, speedMotor02);

  // read the analog in value:
  sharp01Value = analogRead(sharp01Pin);            
  sharp02Value = analogRead(sharp02Pin);            
  
  // print the results to the serial monitor:
  Serial.print("sharp01 = " );                       
  Serial.print(sharp01Value);      
  Serial.print("\t sharp02 = ");      
  Serial.println(sharp02Value);   


  if (sharp01Value > 130) {
    //turn left
    speedMotor02 = 0;
  } else {
     speedMotor02 = 255;
  }

  if (sharp02Value > 130) {
    //turn left
    speedMotor01 = 0;
  } else {
     speedMotor01 = 255;
  }

  delay(2);                     
}
