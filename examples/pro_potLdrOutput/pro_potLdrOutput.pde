import processing.serial.*;

import cc.arduino.*;
Arduino arduino;

int potPin = 0;
int ldrPin = 1;
int potValue = 0;
int ldrValue = 0;
int ledPin = 9;
float circleSize = 0; 

int potIncrem = 5;
int ldrIncrem = 5;

boolean circleOver = false;
boolean ledStatus = false;
int circleX, circleY, screenSizeX, screenSizeY;

void setup() {
  screenSizeX = 640;
  screenSizeY = 480;
  
  circleX = screenSizeX / 2;
  circleY = screenSizeY / 2;

  size(screenSizeX, screenSizeY);
  
  arduino = new Arduino(this, Arduino.list()[0], 57600);

  arduino.pinMode(ledPin, Arduino.OUTPUT);
}


void draw() {
  update(mouseX, mouseY);
  
  potValue = arduino.analogRead(potPin);
  ldrValue = arduino.analogRead(ldrPin);

  background(255, 255, 255);

  circleSize = potValue / 2.5;

  fill((ldrValue / 4), 255 - (ldrValue / 4), (ldrValue / 4) / 2);
  ellipse(320, 240, circleSize, circleSize);
}


void mousePressed() {
  if (circleOver) {
    if (ledStatus) {
      arduino.digitalWrite(ledPin, Arduino.LOW);
    } else {
      arduino.digitalWrite(ledPin, Arduino.HIGH);
    }  
    ledStatus = !ledStatus;
  }  
}


boolean overCircle(int x, int y, float diameter) {
  float disX = x - mouseX;
  float disY = y - mouseY;
  if(sqrt(sq(disX) + sq(disY)) < diameter/2 ) {
    return true;
  } else {
    return false;
  }
}


void update(int x, int y) {
  if( overCircle(circleX, circleY, circleSize) ) {
    circleOver = true;
  } else {
    circleOver = false;
  }
}
