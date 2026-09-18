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

    DEBUG_INFO("Wait for devices...");
    delay(5 * 1000ul);   // Wait for all devices to become ready. 

    DEBUG_INFO("Devices ready.");
    changeCarLightSignal(green);
    changePedestrianLightSignal(red);
    changeCarCountdownSignal(green, hide);
    changePedestrianCountdownSignal(red, hide);

    DEBUG_INFO("Ready");
    detectSignal(begSignalPin, 30);
    DEBUG_INFO("%d", digitalRead(begSignalPin));
}

void loop() {
    DEBUG_INFO("Start!");
    
    changeCarCountdownSignal(green, show);
    changePedestrianCountdownSignal(red, show);
    DEBUG_INFO("Turn on Countdown");
    DEBUG_INFO("Car: Green");

    delay(20 * 1000 + 50);
    changeCarLightSignal(orange);
    changeCarCountdownSignal(orange, show);
    DEBUG_INFO("Car: Orange");

    delay(3 * 1000 + 50);
    changeCarLightSignal(red);
    changeCarCountdownSignal(red, show);
    DEBUG_INFO("Car: Red");

    delay(2 * 1000 + 50);
    changePedestrianLightSignal(green);
    changePedestrianCountdownSignal(green, show);
    DEBUG_INFO("Pedestrian: Green");

    delay(15 * 1000 + 50);
    changePedestrianLightSignal(red);
    changePedestrianCountdownSignal(red, hide);
    DEBUG_INFO("Pedestrian: Red");

    delay(2 * 1000 + 50);
    changeCarLightSignal(green);
    changeCarCountdownSignal(green, hide);
    DEBUG_INFO("Car: Green");
    DEBUG_INFO("Turn off Countdown");
    DEBUG_INFO("Finish Sequence");

    timeout = millis() + 60ul * 1000;
    DEBUG_INFO("Ready");
    detectSignal(begSignalPin, 30);
    DEBUG_INFO("%d", digitalRead(begSignalPin));
    while(!isDue(timeout)) {;}
    DEBUG_INFO("Start!");
}
