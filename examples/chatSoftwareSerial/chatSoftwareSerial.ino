/*
  Simple Arduino Chat
  Set "Both NL & CR" as line ending
*/
  
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX
String msg = "";
char c;

void setup()  
{
  Serial.begin(9600);
  
  Serial.println("Arduino Chat");
  mySerial.begin(9600);
}

void loop() // run over and over
{
  if (mySerial.available()) {
    c = mySerial.read(); 
    if (c == '\n') {    
      Serial.println(msg);
      msg = "";
    } else {  
      msg += c;
    }    
  }
  
  if (Serial.available())
    mySerial.write(Serial.read());
}
