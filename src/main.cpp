#include <limits.h>

#include <Arduino.h>
#include <Arduino_DebugUtils.h>
#include "pin.h"
#include "control.h"

unsigned long timeout = 0;

void setup() {
    Serial.begin(9600);
    Debug.timestampOn();

    pinMode(begSignalPin, INPUT_PULLUP);
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

    changeCarLightSignal(green);
    changePedestrianLightSignal(red);
    changeCarCountdownSignal(green, hide);
    changePedestrianCountdownSignal(red, hide);

    DEBUG_INFO("Ready");
    while(digitalRead(begSignalPin) != HIGH) {;}
    DEBUG_INFO("%d", digitalRead(begSignalPin));
    DEBUG_INFO("Start!");
}

void loop() {
    changeCarCountdownSignal(green, show);
    changePedestrianCountdownSignal(red, show);

    delay(20 * 1000 + 50);
    changeCarLightSignal(orange);
    changeCarCountdownSignal(orange, show);

    delay(3 * 1000 + 50);
    changeCarLightSignal(red);
    changeCarCountdownSignal(red, show);

    delay(2 * 1000 + 50);
    changePedestrianLightSignal(green);
    changePedestrianCountdownSignal(green, show);

    delay(15 * 1000 + 50);
    changePedestrianLightSignal(red);
    changePedestrianCountdownSignal(red, hide);

    delay(2 * 1000 + 50);
    changeCarLightSignal(green);
    changeCarCountdownSignal(green, hide);

    timeout = millis() + 60ul * 1000;
    DEBUG_INFO("Ready");
    while(digitalRead(begSignalPin) != HIGH) {;}
    DEBUG_INFO("%d", digitalRead(begSignalPin));
    while(!isDue(timeout)) {;}
    DEBUG_INFO("Start!");
}
