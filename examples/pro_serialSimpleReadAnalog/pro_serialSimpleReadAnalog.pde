import processing.serial.*;

Serial myPort;  // Create object from Serial class
float potentiometer;      // Data received from the serial port
int button;
String longValue = ""; 

void setup() 
{
  size(200, 200);
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = "/dev/ttyACM1"; 
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
}

void draw()
{
  background(255);             // Set background to white
  //fill(val * 255);                   // set fill to black
  fill(123, button * 255, potentiometer);
  /*
  if (val == 1) {
    fill(255);  
  } else {
    fill(0); 
  }
  */  
  rect(50, 50, 100, 100);
}

void serialEvent(Serial p)
{
  longValue = myPort.readString();
  //longValue.split(";")
  
  print(longValue);
  
  button = int(longValue.split(";")[0]);
  potentiometer = float(longValue.split(";")[1]);
  
  print(button);
  print(";");
  println(potentiometer);
  //val = float(myPort.readString());
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
