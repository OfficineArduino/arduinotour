int led = 9;

void setup() {                
  pinMode(led, OUTPUT);     
}

void loop() {
  for (int i=0; i < 10; i++) {
    for (int y=0; y < 100; y++) {
      digitalWrite(led, HIGH);
      delay(i);       // 0, 1, 2, 3 ...       
      digitalWrite(led, LOW); 
      delay(10 -i );  // 10, 9, 8, 7 ...
    } 
  }
}
