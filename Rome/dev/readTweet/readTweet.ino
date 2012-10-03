/*
  Arduinotour 2012 @ Rome 
  
  Basic example of Twitter client, both for WIFI and Ethernet, by pitusso  
  
  Twitter library:
  http://arduino.cc/playground/Code/TwitterLibrary
  updated following this post:
  http://arduino.cc/forum/index.php?topic=121483.0
  Wifi shield Arduino library:
  http://arduino.cc/en/uploads/Main/WiFiShield_library.zip
*/

#include <SPI.h>
//#include <WiFi.h>
#include <Ethernet.h>

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
*/

int keyIndex = 0;            // your network key Index number (needed only for WEP)

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
//</WIRED>

  // you're connected now, so print out the status:
  //printWifiStatus(); //wireless
  printWiredStatus(); //wired
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
        } 
        else {
          // if you got a "<" character,
          // you've reached the end of the tweet:
          readingTweet = false;
          Serial.println(tweet);   



          //parsing example
          int s1Pos = 0;
          int sepPos = 0;
          String s1Val = "";
          int s1ValI = 0;
          char tmpVal[4];
  
          s1Pos = tweet.indexOf("s1=");
          sepPos = tweet.indexOf(",", s1Pos + 3);
          s1Val = tweet.substring(s1Pos + 3, sepPos);
          if (s1Val != "") {
            s1Val.toCharArray(tmpVal, 4);
            s1ValI = atoi(tmpVal);
          }  
          
          //print value
          Serial.println(s1ValI);

          //operations
          if (s1ValI > 123) { 
            digitalWrite(13, HIGH); 
            //do something 
          }              
         

          
          // close the connection to the server:
          client.stop(); 
        }
      }
    }   
  }
  else if (millis() - lastAttemptTime > requestInterval) {
    // if you're not connected, and two minutes have passed since
    // your last connection, then attempt to connect again:
    connectToServer();
  }
}

void connectToServer() {
  // attempt to connect, and wait a millisecond:
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
