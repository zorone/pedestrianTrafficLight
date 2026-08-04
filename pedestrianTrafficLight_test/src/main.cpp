#include <Arduino.h>
#include <Arduino_DebugUtils.h>

#include "pin.h"

char mode = -1; 

char* travel[] = {
  "car",
  "pedestrian"
}

char* outputType[] = {
  "light",
  "countdown"
}

char* light[] = {
  "GREEN",
  "ORANGE",
  "RED"
}

char+ signal[] = {
  "off",
  "on"
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(begSignalPin, OUTPUT);
  pinMode(pedestrianLightPin_GREEN, INPUT_PULLUP);
  pinMode(pedestrianLightPin_RED, INPUT_PULLUP);
  pinMode(pedestrianCountdownPin_GREEN, INPUT_PULLUP);
  pinMode(pedestrianCountdownPin_RED, INPUT_PULLUP);
  pinMode(carLightPin_GREEN, INPUT_PULLUP);
  pinMode(carLightPin_ORANGE, INPUT_PULLUP);
  pinMode(carLightPin_RED, INPUT_PULLUP);
  pinMode(carCountdownPin_GREEN, INPUT_PULLUP);
  pinMode(carCountdownPin_ORANGE, INPUT_PULLUP);
  pinMode(carCountdownPin_RED, INPUT_PULLUP);

  Debug.timestampOn();

  while(!Serial) {;}
  Serial.println("Select testing mode, press enter to test from the start");
}

void loop() {
  if(Serial.available() <= 0) return;
  Serial.read(mode);

  switch(mode) {
    case '\n':
      normalTest();
    default: return;
  }
}

void normalTest() {
  digitalWrite(begSignalPin, HIGH);
  DEBUG_INFO("%s: %s: %s: %s",
    travel[0], outputType[1], light[0],
    signal[digitalRead(carCountdownPin_GREEN)]);
  DEBUG_INFO("%s: %s: %s: %s",
    travel[1], outputType[1], light[2],
    signal[digitalRead(carCountdownPin_RED)]);
  
  DEBUG_INFO("%s: %s: %s: %lu", 
    travel[0], outputType[1], light[0],
    pulseIn(carCountdownPin_GREEN, HIGH));
}