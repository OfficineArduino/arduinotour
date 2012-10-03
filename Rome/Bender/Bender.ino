/*
  Bender
  Show me the way to the next whisky bar
 
  Developed by  Antonio / Luisa / Danilo / Yuri during 
  Arduinotour 2012 @ Rome  
*/


int sensorPinSX = A0;
int sensorPinC = A1;
int sensorPinDX = A2;

const int motor1Pin = 9;
const int motor2Pin = 10;

int speedMotor01 = 80;
int speedMotor02 = 80;

int sensorValueSX = 0;
int sensorValueC = 0;
int sensorValueDX = 0;

int zeroValueSX;
int zeroValueDX;
int unoValueC;

int ledPin1 = 3;
int ledPin2 = 4;

int alcoolSXPin = 7;
int alcoolDXPin = 8;

int alcoolValueSX=0;
int alcoolValueDX=0;

int alcoolSXOut = 5;
int alcoolDXOut = 6;

//Se modalit == 0 segue la striscia bianca e si ferma in prossimita' di un incrocio
//Se modalit == 1 impazzisce, va sempre dritto e curva dal lato in cui rileva alcool
int modalit = 0;


void setup() {

  Serial.begin(9600);

  pinMode(ledPin1, OUTPUT);  
  digitalWrite(ledPin1, HIGH);

  pinMode(ledPin2, OUTPUT);  
  digitalWrite(ledPin2, HIGH);

  pinMode(alcoolDXPin, OUTPUT);  
  digitalWrite(alcoolDXPin, HIGH);

  pinMode(alcoolSXPin, OUTPUT);  
  digitalWrite(alcoolSXPin, HIGH);

  pinMode(alcoolSXOut, INPUT);
  pinMode(alcoolDXOut, INPUT);

  pinMode(motor1Pin, OUTPUT);
  pinMode(motor2Pin, OUTPUT);

  delay(2000);

  //Prima di partire setta i valori relativi allo zero e all'uno
  inizializza();

}

void loop() {

  if(modalit==0){

    analogWrite(motor1Pin, speedMotor01);
    analogWrite(motor2Pin, speedMotor02);

    sensorValueDX = analogRead(sensorPinDX);
    sensorValueC = analogRead(sensorPinC);
    sensorValueSX = analogRead(sensorPinSX);


    alcoolValueSX = digitalRead(alcoolSXOut);
    alcoolValueDX = digitalRead(alcoolDXOut);

    sensorValueDX = map(sensorValueDX, 450, 900, 0, 255);
    sensorValueC = map(sensorValueC, 500, 885, 0, 255);
    sensorValueSX = map(sensorValueSX, 450, 900, 0, 255);

    //come andare dritto
    if(abs(unoValueC-sensorValueC)>40 ){
      //caso in cui sta uscendo dal binario centrale
      if(abs(zeroValueSX-sensorValueSX)>25){
        //se sta uscendo verso sinista
        // Serial.println("\t Stai uscendo verso sinistra");
        speedMotor01 = 67;
        speedMotor02 = 80;
      }
      else if(abs(zeroValueDX-sensorValueDX)>15){
        //se sta uscendo verso destra
        // Serial.println("\t Stai uscendo verso destra");
        speedMotor01 = 80;
        speedMotor02 = 67;
      }
    }
    else {
      // Serial.println("\t Vado drittooo!!!!");
      speedMotor01 = 80;
      speedMotor02 = 80;

    }

    if((abs(sensorValueC-250)<20)&&(abs(sensorValueSX-109)<20)&&(abs(sensorValueDX-180)<20)) {
      //sono in prossimita' di un incrocio, blocco le ruote
      speedMotor01 = 0;
      speedMotor02 = 0;
      analogWrite(motor1Pin, speedMotor01);
      analogWrite(motor2Pin, speedMotor02);

      while(true){
        //mi metto in attesa di alcool, se lo rilevo effettuo la curva e riprendo a seguire il binario
        alcoolValueSX = digitalRead(alcoolSXOut);
        alcoolValueDX = digitalRead(alcoolDXOut);
        if(alcoolValueSX==1){
          speedMotor01 = 80;
          analogWrite(motor1Pin, speedMotor01);
          modalit=1;
          delay(1500);
          speedMotor02 = 80;
          analogWrite(motor2Pin, speedMotor02);
          break;
        }
        if(alcoolValueDX==1){
          speedMotor02 = 80;
          analogWrite(motor2Pin, speedMotor02);
          delay(1500);
          speedMotor01 = 80;
          analogWrite(motor1Pin, speedMotor01);
          modalit=1;
          break;
        }
        delay(100);
      }
    }
  }
  else {
    //modalita' impazzisce e va alla ricerca di alcool
    speedMotor01 = 160;
    speedMotor02 = 160;
    analogWrite(motor1Pin, speedMotor01);
    analogWrite(motor2Pin, speedMotor02);

    alcoolValueSX = digitalRead(alcoolSXOut);
    alcoolValueDX = digitalRead(alcoolDXOut);
    if(alcoolValueSX==1){
      speedMotor01 = 80;
      analogWrite(motor1Pin, speedMotor01);
      delay(1500);
      speedMotor02 = 80;
      analogWrite(motor2Pin, speedMotor02);
    }
    if(alcoolValueDX==1){
      speedMotor02 = 80;
      analogWrite(motor2Pin, speedMotor02);
      delay(1500);
      speedMotor01 = 80;
      analogWrite(motor1Pin, speedMotor01);
    }
  }
  delay(50);                  
}

void inizializza(){

  sensorValueDX = analogRead(sensorPinDX);
  sensorValueC = analogRead(sensorPinC);
  sensorValueSX = analogRead(sensorPinSX);

  zeroValueDX = map(sensorValueDX, 450, 900, 0, 255);
  unoValueC = map(sensorValueC, 400, 885, 0, 255);
  zeroValueSX = map(sensorValueSX, 400, 900, 0, 255);

}

