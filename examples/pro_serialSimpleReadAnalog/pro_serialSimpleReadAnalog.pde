import processing.serial.*;

Serial myPort;  // Create object from Serial class
float val;      // Data received from the serial port

void setup() 
{
  size(200, 200);
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
}

void draw()
{
  background(255);             // Set background to white
  fill(val);                   // set fill to black
  rect(50, 50, 100, 100);
}

void serialEvent(Serial p)
{
  val = float(myPort.readStringUntil('\n'));
}


/*

// Wiring / Arduino Code
// Code for sensing a potentiometer and writing the value to the serial port.

const int analogInPin = A0;

int sensorValue = 0;
int outputValue = 0;

void setup() {
  Serial.begin(9600); 
}

void loop() {
  sensorValue = analogRead(analogInPin);            
  outputValue = map(sensorValue, 0, 1023, 0, 255);  

  // print the results to the serial monitor: 
  Serial.println(outputValue);   

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(2);                     
}

*/
