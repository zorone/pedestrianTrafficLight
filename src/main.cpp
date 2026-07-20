#include "common.h"
#include "pin.h"
#include "control.h"
#include "debug.h"

void setup() {
    Serial.begin(9600);
    inputTimeframe = millis() + 100;
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

    changeCarLightSignal(carSignal);
    changeCarCountdownSignal(carCountdown);
    changePedestrianLightSignal(pedestrianSignal);
    changePedestrianCountdownSignal(pedestrianCountdown);

    printDeviceStatus();
    printCarStatus();
    printPedestrianStatus();
    printCurrentInput();

    Debug.timestampOn();
    enableTextOutput = false;
}

void loop() {
    if (isDue(inputTimeframe, inputInterval)) {
        prevInput = currentInput;
        currentInput = digitalRead(begSignalPin);
        // printCurrentInput();
        if(prevInput == currentInput && currentInput == HIGH) begSignalInterrupt();
        inputTimeframe += inputInterval;
    }
    if (isDue(debuggingSchedule, debuggingInterval)) {
        enableTextOutput = true;
        debuggingSchedule += debuggingInterval;
    }
    // printDeviceStatus();
    switch (deviceStatus) {
        case ready:
            // printCarStatus();
            // printPedestrianStatus();
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
                enableTextOutput = false;
                DEBUG_INFO("Finish the sequence");
                DEBUG_INFO("Beg Signal will become available at %lu", timingSchedule[4]);
            }
            break;
        case cooldown:
            // printCarStatus();
            // printPedestrianStatus();
            if (isCooldownFinish()) {
                deviceStatus = ready;
                DEBUG_INFO("%lu is due", timingSchedule[4]);
                DEBUG_INFO("Beg signal is now available");
            }
            break;
        default:
            DEBUG_ERROR("loop(): unreachable state: %d", deviceStatus);
            unreachable();
    }
    enableTextOutput = false;
}
