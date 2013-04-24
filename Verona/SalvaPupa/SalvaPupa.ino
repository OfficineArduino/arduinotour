/*
  Arduinotour 2013 @ Verona
 
  Nome Progetto: SMS x Salvare la pupa in auto
  Autori: Cristiano Zanetti
  Materiale utilizzato: Arduino Uno, StarterKit, Windows PC with GSM Shield
  Linguaggi di programmazione: Arduino, 
  Descrizione:Ho connesso alcuni sensori ad Arduino ( temperatura e sensore 
  improvvisato di umidità). Poi ho connesso lo shield GSM con scheda sim che 
  spediva un messaggio al proprietario quando i due sensori raggiungevano 
  il limite di calore e umidità che rilevavano. (ipotesi di un abitacolo di 
  un auto che potrebbe avere a bordo un bimbo lasciato dentro).
*/

/* SALVA PUPA
 Un dispositivo che analizzi umidità e calore interno all'abitacolo 
 Trasmissione sms al telefono */

const int sensorPin = A0; // sensore temp
const int VAL_PROBE = A1; // sensore umidità
const int MOISTURE_LEVEL = 70; // limite umidità
const int TEMP_LEVEL = 27;

#include <GSM.h>
#define PINNUMBER ""
// initialize the library instance
GSM gsmAccess;
GSM_SMS sms;

char remoteNum[20]="+39XXXXXXXXXX"; 
char txtMsg[200]="Troppo CaldoUmido In AUTO!!!";

void setup(){
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop(){
  int sensorVal = analogRead(sensorPin); //LEGGO LA TEMPERATURA
  int moisture = analogRead(VAL_PROBE); // LEGGO L'UMIDITA'
  int y = map(moisture, 0, 1023, 0, 100); // converto il valore da 0-1023 a 0-100
  y=100-y;

  float voltage = (sensorVal/1024.0) * 5.0; // converte il valore a un numero di tensione tra o e 5v
  // convert the voltage to temperature in degrees C
  // the sensor changes 10 mV per degree
  // the datasheet says there's a 500 mV offset
  // ((volatge - 500mV) times 100)
  Serial.print(", degrees C: "); // SERVE PER LEGGERE MEGLIO SUL MONITOR
  float temperature = (voltage - .5) * 100;
  Serial.println(temperature);
  Serial.print(", Umidita': "); // SERVE PER LEGGERE MEGLIO SUL MONITOR
  Serial.println(y);

  if (temperature > 27){
    digitalWrite(4, HIGH);
  }
  else {
    digitalWrite(4, LOW);
  } 
  if (y > MOISTURE_LEVEL){
    digitalWrite(5, HIGH);
  }
  else {
    digitalWrite(5, LOW);

  }
  delay(1000);

  if (temperature > TEMP_LEVEL && y > MOISTURE_LEVEL){

    Serial.println("SMS Messages Sender");

    // connection state
    boolean notConnected = true;

    // Start GSM shield
    // If your SIM has PIN, pass it as a parameter of begin() in quotes
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
    Serial.println("Troppo Caldooooooo In AUTO!!!!!");
    Serial.println("GSM initialized");
    // send the message
    sms.beginSMS(remoteNum);
    sms.print(txtMsg);
    sms.endSMS(); 
    Serial.println("Operazione completata !\n");
  }
}


