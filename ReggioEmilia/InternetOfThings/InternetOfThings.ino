/*
  Internet of Things
  invio dati relativi a sensori digitali, analogici e tag rfid via rete
  
  This Project is created by
  Luca Gualandri,Stefano,Marco Negri,Luca,Nicola Minerva,Giulia Dal Pont,Nicola Diligu,Pietro,Aras,Giorgia Dal Pont
  27/01/2013
 
  ArduinoTour 2013 @ FabLab / ReggioEmilia
  
*/

// constants won't change. They're used here to 
// set pin numbers:

#include <WiFi.h>
#include <SoftwareSerial.h>

long lasttimetemp = 0;
long tempdelay = 50;
float tempValue = 0;
int opticoupler = LOW;
int optoPin =8;

/*
char emonServer = "emoncms.org";
 char staticJsonURLS = "HTTP GET http://emoncms.org/input/post?json={power:"
 200}&apikey=44f2037922f67ddbfe1e1c7369ef3bc4";
 
 */

SoftwareSerial mySerial(2, 3); // RX, TX
String tagID = "";

const int tempSensorPin = A0; 
const int buttonPin = 3;     // the number of the pushbutton pin
const int ledPin =  10;      // the number of the LED pin
int buttonState = 0;         // variable for reading the pushbutton status
int luce;
int lastsend=0;


//SSID of your network 
char ssid[] = "ssid";

//password of your WPA Network 
char pass[] = "passkey";

int status = WL_IDLE_STATUS;
char servername[]="XXX.XXX.XXX.XXX";  // remote server we will connect to
WiFiClient client;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);     
  //inizializzo il monitor seriale
  //pinMode(ldr, INPUT);
  Serial.begin(9600); 
  mySerial.begin(9600); 
  pinMode(optoPin, INPUT);     

  status = WiFi.begin(ssid, pass);
  if ( status != WL_CONNECTED) { 
    Serial.println("Couldn't get a wifi connection");
    // don't do anything else:
    while(true);
  }
}

void loop(){
  //Lettura temperatura
  if ((millis() - lasttimetemp) > tempdelay) {
    lasttimetemp = millis();
    tempValue = getCelsius(analogRead(tempSensorPin));
    tempValue--;
    // Serial.print("temp=");
    //Serial.println(tempValue);
  }

  opticoupler=digitalRead(optoPin);

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {     
    // turn LED on:    
    digitalWrite(ledPin, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW); 
  }

  mySerial.listen();

  tagID = "";
  while (mySerial.available())
  {
    tagID += (char)mySerial.read();
  } 

  if (millis()-lastsend>3000) {

    if (client.connect(servername, 50007)) {
      Serial.println("connected");
      // Make a HTTP request:
      client.println("CONNECTION TEST ");

      client.println();

      if (tagID != "") {
        client.println("RF ID: ");
        client.println(tagID.substring(1,11));
        Serial.print("Tag ID : ");
        Serial.println(tagID.substring(1,11));
      }

      client.println("Temp: ");
      client.println(tempValue);
      Serial.print("Temp: ");
      Serial.println(tempValue);

      client.println("Stato dispositivo: ");
      client.println(opticoupler);   
      Serial.print("Opti: ");
      Serial.println(opticoupler);

      //legge i dati trasmessi dal server e li manda in seriale
      while (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
      lastsend=millis();
      // if the server's disconnected, stop the client:
      if (!client.connected()) {
        Serial.println();
        Serial.println("disconnecting from server.");
        client.stop();
      }

    }     

  }

}



float getCelsius(int sensorValue) {
  /*
created by Federico Vanzati for TinkerKit Thermistor Library
   */
  const static float ADCres = 1023.0;
  const static int Beta = 3950;	// Beta parameter
  const static float Kelvin = 273.15;	// 0°C = 273.15 K
  const static int Rb = 10000;	// 10 kOhm
  const static float Ginf = 120.6685;	// Ginf = 1/Rinf

  float Rthermistor = Rb * (ADCres / sensorValue - 1);
  float _temperatureC = Beta / (log( Rthermistor * Ginf )) ;
  return _temperatureC - Kelvin;
}

