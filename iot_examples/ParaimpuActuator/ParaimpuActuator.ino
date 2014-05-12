/*************************************************************************
 This sketch sets 12 digital and all 6 analog outputs of the Arduino Yùn
 according to the inputs read by actuator instance on Paraimpu.com

 Original code by Paraimpu Team ( http://www.paraimpu.com ) released under GPL v3.
 modified by Mirco Piccin on April 2014 for ArduinoTour IoT Workshop 
 @ WeMake / Milan
 
 For more details, please read http://www.gnu.org/licenses/gpl-3.0.txt
***************************************************************************/
#include <Bridge.h>
#include <HttpClient.h>

int p;
int i;
String data = String();
char c;
boolean take;
int waiting;
int n;
int v;
int digits;
int jump;

void setup() {
  for (i = 2; i < 20; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  Bridge.begin();
  Serial.begin(9600);

  delay(1000);
}

void loop() {
  HttpClient client;
  client.get("http://api.paraimpu.com/use?token=xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx");

  while (client.available()) {
    c = client.read();
    Serial.print(c);
    if (c == '}') {
      run();
      break;
    }
    else if (c == '{') {
      take = true;
    }
    else if (c == '\\' || c == ' ') {}
    else {
      if (take) {
        data.concat(String(c));
      }
    }

  }
  delay(60000);
}

void run() {
  if (data.charAt(2) == 'X' ) { //all data
    writeAll(data.charAt(7));
    data = "";
  }
  else {
    p = 0;
    n = 0;
    v = 0;
    digits = 0;
    jump = 0;
    while (p < data.length()) {
      if (data.charAt(p + 1) == 'A') {
        n = int(data.charAt(p + 2)) - 48;
        n = n + 14;
        jump = 9;
        if (data.charAt(p + 6) == 'L') {
          digitalWrite(n, LOW);
        }
        else {
          digitalWrite(n, HIGH);
        }
      }
      else {
        n = (int(data.charAt(p + 2)) - 48) * 10;
        n = n + int(data.charAt(p + 3)) - 48;
        if (data.charAt(p + 7) == 'L') {
          digitalWrite(n, LOW);
          jump = 10;
        }
        else if (data.charAt(p + 7) == 'H') {
          digitalWrite(n, HIGH);
          jump = 10;
        }
        else {
          v = 0;
          if (isDigit(data.charAt(p + 8))) {
            v = int(data.charAt(p + 8)) - 48 + (int(data.charAt(p + 7)) - 48) * 10 + (int(data.charAt(p + 6)) - 48) * 100;
            jump = 10;
          }
          else if (isDigit(data.charAt(p + 7))) {
            v = int(data.charAt(p + 7)) - 48 + (int(data.charAt(p + 6)) - 48) * 10;
            jump = 9;
          }
          else {
            v = int(data.charAt(p + 6)) - 48;
            jump = 8;
          }
          analogWrite(n, v);
        }
      }
      p = p + jump;
    }
    data = "";
  }
}

void writeAll(char out) {
  if (out == 'L') {
    for (i = 2; i < 20; i++) {
      digitalWrite(i, LOW);
    }
  }
  else {
    for (i = 2; i < 20; i++) {
      digitalWrite(i, HIGH);
    }
  }
}

boolean isDigit(char ch) {
  if (ch >= 48 && ch <= 75) {
    return true;
  }
  return false;
}
