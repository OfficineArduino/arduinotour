import processing.serial.*;

import cc.arduino.*;
Arduino arduino;

int potPin = 0;
int ldrPin = 1;
int potValue = 0;
int ldrValue = 0;
float circleSize = 0; 

int potIncrem = 5;
int ldrIncrem = 5;

boolean circleOver = false;
int circleX, circleY, screenSizeX, screenSizeY;

void setup() {
  screenSizeX = 640;
  screenSizeY = 480;
  
  circleX = screenSizeX / 2;
  circleY = screenSizeY / 2;

  size(screenSizeX, screenSizeY);
  
  arduino = new Arduino(this, Arduino.list()[0], 57600);
}

void draw() {
  potValue = arduino.analogRead(potPin);
  ldrValue = arduino.analogRead(ldrPin);

  background(255, 255, 255);

  circleSize = potValue / 2.5;

  fill((ldrValue / 4), 255 - (ldrValue / 4), (ldrValue / 4) / 2);
  ellipse(320, 240, circleSize, circleSize);
}

