#include <limits.h>

#include <Arduino.h>
#include <Arduino_DebugUtils.h>

#define pedestrianLightPin_GREEN 4
#define pedestrianLightPin_RED 5
#define pedestrianCountdownPin_GREEN 6
#define pedestrianCountdownPin_RED 7

#define carLightPin_GREEN 8
#define carLightPin_ORANGE 9
#define carLightPin_RED 10
#define carCountdownPin_GREEN 11
#define carCountdownPin_ORANGE 12
#define carCountdownPin_RED 13

unsigned long timeout = 0;

void setup() {
    Serial.begin(9600);
    Debug.timestampOn();

    pinMode(pedestrianLightPin_GREEN, OUTPUT);
    pinMode(pedestrianLightPin_RED, OUTPUT);
    pinMode(pedestrianCountdownPin_GREEN, OUTPUT);
    pinMode(pedestrianCountdownPin_RED, OUTPUT);
    pinMode(carLightPin_GREEN, OUTPUT);
    pinMode(carLightPin_ORANGE, OUTPUT);
    pinMode(carLightPin_RED, OUTPUT);
    pinMode(carCountdownPin_GREEN, OUTPUT);
    pinMode(carCountdownPin_ORANGE, OUTPUT);
    pinMode(carCountdownPin_RED, OUTPUT);

    digitalWrite(pedestrianLightPin_GREEN, HIGH);
    digitalWrite(pedestrianLightPin_RED, HIGH);
    digitalWrite(pedestrianCountdownPin_GREEN, HIGH);
    digitalWrite(pedestrianCountdownPin_RED, HIGH);
    digitalWrite(carLightPin_GREEN, HIGH);
    digitalWrite(carLightPin_ORANGE, HIGH);
    digitalWrite(carLightPin_RED, HIGH);
    digitalWrite(carCountdownPin_GREEN, HIGH);
    digitalWrite(carCountdownPin_ORANGE, HIGH);
    digitalWrite(carCountdownPin_RED, HIGH);
}

void loop() {
  
}
