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

char* signal[] = {
  "off",
  "on"
}

char lightPin[2][2][3] = {
  {
    {carLightPin_GREEN, carLightPin_ORANGE, carLightPin_RED},
    {carCountdownPin_GREEN, carCountdownPin_ORANGE, carCountdownPin_RED}
  },
  {
    {pedestrianLightPin_GREEN, 0, pedestrianLightPin_RED},
    {pedestrianCountdownPin_GREEN, 0, pedestrianCountdownPin_RED}
  }
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
    signal[digitalRead(lightPin[0][1][0])]);
  DEBUG_INFO("%s: %s: %s: %s",
    travel[1], outputType[1], light[2],
    signal[digitalRead(lightPin[1][1][2])]);

  DEBUG_INFO("%s: %s: %s: %lu", 
    travel[0], outputType[0], light[0],
    pulseIn(lightPin[0][0][0], HIGH));

  DEBUG_INFO("%s: %s: %s: %s",
    travel[0], outputType[1], light[1],
    signal[digitalRead(lightPin[0][1][1])]);
  DEBUG_INFO("%s: %s: %s: %lu", 
    travel[0], outputType[0], light[1],
    pulseIn(lightPin[0][0][1], HIGH));

  DEBUG_INFO("%s: %s: %s: %s",
    travel[0], outputType[0], light[2],
    signal[digitalRead(lightPin[0][0][2])]);
  DEBUG_INFO("%s: %s: %s: %s",
    travel[0], outputType[1], light[2],
    signal[digitalRead(lightPin[0][1][2])]);
  DEBUG_INFO("%s: %s: %s: %lu", 
    travel[1], outputType[0], light[2],
    pulseIn(lightPin[1][0][2], HIGH));

  DEBUG_INFO("%s: %s: %s: %s",
    travel[1], outputType[1], light[0],
    signal[digitalRead(lightPin[1][1][0])]);
  DEBUG_INFO("%s: %s: %s: %lu", 
    travel[1], outputType[0], light[0],
    pulseIn(lightPin[1][0][0], HIGH));

  DEBUG_INFO("%s: %s: %s: %s",
    travel[1], outputType[0], light[2],
    signal[digitalRead(lightPin[1][0][2])]);
  DEBUG_INFO("%s: %s: %s: %s",
    travel[1], outputType[1], light[2],
    signal[digitalRead(lightPin[1][1][2])]);
  DEBUG_INFO("%s: %s: %s: %lu", 
    travel[0], outputType[0], light[2],
    pulseIn(lightPin[0][0][2], HIGH));

  DEBUG_INFO("%s: %s: %s: %s",
    travel[0], outputType[0], light[0],
    signal[digitalRead(lightPin[0][0][0])]);
  DEBUG_INFO("%s: %s: %s: %s",
    travel[0], outputType[1], light[0],
    signal[digitalRead(lightPin[1][0][0])]);

  DEBUG_INFO("%s: %s: %s: %lu", 
    travel[0], outputType[1], light[0],
    pulseIn(lightPin[0][1][0], LOW));
  
  DEBUG_INFO("%s: %s: %s: %s",
    travel[0], outputType[1], light[0],
    signal[digitalRead(lightPin[0][1][0])]);
  DEBUG_INFO("%s: %s: %s: %s",
    travel[1], outputType[1], light[2],
    signal[digitalRead(lightPin[1][1][2])]);

  digitalWrite(begSignalPin, LOW);
  DEBUG_INFO("%s: %s: %s: %lu", 
    travel[0], outputType[0], light[0],
    pulseIn(lightPin[0][0][0], HIGH));

  DEBUG_INFO("%s: %s: %s: %s",
    travel[0], outputType[1], light[1],
    signal[digitalRead(lightPin[0][1][1])]);
  DEBUG_INFO("%s: %s: %s: %lu", 
    travel[0], outputType[0], light[1],
    pulseIn(lightPin[0][0][1], HIGH));

  DEBUG_INFO("%s: %s: %s: %s",
    travel[0], outputType[0], light[2],
    signal[digitalRead(lightPin[0][0][2])]);
  DEBUG_INFO("%s: %s: %s: %s",
    travel[0], outputType[1], light[2],
    signal[digitalRead(lightPin[0][1][2])]);
  DEBUG_INFO("%s: %s: %s: %lu", 
    travel[1], outputType[0], light[2],
    pulseIn(lightPin[1][0][2], HIGH));

  DEBUG_INFO("%s: %s: %s: %s",
    travel[1], outputType[1], light[0],
    signal[digitalRead(lightPin[1][1][0])]);
  DEBUG_INFO("%s: %s: %s: %lu", 
    travel[1], outputType[0], light[0],
    pulseIn(lightPin[1][0][0], HIGH));

  DEBUG_INFO("%s: %s: %s: %s",
    travel[1], outputType[0], light[2],
    signal[digitalRead(lightPin[1][0][2])]);
  DEBUG_INFO("%s: %s: %s: %s",
    travel[1], outputType[1], light[2],
    signal[digitalRead(lightPin[1][1][2])]);
  DEBUG_INFO("%s: %s: %s: %lu", 
    travel[0], outputType[0], light[2],
    pulseIn(lightPin[0][0][2], HIGH));

  DEBUG_INFO("%s: %s: %s: %s",
    travel[0], outputType[0], light[0],
    signal[digitalRead(lightPin[0][0][0])]);
  DEBUG_INFO("%s: %s: %s: %s",
    travel[0], outputType[1], light[0],
    signal[digitalRead(lightPin[1][0][0])]);
}