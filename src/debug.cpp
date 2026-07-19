#include "common.h"
#include "control.h"
#include "debug.h"

void printDeviceStatus() {
    if (!enableTextOutput) return;
    DEBUG_INFO("deviceStatus = %d\n", deviceStatus);
}

void printCarStatus() {
    if (!enableTextOutput) return;
    DEBUG_INFO("carSignal = %d, carCountdown = %d\n", carSignal, carCountdown);
}

void printCarStatusTransition(LightSignal lastLightSignal) {
    if (!debug) return;
    DEBUG_INFO("changeCarSignalState: transition carSignal from %d to %d\n", lastLightSignal, carSignal);
    DEBUG_INFO("changeCarSignalState: Next Schedule: %lu", timingSchedule[0]);
}

void printPedestrianStatus() {
    if (!enableTextOutput) return;
    DEBUG_INFO("pedestrianSignal = %d, pedestrianCountdown = %d\n", pedestrianSignal, pedestrianCountdown);
}

void printCurrentInput() {
    DEBUG_INFO("currentInput = %d\n", currentInput);
}
