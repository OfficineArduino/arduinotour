/*
  SonameQuesto
  (Sensors -> Internet)

  Developed by  Paolo / David / Daniele / Emiliano / Giacomo during 
  Arduinotour 2012 @ Rome 
  
  Twitter library:
  http://arduino.cc/playground/Code/TwitterLibrary
  updated following this post:
  http://arduino.cc/forum/index.php?topic=121483.0
  Wifi shield Arduino library:
  http://arduino.cc/en/uploads/Main/WiFiShield_library.zip
  
*/

#include <SPI.h> // needed in Arduino 0019 or later
#include <WiFi.h>
#include <Twitter.h>

char ssid[] = "htc"; //"crr-cisco";
char pass[] =  "qwertyuio"; //"ciaoo";

// Your Token to Tweet (get it from http://arduino-tweet.appspot.com/)
Twitter twitter("853399658-NYlyikic5XuHE1QKD1bAT8RkWN833QD1zA2vFzKa");

String msgStrBase = "#boh ";
String msgStr = "";

const int tweetLimit = 135;

char msgChar[tweetLimit];

int status = WL_IDLE_STATUS; // status of the wifi connection

const int  buttonPin = 2;

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;     	// current state of the button
int lastButtonState = 0; 	// previous state of the button

// initialize the library instance:
WiFiClient client;

// ---------------------

const int soglia = 35;
const int notes[] = {
  0, 1, 2, 3, 4, 5};
unsigned long startTimer[] = {
  0, 0, 0, 0, 0, 0};
unsigned long endTimer[] = {
  0, 0, 0, 0, 0, 0};
const int numNotes = 6;
int reading = 0;
int currNote;
String tweet = "";
int counter = 0;
boolean flagSend = false;
boolean flagBtn = false;
const int pinBtn = 7;
int ledArray[] = {
  8, 9, 3, 4, 5, 6};
int pinBuzzer = 5;

void setup(void) {
  Serial.begin(9600);

  for (int i=0; i<numNotes; i++)
  {
	pinMode(ledArray[i],OUTPUT);
  }

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

void loop(void) {

  pulsantePremuto();

  for (int i=0; i<numNotes; i++) {
    currNote = notes[i];
    reading = analogRead(currNote);
    if(reading >= soglia) {
      if(controlTweetLenght()) {
        startTimerFunc(i);
      } else {
        Serial.println("Limite Tweet");
      }
    } else if(reading < soglia) {
      endTimerFunc(i);
    }    
  }
}

void pulsantePremuto(){
  if (digitalRead(pinBtn)==1){
    if(flagBtn==false){
      sendTweet();
    } else {
      flagBtn=true;
    }
  }
}


boolean controlTweetLenght() {
  if(tweet.length() >= tweetLimit) {
    // NOn posso più tweettare!
    return false;
  } else {
    return true;
  }
}

void startTimerFunc(int i) {
  if(startTimer[i] == 0) {
    startTimer[i] = millis();
    digitalWrite(ledArray[i],HIGH);
    //digitalWrite(pinBuzzer,HIGH);  
    Serial.println ("ST");
    Serial.println (startTimer[i]);
  }
}

void endTimerFunc(int i) {
  if(endTimer[i] == 0 && startTimer[i] > 0) {
  //  digitalWrite(pinBuzzer,LOW);  
    endTimer[i] = millis() - startTimer[i];
    digitalWrite(ledArray[i],LOW);
    Serial.println ("eT");
    Serial.println (endTimer[i]);
      writeTweet(i);
  }
}

void writeTweet(int i) {
  int note = notes[i];
  int duration = endTimer[i];
  Serial.println(note);
  Serial.println(duration);
  Serial.println("");

  switch(i) {
  case 0:
    tweet +="C";
    break;
  case 1:
    tweet +="D";
    break;
  case 2:
    tweet +="E";
    break;
  case 3:
    tweet +="F";
    break;
  case 4:
    tweet +="G";
    break;
  case 5:
    tweet +="A";
    break;
  }

  tweet += "=";
  tweet += duration;
  tweet += ",";
  startTimer[i] = 0;
  endTimer[i] = 0;

  Serial.println(tweet);
}

void sendTweet() {
  if(flagSend==0){  
    flagSend=true;
    counter++;
    //int tempCounter=counter
    tweet += (String) counter;
    Serial.println("Send a Tweet");
    Serial.println(tweet);
    flagBtn=false;
    tweet.toCharArray(msgChar, tweetLimit);
    tweetMsg(msgChar);
  }
}

void resetTweet() {
  tweet = "";
  for (int i=0; i<numNotes; i++) {
    startTimer[i]=0;
    endTimer[i]=0;
  }
  flagSend= false;
 // flagBtn= false;
}

void tweetMsg(char *msg) {
  if (twitter.post(msg)) {
    // Specify &Serial to output received response to Serial.
    // If no output is required, you can just omit the argument, e.g.
    // int status = twitter.wait();
    int status = twitter.wait(&Serial);
    if (status == 200) {
      Serial.println("OK.");
      resetTweet();
    } else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("sending failed.");
  }  
}
