#include "common.h"
#include "control.h"
#include "debug.h"

void printDeviceStatus() {
    if (!enableTextOutput) return;
    Serial.print("deviceStatus = ");
    Serial.println(deviceStatus);
}

void printCarStatus() {
    if (!enableTextOutput) return;
    Serial.print("carSignal = ");
    Serial.print(carSignal);
    Serial.print(" carCountdown = ");
    Serial.println(carCountdown);
}

void printPedestrianStatus() {
    if (!enableTextOutput) return;
    Serial.print("pedestrianSignal = ");
    Serial.print(pedestrianSignal);
    Serial.print(" pedestrianCountdown = ");
    Serial.println(pedestrianCountdown);
}

void printCurrentInput() {
    Serial.print("currentInput = ");
    Serial.println(currentInput);
}
