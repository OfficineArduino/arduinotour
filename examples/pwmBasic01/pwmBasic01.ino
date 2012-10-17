int led = 9;

void setup() {                
  pinMode(led, OUTPUT);     
}

void loop() {
      digitalWrite(led, HIGH);
      delay(10); //then try 1 here
      digitalWrite(led, LOW); 
      delay(1);  //..and 10 here
}





