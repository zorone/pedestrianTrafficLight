#include <limits.h>

#include <Arduino.h>
#include <Arduino_DebugUtils.h>
#include "pin.h"
#include "control.h"

// waitTimeBetweenCycle, motionTimeoutTime, speakerTimeoutTime, speakerPulseTime
unsigned long timeout[] = {0, 0, 0, 0};
bool timeIsOver[] = {false, false, false, false};

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
    pinMode(speakerPin, OUTPUT);
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
        if(isDue(timeout[0]) && !timeIsOver[0]) {
            digitalWrite(motionSensorSupplyPin, LOW);
            timeIsOver[0] = true;
        }

        int8_t detectSignalResult = detectSignal(begSignalPin, 12);
        DEBUG_INFO("detectSignal = %d", detectSignalResult);
        if(detectSignalResult == 127) {
            digitalWrite(begStatusPin, HIGH);
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

    delay(20 * 1000ul + 50);
    changeCarLightSignal(orange);
    changeCarCountdownSignal(orange, show);
    DEBUG_INFO("Car: Orange");

    delay(3 * 1000ul + 50);
    changeCarLightSignal(red);
    changeCarCountdownSignal(red, show);
    DEBUG_INFO("Car: Red");

    delay(2 * 1000ul + 50);
    digitalWrite(begStatusPin, LOW);
    digitalWrite(motionSensorSupplyPin, HIGH);
    digitalWrite(speakerPin, LOW);
    changePedestrianLightSignal(green);
    changePedestrianCountdownSignal(green, show);
    DEBUG_INFO("Pedestrian: Green");

    // Yield 15 seconds for pedestrian. Need to handle speaker too so it a bit messy.
    timeout[2] = millis() + 15 * 1000ul + 50;
    timeout[3] = millis() + 100ul;
    while(true) {
        if(isDue(timeout[2])) {
            digitalWrite(speakerPin, LOW);
            break;
        }
        if(isDue(timeout[3])) {
            timeout[3] = millis() + 100ul;
            digitalWrite(speakerPin, !digitalRead(speakerPin));
        }
    }

    changePedestrianLightSignal(red);
    changePedestrianCountdownSignal(red, hide);
    DEBUG_INFO("Pedestrian: Red");

    delay(2 * 1000ul + 50);
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
        if(isDue(timeout[0]) && !timeIsOver[0]) {
            digitalWrite(motionSensorSupplyPin, LOW);
            timeIsOver[0] = true;
        }

        int8_t detectSignalResult = detectSignal(begSignalPin, 12);
        if(detectSignalResult == 127) {
            digitalWrite(begStatusPin, HIGH);
            break;
        }
    }
    while(!isDue(timeout[0])) {;}
    digitalWrite(motionSensorSupplyPin, LOW);
    timeIsOver[0] = true;
    DEBUG_INFO("%d", digitalRead(begSignalPin));
    while(!isDue(timeout[1])) {;}
    DEBUG_INFO("Start!");
}
