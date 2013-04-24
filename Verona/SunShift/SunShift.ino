/*
  Arduinotour 2013 @ Verona
 
  Nome Progetto: SunShift, controllo di uno specchio solare 
  (aka “Raggio della Morte”)
  Autori: Gli Ingnoranti <<Marco Rossetti, Manolo Bossi, Giovanni Signori, 
  Andrea De Muri,.........>>
  Materiale utilizzato: Arduino Uno, StarterKit, 
  Linguaggi di programmazione: Arduino, 
  Descrizione: Prototipo per riflettere il sole dentro casa con 
  uno specchio sulla casa di fronte, utilizzabile anche per deviare 
  il raggio della morte ai vicini.
 
  work in progress, qui di seguito un servo punta i sensori di 
  luminosità verso la luce e uno specchio verso un punto definito, 
  il codice non è il copincollato usato durante il workshop ma 
  uno sviluppo scritto con l’ausilio di una tastiera e dei concetti 
  appresi durante il corso.
*/

#include <Servo.h>
Servo Sensore;
Servo Specchio;
int angleSensore=90;		// angolo servo sensori
int angleSpecchio=90;	// angolo servo specchio
int comp=0;			// valore per calibrare lo specchio
const int sAp=A4;		// sensore A pin
const int sBp=A5;		// sensore B pin
int vA=0;			// valore sensore A
int vB=0;			// valore sensore B
void setup(){
  Sensore.attach(9);
  Specchio.attach(10);
}
void loop(){
  vA=analogRead(sAp);
  delay(5);
  vB=analogRead(sBp);
  delay(5);
  if(vA>vB){
    angleSensore=angleSensore+1;
  }	//  C O R E
  else
  {
    angleSensore=angleSensore-1;
  }	
  if(angleSensore<0){
    angleSensore=0;
  }
  if(angleSensore>180){
    angleSensore=180;
  }
  angleSpecchio=anglesensore/2+comp;
  Sensore.write(angleSensore);
  delay(15);
  Specchio.write(angleSpecchio);
  delay(15);
}

/*
Funsia !
 Molto sensibile, preciso e irrequieto senza movimenti inconsulti
*/


