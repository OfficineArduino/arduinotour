/*
  Arduinotour 2013 @ Verona
 
  Nome Progetto: Arduino Cercapersone
  Autori: Davide Pecchini
  Materiale utilizzato: Arduino Uno, StarterKit, Macbook Pro, 
  Arduino GSM Shield, 1x Led verde, 2x bottoni, 1x Display LCD, 
  1x potenziometro, 2x resistenza da 220 ohm, 
  2x resistenza 10 kohm
  Linguaggi di programmazione: Arduino
  Descrizione: Ho utilizzato uno shied GSM per realizzare un 
  lettore di sms in grado di rispondere con 2 possibili messaggi 
  pre-registrati, nello specifico: <<SI>> e <<NO>>, il led avvisa 
  quando si è ricevuto il messaggio illuminandosi e si spegne 
  quando il dispositivo è in attesa.
*/

//cercapersone

// include the library code:
#include <LiquidCrystal.h>
#include <GSM.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 9, 8);
int dx = A0;
int sx = A1;
int statodx = 0;
int statosx = 0;
int pulito = 0;
int led= 10;


// PIN Number for the SIM
#define PINNUMBER ""

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

// Array to hold the number a SMS is retreived from
char senderNumber[20];  

void setup()
{
  // initialize serial communications and wait for port to open:
  lcd.begin(16, 2);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println("SMS Messages Receiver");

  // connection state
  boolean notConnected = true;

  // Start GSM connection
  while(notConnected)
  {
    if(gsmAccess.begin(PINNUMBER)==GSM_READY)
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("GSM initialized");
  Serial.println("Waiting for messages");
}




char txtMsg1[200]="SI";
char txtMsg2[200]="NO";


void loop()
{
  char c;
  // If there are any SMSs available()  
  if (sms.available())
  {
    lcd.clear();
    digitalWrite(led, HIGH);

    // Get remote number
    sms.remoteNumber(senderNumber, 20);
    lcd.print("da:");
    lcd.print(senderNumber);


    // Read message bytes and print them
    lcd.setCursor(0, 1);
    while(c=sms.read())  {
      lcd.print(c);
    }

    // Delete message from modem memory
    int contatore=0;
    sms.flush();
    Serial.println("MESSAGE DELETED");

    while(contatore!=1){
      Serial.println("ciao");
      statodx=digitalRead(dx);
      statosx=digitalRead(sx);
      delay(100);


      if(statodx==HIGH){
        //messaggio
        sms.beginSMS(senderNumber);
        sms.print(txtMsg2);
        sms.endSMS();
        Serial.println("\nCOMPLETE!\n");
        lcd.clear();
        lcd.print("inviato");
        delay(3000);
        contatore=1;
      }


      else if(statosx==HIGH){
        //messaggio
        sms.beginSMS(senderNumber);
        sms.print(txtMsg1);
        sms.endSMS();
        Serial.println("\nCOMPLETE!\n");
        lcd.clear();
        lcd.print("inviato");
        delay(3000);
        contatore=1;
      }
    }
  }
  else{
    digitalWrite(led, LOW);
    lcd.clear();
    lcd.print("In attesa");
  }

  delay(1000);

}




