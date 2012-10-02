/*
  Tweet Your Music
  (Internet -> Actuators)
  
  Developed by  Marta / Roberta / Simone / Silvano during 
  Arduinotour 2012 @ Rome 
  
  Sounds & Lights based on a tweet by user / hashtag
  Speaker & RGB led (piranha).
  
  Tone library
  http://code.google.com/p/rogue-code/downloads/detail?name=Arduino-Library-Tone.zip&can=2&q=
    updated to work with 1.0.1 Arduino IDE : simply replace in Tone/Tone.cpp 
    #include <wiring.h> 
    with
    #include <Arduino.h>
  Wifi shield Arduino library
  http://arduino.cc/en/uploads/Main/WiFiShield_library.zip
  
*/

#include <Tone.h>
#include <SPI.h>
//#include <WiFi.h>
#include <Ethernet.h>

const int MAX_NOTES = 22;
int notes[MAX_NOTES];
int duration[MAX_NOTES];

int ledR = 3;
int ledG = 6;
int ledB = 5;

int speaker = 9;

Tone tone1;
  q
//wired
byte mac[] = {  
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
EthernetClient client;  
  
/**  
//wireless  
char ssid[] = "ssid"; //  your network SSID (name) 
char pass[] = "pwd";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS; // status of the wifi connection
WiFiClient client; // initialize the library instance:
int keyIndex = 0;            // your network key Index number (needed only for WEP)
*/

const unsigned long requestInterval = 30*1000;    // delay between requests; 30 seconds

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(199,59,149,200);    // numeric IP for api.twitter.com
char server[] = "api.twitter.com";     // name address for twitter API
//char server[] = "search.twitter.com";     // name address for twitter search

boolean requested;                     // whether you've made a request since connecting
unsigned long lastAttemptTime = 0;     // last time you connected to the server, in milliseconds

String currentLine = "";               // string to hold the text from server
String tweet = "";                     // string to hold the tweet
boolean readingTweet = false;          // if you're currently reading the tweet

String stringTwo = "";
int index = 0;

void setup() {
  
  Serial.begin(9600);  
  tone1.begin(9);
  
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  
  pinMode(speaker, OUTPUT);
  
  //inizializzazione array
  emptyArray();
  
  // reserve space for the strings:
  currentLine.reserve(256);
  tweet.reserve(150);

  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

/**
//<WIRELESS>  
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
  
  printWifiStatus(); //wireless
//</WIRELESS>  
*/

//<WIRED>
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }

  printWiredStatus();
//</WIRED>

  connectToServer();
}

void loop()
{
  if (client.connected()) {
    if (client.available()) {
      // read incoming bytes:
      char inChar = client.read();

      // add incoming byte to end of line:
      currentLine += inChar; 

      // if you get a newline, clear the line:
      if (inChar == '\n') {
        currentLine = "";
      } 
      // if the current line ends with <text>, it will
      // be followed by the tweet:
      if ( currentLine.endsWith("<text>")) {
        // tweet is beginning. Clear the tweet string:
        readingTweet = true; 
        tweet = "";
        // break out of the loop so this character isn't added to the tweet:
        return;
      }
      // if you're currently reading the bytes of a tweet,
      // add them to the tweet String:
      if (readingTweet) {
        if (inChar != '<') {
          tweet += inChar;
        } else {
          // if you got a "<" character,
          // you've reached the end of the tweet:
          readingTweet = false;
          Serial.println(tweet);  
         
         //String tweet="#tym C=1000,E=500,F=1400,C=500,C=1000,E=1000,C=1000,C=1000,C=1000,F=1000,C=1000,G=1000,C=1000,D=1000,C=1000,A=1000,C=1000,E=1000,i=2"; //test string
          String subtweet = "";
  
          int notesIndex = 0;
          int assPos = tweet.indexOf("=");
          String tmpStr = "";
          char tmpStrChar[5];

          int sepPos = 0;
  
          while (assPos > -1) {
            sepPos = tweet.indexOf(",", assPos);
    
            if (sepPos > -1) {

              tmpStr = tweet.substring(assPos -1, assPos);
              if (tmpStr == "C") notes[notesIndex] = NOTE_C5;
              if (tmpStr == "D") notes[notesIndex] = NOTE_D5;
              if (tmpStr == "E") notes[notesIndex] = NOTE_E5;
              if (tmpStr == "F") notes[notesIndex] = NOTE_F5;
              if (tmpStr == "G") notes[notesIndex] = NOTE_G5;
              if (tmpStr == "A") notes[notesIndex] = NOTE_A5;
              if (tmpStr == "B") notes[notesIndex] = NOTE_B5;
      
              tmpStr = tweet.substring(assPos + 1, sepPos);
              tmpStr.toCharArray(tmpStrChar, 5);
              duration[notesIndex] = atoi(tmpStrChar);
      
              Serial.print("Saving note: ");
              Serial.print(notes[notesIndex]);
              Serial.print(", Duration: ");
              Serial.println(duration[notesIndex]);
            }else{
              break;
            }
            assPos = tweet.indexOf("=", sepPos);
            notesIndex ++;
          }  

          //Play your tweet
          playmusic(); 
    
          // close the connection to the server:
          client.stop(); 
        }
      }
    }   
  }
  else if (millis() - lastAttemptTime > requestInterval) {
    // if you're not connected, and <requestInterval> have passed since
    // your last connection, then attempt to connect again:
    connectToServer();
  }
}

void connectToServer() {
  // attempt to connect:
  Serial.println("connecting to server...");
  if (client.connect(server, 80)) {
    Serial.println("making HTTP request...");
    // make HTTP GET request to twitter:
    client.println("GET /1/statuses/user_timeline.xml?screen_name=ArduinoTX&count=1 HTTP/1.1");
    client.println("Host:api.twitter.com");
//    client.println("GET /search.atom?q=%23tym&count=1 HTTP/1.1");
//    client.println("HOST: search.twitter.com");
    client.println("Connection:close");
    client.println();
  }
  // note the time of this connect attempt:
  lastAttemptTime = millis();
}   

/*
void printWifiStatus() {
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
  Serial.println(" dBm");
}
*/

void printWiredStatus() {
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
}

void playmusic(){
  
  for (int i=0; i< MAX_NOTES;i++) {
    if (notes[i] != 0){
      if (notes[i] == 523) {
      //color c
        analogWrite(ledR, 254);
        analogWrite(ledG, 0);
        analogWrite(ledB, 0);
      } else if (notes[i] == 587) {
      //color d
        analogWrite(ledR, 0);
        analogWrite(ledG, 255);
        analogWrite(ledB, 0);
      } else if (notes[i] == 659) {
      //color e
        analogWrite(ledR, 0);
        analogWrite(ledG, 0);
        analogWrite(ledB, 255);
      } else if (notes[i] == 698) {
      //color f
        analogWrite(ledR, 254);
        analogWrite(ledG, 255);
        analogWrite(ledB, 0);
      } else if (notes[i] == 784) {
      //color g
        analogWrite(ledR, 0);
        analogWrite(ledG, 255);
        analogWrite(ledB, 255);
      } else if (notes[i] == 880) {
      //color a
        analogWrite(ledR, 254);
        analogWrite(ledG, 255);
        analogWrite(ledB, 120);
      } else if (notes[i] == 988) {
      //color b
        analogWrite(ledR, 120);
        analogWrite(ledG, 100);
        analogWrite(ledB, 255);
      }

      tone1.play(notes[i]);
      
      Serial.print("Playing note: ");
      Serial.print(notes[i]);
      Serial.print(", Duration: ");
      Serial.println(duration[i]);
      
      delay(duration[i]);
    
      tone1.stop();
    }
    
    digitalWrite(ledR, HIGH);   // turn the LED off by making the voltage HIGH ()
    digitalWrite(ledG, HIGH); 
    digitalWrite(ledB, HIGH); 
    delay(10); 
  }
  emptyArray();
}
  
void emptyArray(){
  for (int i=0; i< MAX_NOTES;i++) { 
    notes[i] = 0;
    duration[i] = 0;  
  }
}

