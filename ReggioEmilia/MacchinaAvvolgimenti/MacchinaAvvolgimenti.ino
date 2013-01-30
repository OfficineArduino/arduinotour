/*
  Macchina per avvolgimenti 
  (es. per custom pickup per CGB - Cigar Box Guitar)
  
  Developed by Andrea Amadori / Enrico Bedogni / Riccardo Bianchi /
  Nicola Giacche' / Lucio Gonzalez / Giovanni Vecchet
 
  Project slides: http://bit.ly/cbg_avv
 
  ArduinoTour 2013 @ FabLab / ReggioEmilia
*/  
  
// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(10, 7, 6, 5, 4, 2);

const int 
PWM_A   = 3,
DIR_A   = 12,
BRAKE_A = 9,
SNS_A   = A0,
POTENZIOMENTRO = A2,
SENSORE = A3,
buttonPin = A5;

int valvelocita = 0;
int flag = 0;
int valoresensore = 0;
int cont=0;
int time_start=0;
int time_loop=0;
int buttonState = 0;         
float velo;
float contspeed;
float intervallo;
float minuti;

void setup() {
  // Configure the A output
  pinMode(BRAKE_A, OUTPUT);  // Brake pin on channel A
  pinMode(DIR_A, OUTPUT);    // Direction pin on channel A
  pinMode(SENSORE, INPUT);  // Sensore di prossimità
  pinMode(buttonPin, INPUT);     
  // Open Serial communication
  Serial.begin(9600);
  Serial.println("Motor shield DC motor Test:\n");

  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Stay Metal!  m/  ");

  time_start=millis();
  intervallo=100; 
  contspeed=0;
  minuti=(60/(intervallo/1000));
}

void loop() {

  // Set the outputs to run the motor forward

  digitalWrite(BRAKE_A, LOW);  // setting brake LOW disable motor brake
  digitalWrite(DIR_A, HIGH);   // setting direction to HIGH the motor will spin forward
  valoresensore = digitalRead(SENSORE);
  if (valoresensore==LOW){
    flag=0;
  }
  else{
    if (flag==0){
      cont++;
      contspeed++;
      flag=1;
    }
  }

  valvelocita = analogRead(POTENZIOMENTRO);
  analogWrite(PWM_A, valvelocita/4);     // Set the speed of the motor, 255 is the maximum value

  time_loop=millis(); 

  if ((time_loop-time_start)>intervallo){
    time_start=millis();
    //time_loop=0;
    velo=minuti*contspeed/(intervallo/1000);
    contspeed=0;
    Serial.print("current consumption at full speed: ");
    Serial.println(analogRead(valvelocita));
    Serial.print("Contatore giri");
    Serial.println(cont);
    Serial.print("velocita: ");
    Serial.println(velo);
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print(cont);   

    buttonState = digitalRead(buttonPin);

    if (buttonState == HIGH) {     
      //if the pushbutton, check buttonFlag value
      cont=0;  
      lcd.clear();
      // set up the LCD's number of columns and rows: 
      lcd.begin(16, 2);
      // Print a message to the LCD.
      lcd.print("Stay Metal!   m/  ");
    }    
  }
}
