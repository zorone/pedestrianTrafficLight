#include "common.h"
#include "pin.h"
#include "control.h"
#include "debug.h"

void setup() {
    Serial.begin(9600);
    debuggingSchedule = millis() + 250;

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
    attachInterrupt(digitalPinToInterrupt(2), begSignalInterrupt, RISING);

    changeCarLightSignal(carSignal);
    changeCarCountdownSignal(carCountdown);
    changePedestrianLightSignal(pedestrianSignal);
    changePedestrianCountdownSignal(pedestrianCountdown);

    printDeviceStatus();
    printCarStatus();
    printPedestrianStatus();

    Debug.timestampOn();
    enableTextOutput = false;
}

void loop() {
    if (isDue(debuggingSchedule)) {
        enableTextOutput = true;
        debuggingSchedule += 250;
    }
    // printDeviceStatus();
    switch (deviceStatus) {
        case ready:
            printCarStatus();
            printPedestrianStatus();
            if (begSignal == true) {
                scheduleLightSignal();
                begSignal = false;
                deviceStatus = running;
                enableTextOutput = true;
            }
            break;
        case running:
            changeCarSignalState();
            changeCarCountdownState();
            changeCarLightSignal(carSignal);
            changeCarCountdownSignal(carCountdown);
            changePedestrianSignalState();
            changePedestrianCountdownState();
            changePedestrianLightSignal(pedestrianSignal);
            changePedestrianCountdownSignal(pedestrianCountdown);
            if (isSignalSequenceFinish()) {
                deviceStatus = cooldown;
                carCountdown = hide;
                pedestrianCountdown = hide;
                Serial.println("Finish the sequence");
            }
            break;
        case cooldown:
            printCarStatus();
            printPedestrianStatus();
            if (isCooldownFinish()) deviceStatus = ready;
            break;
        default:
            Serial.print("loop(): unreachable state: ");
            Serial.println(deviceStatus);
            unreachable();
    }
    enableTextOutput = false;
}
