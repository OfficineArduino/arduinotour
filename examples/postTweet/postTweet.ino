/*
  Arduinotour 2012 @ Rome 
  
  Basic example of Twitter post, by pitusso  
  
  Twitter library:
  http://arduino.cc/playground/Code/TwitterLibrary
  updated following this post:
  http://arduino.cc/forum/index.php?topic=121483.0
  Wifi shield Arduino library:
  http://arduino.cc/en/uploads/Main/WiFiShield_library.zip
*/


#include <SPI.h>
#include <WiFi.h>
#include <Twitter.h>

char ssid[] = "ssid";
char pass[] =  "pwd";

// Your Token to Tweet (get it from http://arduino-tweet.appspot.com/ ; following token is for ArduinoTX)
Twitter twitter("853399658-NYlyikic5XuHE1QKD1bAT8RkWN833QD1zA2vFzKa");

String msgStrBase = "#hashtag ";
String msgStr = "";
char msgChar[50];

int status = WL_IDLE_STATUS; // status of the wifi connection

const int  buttonPin = 2;

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

// initialize the library instance:
WiFiClient client;

void setup()
{
  Serial.begin(9600);
  
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);  
 
    // wait 10 seconds for connection:
    delay(10000);
  } 
  
  Serial.println("Ready ...");
}

void loop()
{
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {

      //TEST STRING 
 
      //create the string with sensors value
      // read the analog sensor:
      int sensorReading = analogRead(A0);   
      
      msgStr = msgStrBase;
      msgStr += "s1=";
      msgStr += sensorReading;

      // you can append multiple readings to this String if your
      // pachube feed is set up to handle multiple values:
      int otherSensorReading = analogRead(A1);
      msgStr += ",s2=";
      msgStr += otherSensorReading;
   
      msgStr = msgStr + ",i=" + buttonPushCounter;
      msgStr.toCharArray(msgChar, 50);

      tweetMsg(msgChar);
      buttonPushCounter++;
    } 
  }
  // save the current state as the last state, 
  //for next time through the loop
  lastButtonState = buttonState;
}

void tweetMsg(char *msg) {
  if (twitter.post(msg)) {
    // Specify &Serial to output received response to Serial.
    // If no output is required, you can just omit the argument, e.g.
    // int status = twitter.wait();
    int status = twitter.wait(&Serial);
    if (status == 200) {
      Serial.println("OK.");
    } else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("sending failed.");
  }  
}
