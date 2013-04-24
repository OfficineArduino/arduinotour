/*
  Arduinotour 2013 @ Verona
 
  Nome Progetto: Arduino Sensor Monitor
  Autori: Andrea Tabarelli de Fatis, Gionata, Luca Rigon, ... 
  Materiale utilizzato: Arduino Uno, StarterKit, Arduino WifiShield.
  Linguaggi di programmazione: Arduino
  Descrizione: rilevare la temperatura e la luce con appositi sensori ed inviarli tramite una Wifi Shield 
  a OpenEnergyMonitor che salva e monitora l’andamento dei valori grazie ad una dashboard personalizzabile 
  (emoncms.org) user: workshoparduino pass: workshop
  
  based on Arduino & OpenEnergyMonitor
  https://github.com/OfficineArduino/Tutorial-Wired/tree/master/OpenEnergyMonitor   
*/

#include <stdio.h>
#include <LiquidCrystal.h>
int led = 13;

int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
LiquidCrystal lcd(9, 8, 5, 4, 3, 2);
const int VAL_PROBE = 0; // Analog pin 0
const int MOISTURE_LEVEL = 250; // the value after the LED goes ON

int termometro = 0;
float temperatura = 20.0;
float voltage = 0.0;

//if using a W5100 based Ethernet shield, comment out the following line;
//leave untouched if using Arduino Wifi Shield
#define WIFI

#include <SPI.h>

#ifdef WIFI
#include <WiFi.h>
#include <WiFiClient.h>
#else
#include <Ethernet.h>
#endif

//network configuration, WIRED or WIFI
#ifdef WIFI
//if using WIFI
char ssid[] = "htc"; //  your network SSID (name)
char pass[] = "qwertyuio";    // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
int keyIndex = 0;            // your network key Index number (needed only for WEP)

WiFiClient client;
#else
//if using WIRED
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x00, 0x69, 0xD5};

// fill in an available IP address on your network here,
// for auto configuration:
IPAddress ip(192, 168, 1, 2);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);
IPAddress gw(192, 168, 1, 254);

EthernetClient client;
#endif

//Calibrations
const float temp_offset = 0;

// Sensor pins
const int tempSensorPin = A1;
const int lightSensorPin = A0;

float tempValue = 0;
int lightValue = 0;

//Emoncms configurations
char server[] = "emoncms.org";     // name address for emoncms.org
//IPAddress server(213, 138, 101, 177);  // numeric IP for emoncms.org (no DNS)

String apikey = "1cf4342742c4516c9a319052f7b45166";  //api key
int node = 0; //if 0, not used per usare più arduino

unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 1*1000;  // delay between updates, in milliseconds ogni 10 secondi

void setup() {
  // start serial port:
  Serial.begin(9600);
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(led, OUTPUT);
  // Display a welcome message
  Serial.println("Emoncms client starting...");

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
    Ethernet.begin(mac, ip, dns, gw, subnet);
  }
#endif  

  printStatus();
}

void loop() {
  // read the value from the sensor:

  int moisture = analogRead(VAL_PROBE);
  int y = map(moisture, 730, 1023, 0, 254);
  termometro = analogRead(A1);
  voltage = (termometro/1024.0) * 5.0;
  temperatura = (voltage - .5) * 100;
  sensorValue = analogRead(sensorPin); 

  // turn the ledPin on
  // stop the program for <sensorValue> milliseconds:
  lcd.print("luce:");
  lcd.print(sensorValue);
  lcd.print("gradi:");
  lcd.print(temperatura);
  delay (500);
  lcd.clear();

  if (temperatura > 27) {
    digitalWrite(led ,HIGH);
    Serial.print("ok");
    Serial.print(temperatura);


  }
  else {
    Serial.print("no");

  }



  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("Disconnecting...");
    client.stop();
  }

  // if you're not connected, and at least <postingInterval> milliseconds have
  // passed sinceyour last connection, then connect again and
  // send data:
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {

    //read sensors

    //Print values (debug)
    Serial.println();
    Serial.print("Temp : ");
    Serial.print(temperatura);
    Serial.print(" ; Light : ");
    Serial.println(sensorValue);

    //send values
    sendData();
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

// this method makes a HTTP connection to the server:
void sendData() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("Connecting...");
    // send the HTTP GET request:
    client.print("GET /api/post?apikey=");
    client.print(apikey);
    if (node > 0) {
      client.print("&node=");
      client.print(node);
    }
    client.print("&json={temp2:");
    client.print(temperatura + temp_offset);    
    client.print(",light2:");
    client.print(sensorValue);
    client.println("} HTTP/1.1");
    client.println("Host:emoncms.org");
    client.println("User-Agent: Arduino-ethernet");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  }
  else {
    // if you couldn't make a connection:
    Serial.println("Connection failed");
    Serial.println("Disconnecting...");
    client.stop();
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


