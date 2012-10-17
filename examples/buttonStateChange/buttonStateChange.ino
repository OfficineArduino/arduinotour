/*
  State change detection (edge detection)
 */

const int  buttonPin = 2;    
const int ledPin = 9;       

// current state of the button
int buttonState = 0;         
// previous state of the button
int lastButtonState = 0;     

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}


void loop() {
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, do something!
    if (buttonState == HIGH) {
      digitalWrite(ledPin, HIGH);
    } 
    else {
      digitalWrite(ledPin, LOW);
    }
  }
  // save the current state as the last state, 
  //for next time through the loop
  lastButtonState = buttonState;
}













