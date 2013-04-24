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

//#define WIFI

#include <SPI.h>
#include <Twitter.h>
#include <SoftwareSerial.h>
#include <avr/pgmspace.h>

#ifdef WIFI
#include <WiFi.h>
#include <WiFiClient.h>
#else
#include <Ethernet.h>
#endif

// Your Token to Tweet (get it from http://arduino-tweet.appspot.com/ ; following token is for ArduinoTX)
Twitter twitter("1291709335-dWkDursuQHyhQtmWqNntnSA6BZNogfdxmwK3jC4");

String msgStrBase = "#hashtag ";
String msgStr = "";
char msgChar[50];

//network configuration, WIRED or WIFI
#ifdef WIFI
// Enter your WiFi network settings
char ssid[] = "your_ssid"; //  your network SSID (name) 
char pass[] = "your_password";    // your network password (use for WPA, or use as key for WEP)

int keyIndex = 0;            // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS; // status of the wifi connection
WiFiClient client;
#else
//if using WIRED
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x00, 0x69, 0xD5};

// fill in an available IP address on your network here,
// for auto configuration:
IPAddress ip(10, 0, 21, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress dnss(8, 8, 8, 8);
IPAddress gw(10, 0, 3, 79);

EthernetClient client;
#endif

const int  buttonPin = 3;

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button


void setup()
{
  Serial.begin(9600);

  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
#ifdef WIFI
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
    // Connect to WPA/WPA2 network. 
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  } 
  Serial.println("Connected to wifi");
#else
  if (!Ethernet.begin(mac)) {
    // if DHCP fails, start with a hard-coded address:
    Serial.println("Failed to get an IP address using DHCP, forcing manually");
    Ethernet.begin(mac, ip, dnss, gw, subnet);
  }
#endif  
  printStatus();
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
    } 
    else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } 
  else {
    Serial.println("sending failed.");
  }  
}

void printStatus() {
#ifdef WIFI
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.print(" dBm");
#else
  // print your local IP address:
  Serial.print("IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }  
#endif
  Serial.println();
}
