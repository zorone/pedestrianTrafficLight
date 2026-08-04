#include <Arduino.h>

int mode = -1; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial) {
    ;
  }
  Serial.println("Select testing mode, press enter to testing from the start");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0) {
    
  }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}