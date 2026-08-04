#include "Arduino.h"
#include "pin.h"
#include "control.h"

unsigned long now = 0;

void setup() {
    Serial.begin(9600);

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

    pulseIn(begSignalPin, LOW);
}

void loop() {
    changeCarCountdownSignal(show);
    changePedestrianCountdownSignal(show);

    delay(20 * 1000 + 50);
    changeCarLightSignal(orange);
    changeCarCountdownSignal(show);

    delay(3 * 1000 + 50);
    changeCarLightSignal(red);
    changeCarCountdownSignal(show);

    delay(2 * 1000 + 50);
    changePedestrianLightSignal(green);
    changePedestrianCountdownSignal(show);

    delay(15 * 1000 + 50);
    changePedestrianLightSignal(red);
    changePedestrianCountdownSignal(hide);

    delay(2 * 1000 + 50);
    changeCarLightSignal(green);
    changeCarCountdownSignal(hide);

    now = millis();
    pulseIn(begSignalPin, LOW);
    while(!isDue(now)) {;}
}
