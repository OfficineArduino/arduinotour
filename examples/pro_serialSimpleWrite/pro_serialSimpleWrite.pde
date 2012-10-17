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
  size(200, 200);
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
  background(255);
  if (mouseOverRect() == true) {  // If mouse is over square,
    fill(204);                    // change color and
    myPort.write(int(1));              // send an 1 to indicate mouse is over square
  } 
  else {                        // If mouse is not over square,
    fill(0);                      // change color and
    myPort.write(0);              // send an 0 otherwise
  }
  rect(50, 50, 100, 100);         // Draw a square
}

boolean mouseOverRect() { // Test if mouse is over square
  return ((mouseX >= 50) && (mouseX <= 150) && (mouseY >= 50) && (mouseY <= 150));
}

void serialEvent(Serial p) {
  String inS = myPort.readString();
  print(inS);
}
/*
  // Wiring/Arduino code:
 // Read data from the serial and turn ON or OFF a light depending on the value

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
  if (val == 1) 
  {
    Serial.println("Led is On"); 
    digitalWrite(ledPin, HIGH);
  }
  else if (val == 0)
  {
    Serial.println("Led is Off");
    digitalWrite(ledPin, LOW);
  }
  else
  {
    Serial.println("Invalid!");
  }
  Serial.flush();
}

 */
