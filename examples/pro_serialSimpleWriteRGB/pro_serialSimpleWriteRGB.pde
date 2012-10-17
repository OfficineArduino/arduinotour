/*
  Originally created by smitec08
  http://www.instructables.com/id/Controlling-an-RGB-Led-with-Arduino-and-Processing/
*/
import processing.serial.*;

PImage img;	
Serial myPort;

void setup() {
  
  //set these to the size of the image
  size(512,512);
  
  //this is the name of your image file saved in the data folder in your
  //processing folder see processing.org for help
  
  img = loadImage("RGBR.png");
  
  //the [0] may be [another number] on your computer
  myPort = new Serial(this, "/dev/ttyUSB1", 9600);
  
}

void draw() {
  background(0);
  image(img,0,0);
  img.loadPixels();
}

void mousePressed() 
{
 myPort.write("CL");
 myPort.write(int(red(img.pixels[mouseX+mouseY*img.width])));
 myPort.write(int(green(img.pixels[mouseX+mouseY*img.width]))); 
 myPort.write(int(blue(img.pixels[mouseX+mouseY*img.width]))); 
}
  
  
/*

//Arduino code
// Output
int redPin   = 5;
int greenPin = 9;
int bluePin  = 6;

long int inByte; 
int wait = 10; //10ms

void setup()
{
  pinMode(redPin,   OUTPUT);   // sets the pins as output
  pinMode(greenPin, OUTPUT);   
  pinMode(bluePin,  OUTPUT);
  
  analogWrite(redPin, 255);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 255);

  Serial.begin(9600); 
}


void outputColour(int red, int green, int blue) {
  analogWrite(redPin,255 - red);
  analogWrite(bluePin,255 - blue);
  analogWrite(greenPin,255 - green);    
}


int* getColour() {
  int* colour;
  int i;
  
  i = 0;
  
  //for some reason it only works if we put a dud value between the C and 
  // the R value
  while (i < 4)
  {
    if (Serial.available() > 0) {
        colour[i] = Serial.read();
        i++;
    }
  }
  
  return colour;
}

// Main program
void loop()
{  
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    
     if (inByte == 'C') {
        int* one;
      one =  getColour();
      
      //1 2 3 not 0 1 2 due to the dud value
      outputColour(one[1],one[2],one[3]);    
       
    } 
  }
  
  delay(wait);
}
*/

