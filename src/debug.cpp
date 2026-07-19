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

// TODO: How would I track its real function name?
void printCarStatusTransition(LightSignal lastCarSignal) {
    if (!debug) return;
    DEBUG_INFO("changeCarSignalState: transition carSignal from %d to %d\n", lastCarSignal, carSignal);
    DEBUG_INFO("changeCarSignalState: Next Schedule: %lu", timingSchedule[0]);
}

void printCarCountdownTransition(CountdownDisplay lastCarCountdown) {
    if (!debug) return;
    DEBUG_INFO("changeCarCountdownState: transition carCountdown from %d to %d\n", lastCarCountdown, carCountdown);
    DEBUG_INFO("changeCarCountdownState: Next Schedule: %lu", timingSchedule[1]);
}

void printPedestrianStatusTransition(LightSignal lastPedestrianSignal) {
    if (!debug) return;
    DEBUG_INFO("changePedestrianSignalState: transition PedestrianSignal from %d to %d\n", lastPedestrianSignal, pedestrianSignal);
    DEBUG_INFO("changePedestrianSignalState: Next Schedule: %lu", timingSchedule[2]);
}

void printPedestrianCountdownTransition(CountdownDisplay lastPedestrianCountdown) {
    if (!debug) return;
    DEBUG_INFO("changePedestrianCountdownState: transition PedestrianCountdown from %d to %d\n", lastPedestrianCountdown, pedestrianCountdown);
    DEBUG_INFO("changePedestrianCountdownState: Next Schedule: %lu", timingSchedule[3]);
}

void printPedestrianStatus() {
    if (!enableTextOutput) return;
    DEBUG_INFO("pedestrianSignal = %d, pedestrianCountdown = %d\n", pedestrianSignal, pedestrianCountdown);
}

void printCurrentInput() {
    DEBUG_INFO("currentInput = %d\n", currentInput);
}
