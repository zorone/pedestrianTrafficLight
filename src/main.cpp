#include <limits.h>

#include <Arduino.h>
#include <Arduino_DebugUtils.h>
#include "pin.h"
#include "control.h"

unsigned long timeout[] = {0, 0};
bool timeIsOver[] = {false, false};
uint8_t detectResult = 0;

void setup() {
    Serial.begin(9600);
    Debug.timestampOn();

    pinMode(begSignalPin, INPUT_PULLUP);
    pinMode(begStatusPin, OUTPUT);
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
    pinMode(motionSensorSupplyPin, OUTPUT);
    digitalWrite(motionSensorSupplyPin, HIGH);

    DEBUG_INFO("Wait for devices...");
    delay(5 * 1000ul);   // Wait for all devices to become ready. 
    timeout[0] = millis() + (10 * 1000ul);

    DEBUG_INFO("Devices ready.");
    digitalWrite(begStatusPin, LOW);
    changeCarLightSignal(green);
    changePedestrianLightSignal(red);
    changeCarCountdownSignal(green, hide);
    changePedestrianCountdownSignal(red, hide);

    DEBUG_INFO("Ready");
    while(true) {
        if(isDue(timeout[0]) && timeIsOver[0]) {
            digitalWrite(motionSensorSupplyPin, LOW);
            timeIsOver[0] = true;
        }

        detectResult = detectSignal(begSignalPin, 30);
        if(detectResult == 127) {
            digitalWrite(begStatusPin, HIGH);
            detectResult = 0;
            break;
        }
    }
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
    digitalWrite(begStatusPin, LOW);
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

    timeout[0] = millis() + 10ul * 1000;
    timeIsOver[0] = false;
    timeout[1] = millis() + 60ul * 1000;
    DEBUG_INFO("Ready");
    while(true) {
        if(isDue(timeout[0]) && timeIsOver[0]) {
            digitalWrite(motionSensorSupplyPin, LOW);
            timeIsOver[0] = true;
        }

        detectResult = detectSignal(begSignalPin, 30);
        if(detectResult == 127) {
            digitalWrite(begStatusPin, HIGH);
            detectResult = 0;
            break;
        }
    }
    DEBUG_INFO("%d", digitalRead(begSignalPin));
    while(!isDue(timeout[1])) {;}
    DEBUG_INFO("Start!");
}
