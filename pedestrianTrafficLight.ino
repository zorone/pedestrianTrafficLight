#include "common.h"
#include "debug.h"
#include "pin.h"
#include "control.h"

DeviceState deviceStatus = ready;
LightSignal carSignal = green;
LightSignal pedestrianSignal = red;
CountdownDisplay carCountdown = hide;
CountdownDisplay pedestrianCountdown = hide;
volatile bool begSignal = false;
unsigned long timingSchedule[] = {
    0,    // 0: Time when car light signal is switched
    0,    // 1: Time when car countdown signal is switched
    0,    // 2: Time when pedestrian light signal is switched
    0,    // 3: Time when pedestrian countdown signal is switched
    0     // 4: Time when beg signal will available again
};

bool prevInput = false, currentInput = false;
unsigned long inputTimeframe = 0;

volatile unsigned long debuggingSchedule = 0;
bool enableTextOutput = true;

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
    enableTextOutput = false;
}

void loop() {
    if (isDue(inputTimeframe)) {
        prevInput = currentInput;
        currentInput = digitalRead(begSignalPin);
        printCurrentInput();
        if(prevInput == currentInput && currentInput == HIGH) begSignalInterrupt();
        inputTimeframe += 100;
    }
    if (isDue(debuggingSchedule)) {
        enableTextOutput = true;
        debuggingSchedule += 250;
    }
    printDeviceStatus();
    switch (deviceStatus) {
        case ready:
            printCarStatus();
            printPedestrianStatus();
            printCurrentInput();
            if (begSignal == true) {
                scheduleLightSignal();
                begSignal = false;
                deviceStatus = running;
                enableTextOutput = true;
            }
            break;
        case running:
            changeCarSignalState();
            printCarStatus();
            changeCarLightSignal(carSignal);
            changePedestrianSignalState();
            printCarStatus();
            changeCarCountdownSignal(carCountdown);
            changeCarCountdownState();
            printPedestrianStatus();
            changePedestrianLightSignal(pedestrianSignal);
            changePedestrianCountdownState();
            printPedestrianStatus();
            changePedestrianCountdownSignal(pedestrianCountdown);
            if (isSignalSequenceFinish()) {
                deviceStatus = cooldown;
                carCountdown = hide;
                pedestrianCountdown = hide;
                enableTextOutput = false;
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
