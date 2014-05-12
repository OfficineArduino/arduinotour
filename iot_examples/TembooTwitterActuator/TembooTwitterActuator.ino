/*
  Arduino Temboo actuator (by Twitter direct message parsing)
  
  created by Mirco Piccin on April 2014 for ArduinoTour IoT Workshop 
  @ WeMake / Milan
 */

#include <TinkerKit.h>
#include <Bridge.h>
#include <Temboo.h>
 
#define TEMBOO_ACCOUNT "temboo_account"  // Your Temboo account name
#define TEMBOO_APP_KEY_NAME "myFirstApp"  // Your Temboo app key name
#define TEMBOO_APP_KEY "temboo_app_key"  // Your Temboo app key
 
int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 10;   // Maximum number of times the Choreo should be executed
 
String tweet = "";
String sender = "";
String id = "1";
 
TKLed ledRosso(O0);

//update from_sender with the twitter user allowed
String from_sender = "tweet_sender_allowed" 
 
void setup() {
  Serial.begin(9600);
 
  // For debugging, wait until the serial console is connected.
  delay(4000);
  while (!Serial);
  Bridge.begin();
}
void loop()
{
  if (numRuns <= maxRuns) {
    Serial.println("Running GetDirectMessages - Run #" + String(numRuns++));
 
    TembooChoreo GetDirectMessagesChoreo;
 
    // Invoke the Temboo client
    GetDirectMessagesChoreo.begin();
 
    // Set Temboo account credentials
    GetDirectMessagesChoreo.setAccountName(TEMBOO_ACCOUNT);
    GetDirectMessagesChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    GetDirectMessagesChoreo.setAppKey(TEMBOO_APP_KEY);
 
    // Set Choreo inputs
    GetDirectMessagesChoreo.addInput("Count", "1");
    GetDirectMessagesChoreo.addInput("AccessToken", "twitter_access_token");
    GetDirectMessagesChoreo.addInput("AccessTokenSecret", "access_token_secret");
    GetDirectMessagesChoreo.addInput("ConsumerSecret", "consumer_secret");
    GetDirectMessagesChoreo.addInput("SinceID", id);
    GetDirectMessagesChoreo.addInput("ConsumerKey", "consumer_key");
    GetDirectMessagesChoreo.addInput("IncludeEntities", "false");
    GetDirectMessagesChoreo.addOutputFilter("text", "/[1]/text", "Response");
    GetDirectMessagesChoreo.addOutputFilter("sender", "/[1]/sender_screen_name", "Response");
    GetDirectMessagesChoreo.addOutputFilter("id", "/[1]/id_str", "Response");
 
    // Identify the Choreo to run
    GetDirectMessagesChoreo.setChoreo("/Library/Twitter/DirectMessages/GetDirectMessages");
 
    // Run the Choreo; when results are available, print them to serial
    int returnCode = GetDirectMessagesChoreo.run();
 
    if (returnCode == 0) {
      //solo se la richiesta è andata a buon fine, procediamo
 
      // choreo outputs are returned as key/value pairs, delimited with
      // newlines and record/field terminator characters, for example:
      // Name1\n\x1F
      // Value1\n\x1E
      // Name2\n\x1F
      // Value2\n\x1E
      while (GetDirectMessagesChoreo.available()) {
 
        String key = GetDirectMessagesChoreo.readStringUntil('\x1F');
        String value = GetDirectMessagesChoreo.readStringUntil('\x1E');
        key.trim();
        value.trim();
 
        if (value != "") {
          if (key == "sender") {
            sender = value;
          } else if (key == "id") {
            id = value;
          } else if (key == "text") {
            tweet = value;
          }
        }
      }
      if (tweet != "") {
        Serial.print(tweet + " by " + sender);
 
        if (sender == from_sender) {
          if ((tweet.indexOf("accendi") > -1) || (tweet.indexOf("ACCENDI") > -1)) {
            //accendi il led
            ledRosso.on();
          } else if ((tweet.indexOf("spegni") > -1) && (tweet.indexOf("led") > -1)) {
            //spegni il led
            ledRosso.off();
          }
        }
        tweet = "";
      }
    } else {
      while (GetDirectMessagesChoreo.available()) {
        char c = GetDirectMessagesChoreo.read();
        Serial.print(c);
      }
    }
    GetDirectMessagesChoreo.close();
  }
 
  Serial.println("Waiting...");
  delay(60000); // wait 30 seconds between GetDirectMessages calls
}
