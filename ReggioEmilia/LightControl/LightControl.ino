/*

 Light control(proximity sensor, giroscopio)
 Controlliamo il colore della luce emesso da una striscia di LED con un sensore di prossimità e un giroscopio.
 
 This Project is created by
 Elisa Sperotto, Giacomo Boffo, Raffaele D'eredità, Massimiliano Govoni, 
 Jacopo Peretti, Gianluca Marraccini ,Alessandro Marraccini.
 27/01/2013
 
 ArduinoTour 2013 @ FabLab / ReggioEmilia
 
*/

//Dichiariamo le variabili
int brightnessred = 0;        //Definsco la luminosità del colore rosso
int brightnessgreen = 0;        //Definsco la luminosità del colore verde
int brightnessblue = 0;        //Definsco la luminosità del colore blu

//Dichiariamo le costanti del giroscopio e del sensore di prossimità
const int red = 3;              //Definisco il pin del colore rosso         
const int green = 5;              //Definisco il pin del colore verde 
const int blue = 6;               //Definisco il pin del colore blu

const int analogInPinX = A1;            //Definisco l'entrata della coordinata X
const int analogInPinY = A2;            //Definisco l'entrata della coordinata Y
const int analogInPinZ = A0;            //Definisco l'entrata della coordinata Z

void setup()  { 
  pinMode(red, OUTPUT);   //Definiamo il pin che comanda il colore rosso come uscita dati
  pinMode(green, OUTPUT);   //Definiamo il pin che comanda il colore verde come uscita dati
  pinMode(blue, OUTPUT);    //Definiamo il pin che comanda il colore blu come uscita dati
  Serial.begin(9600);       //Inizia le comunicazioni con la seriale a 9600 bps
} 

void loop()  { 

  brightnessred = analogRead(analogInPinX);       //Legge il valore della X e ne assegna una tonalità di verde          
  brightnessgreen = analogRead(analogInPinY);         //Legge il valore della X e ne assegna una tonalità di rosso
  brightnessblue = analogRead(analogInPinZ);            //Legge il valore della X e ne assegna una tonalità di blu

  brightnessred = constrain(brightnessred, 370, 595);    //forza i valori di brightness1 a restare nei limiti imposti
  brightnessgreen = constrain(brightnessgreen, 370, 595);    //forza i valori di brightness2 a restare nei limiti imposti

  brightnessred = map(brightnessred, 370, 595, 0, 255);  //adatta i valori di ingresso su un intervallo 0-255(colori)
  brightnessgreen = map(brightnessgreen, 370, 595, 0, 255);  //adatta i valori di ingresso su un intervallo 0-255
  brightnessblue = map(brightnessblue, 0, 1023, 0, 255);   //adatta i valori di ingresso su un intervallo 0-1023(coordinate) e 0-255 

    //Arduino scrive le coordinate 
  Serial.print("Y= ");                  //Scrive i valori della coordinata Y sul monitor di sistema
  Serial.print(brightnessred);            //Determina i valori della Y
  Serial.print("     X= ");             //Scrive i valori della coordinata X sul monitor di sistema            
  Serial.print(brightnessgreen);            //Determina i valori della X
  Serial.print("     Z= ");             //Scrive i valori della coordinata Z sul monitor di sistema
  Serial.println(brightnessblue);          //Determina i valori della Z e va a capo

  //Scrivo sulle uscite la luminosità voluta
  analogWrite(red, brightnessred);
  analogWrite(green, brightnessgreen);
  analogWrite(blue, brightnessblue);

  // ogni 100 millisecondi mi aggiorna i valori delle coordinate
  delay(100);                            
}

