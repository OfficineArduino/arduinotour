/**
 * Simple Write. 
 * 
 * Check if the mouse is over a rectangle and writes the status to the serial port. 
 * This example works with the Wiring / Arduino program that follows below.
 */


import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port

void setup() 
{
  size(600, 200);
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
//  String portName = Serial.list()[0];
  String portName = "/dev/ttyUSB1";
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
}

void draw() { 
  val = constrain(mouseX / 2, 0, 255);
  background(val);
  myPort.write(val);
}

void serialEvent(Serial p) {
  String inS = myPort.readString();
  print(inS);
}
/*
  // Wiring/Arduino code:
 // Read data from the serial and PWM a light depending on the value

int ledPin = 9;

void setup()
{
  Serial.begin(9600); 
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  //Have the arduino wait to receive input
  while (Serial.available() == 0);
  
  //Read the input
  int val = Serial.read();
  
  Serial.println(val);
  analogWrite(ledPin, val);
  Serial.flush();
}

 */
