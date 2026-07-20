#include "common.h"
#include "pin.h"
#include "control.h"
#include "debug.h"

DeviceState deviceStatus = ready;
LightSignal carSignal = green;
LightSignal pedestrianSignal = red;
CountdownDisplay carCountdown = hide;
CountdownDisplay pedestrianCountdown = hide;
volatile bool begSignal = false;
volatile unsigned long timingSchedule[] = {
    0,    // 0: Time when car light signal is switched
    0,    // 1: Time when car countdown signal is switched
    0,    // 2: Time when pedestrian light signal is switched
    0,    // 3: Time when pedestrian countdown signal is switched
    0     // 4: Time when beg signal will available again
};
volatile unsigned long timingDuration[] = {
    0,    // 0: Time when car light signal is switched
    0,    // 1: Time when car countdown signal is switched
    0,    // 2: Time when pedestrian light signal is switched
    0,    // 3: Time when pedestrian countdown signal is switched
    0     // 4: Time when beg signal will available again
};

bool prevInput = false, currentInput = false;
unsigned long inputTimeframe = 0, inputInterval = 100;

volatile unsigned long debuggingSchedule = 0, debuggingInterval = 250;
bool debug = true;
bool enableTextOutput = debug;

void scheduleLightSignal() {
    DEBUG_INFO("Beg signal is registered");
    DEBUG_INFO("Schedule Light Signal");
    unsigned long now = millis();
    timingDuration[0] =  20 * 1000 + 50;
    timingDuration[1] =   0;
    timingDuration[2] =  25 * 1000 + 2 * 50;
    timingDuration[3] =   0 * 1000 + 50;
    timingDuration[4] = 100ul * 1000ul + 3 * 50;
    timingSchedule[0] = now + timingDuration[0];    // Set due time for light signal to become orange, padding for briefly show '0'
    timingSchedule[1] = now + timingDuration[1];    // Set due time to start car countdown
    timingSchedule[2] = now + timingDuration[2];    // Set due time for pedestrian light to become green, need to pad signal down as it need to account for orange light of car traffic
    timingSchedule[3] = now + timingDuration[3];    // Set due time to start pedestrian countdown, need to pad down due to orange signal from car traffic
    timingSchedule[4] = now + timingDuration[4];    // Set due time for next beg signal, padding for brief '0' of each light signal
    DEBUG_INFO("timingSchedule[0] = %lu", timingSchedule[0]);
    DEBUG_INFO("timingSchedule[1] = %lu", timingSchedule[1]);
    DEBUG_INFO("timingSchedule[2] = %lu", timingSchedule[2]);
    DEBUG_INFO("timingSchedule[3] = %lu", timingSchedule[3]);
    DEBUG_INFO("timingSchedule[4] = %lu", timingSchedule[4]);
}

void changeCarSignalState() {
    if (isDue(timingSchedule[0], timingDuration[0])) { 
        LightSignal lastCarSignal = carSignal;
        switch (carSignal) {
            case green:
                timingDuration[0] +=  3 * 1000 + 50;
                timingSchedule[0] += timingDuration[0];
                carSignal = orange;
                break;
            case orange:
                timingDuration[0] += 19 * 1000 + 50;
                timingSchedule[0] += timingDuration[0];
                carSignal = red;
                break;
            case red:
                timingSchedule[0] =  timingSchedule[4];  // Set to finishing time, to circumvent from rerunning this function before reaching the end of cycle
                carSignal = green;
                break;
            default:
                DEBUG_ERROR("changeCarSignalState: unreachable state: %d", carSignal);
                unreachable();
        }
        printCarStatusTransition(lastCarSignal);
    }
}

void changeCarCountdownState() {
    if (isDue(timingSchedule[1], timingDuration[1])) {
        LightSignal lastCarSignal = carSignal;
        CountdownDisplay lastCarCountdown = carCountdown;
        switch(carCountdown) {
            case hide:
                switch (carSignal) {
                    case green:
                        carCountdown = show;
                        timingSchedule[1] = timingSchedule[0];    // Assume that changeCarCountdownState happens after changeCarSignalState()
                        break;
                    case red:
                        break;   // transition back phase, skip once
                    case orange: // Assume that changeCarSignalState() happens beforehand, and doesn't get changed anywhere else.
                    default:
                        DEBUG_ERROR("changeCarCountdownState: unreachable state: %d %d", carSignal, carCountdown);
                        unreachable();
                }
                break;
            case show:
                {
                    switch (carSignal) {
                        case green:
                            break;                                    // transition once
                        case orange:
                            timingSchedule[1] = timingSchedule[0];    // Assume that carCountdown signal doesn't get changed anywhere else
                            break;
                        case red:
                            timingSchedule[1] = timingSchedule[4];    // Return to idle state, set to end of cycle time
                            carCountdown = hide;
                            break;
                        default:
                            DEBUG_ERROR("changeCarCountdownState: unreachable state: %d %d", carSignal, carCountdown);
                            unreachable();
                    }
                }
                break;
            default:
                DEBUG_ERROR("changeCarCountdownState: unreachable state: %d", carCountdown);
                unreachable();
        }
        printCarStatusTransition(lastCarSignal);
        printCarCountdownTransition(lastCarCountdown);
    }
}

void changePedestrianSignalState() {
    if (isDue(timingSchedule[2], timingDuration[2])) {
        LightSignal lastPedestrianSignal = pedestrianSignal;
        switch (pedestrianSignal) {
            case red:
                timingDuration[2] += 15 * 1000 + 50;
                timingSchedule[2] += timingDuration[2];
                pedestrianSignal = green;
                break;
            case green:
                timingSchedule[2] = timingSchedule[4];
                pedestrianSignal = red;
                break;
            case orange:
            default:
                DEBUG_ERROR("changePedestrianSignalState: unreachable state: %d", pedestrianSignal);
                unreachable();
        }
        printPedestrianStatusTransition(lastPedestrianSignal);
    }
}

void changePedestrianCountdownState() {
    if (isDue(timingSchedule[3], timingDuration[3])) {
        LightSignal lastPedestrianSignal = pedestrianSignal;
        CountdownDisplay lastPedestrianCountdown = pedestrianCountdown;
        switch (pedestrianCountdown) {
            case hide:
                switch (pedestrianSignal) {
                    case red:
                        timingSchedule[3] = timingSchedule[2];    // Assume that changePedestrianCountdownState happens after changePedestrianSignalState()
                        pedestrianCountdown = show;
                        break;
                    case green:
                    case orange:    // Assume that changePedestrianSignalState happens beforehand, and doesn't get changed anywhere else.
                    default:
                        DEBUG_ERROR("changePedestrianCountdownState: unreachable state: %d %d", pedestrianSignal, pedestrianCountdown);
                        unreachable();
                }
                break;
            case show:
                switch (pedestrianSignal) {
                    case green:
                        timingSchedule[3] = timingSchedule[4]; // Return to idle state, set to end cycle time
                        pedestrianCountdown = hide;
                        break;
                    case red:
                        break;  // transition back phase, skip once
                    case orange:
                    default:
                        DEBUG_ERROR("changePedestrianCountdownState(): unreachable state: %d %d", pedestrianSignal, pedestrianCountdown);
                        unreachable();
                }
                break;
            default:
                DEBUG_ERROR("changePedestrianCountdownState(): unreachable state: %d", pedestrianCountdown);
                unreachable();
        }
        printPedestrianStatusTransition(lastPedestrianSignal);
        printPedestrianCountdownTransition(lastPedestrianCountdown);
    }
}

void changeCarLightSignal(LightSignal signal) {
    for (int pin = 8; pin <= 10; pin++) {
        digitalWrite(pin, LOW);
    }
    switch (signal) {
        case green: digitalWrite(carLightPin_GREEN, HIGH); break;
        case orange: digitalWrite(carLightPin_ORANGE, HIGH); break;
        case red: digitalWrite(carLightPin_RED, HIGH); break;
        default:
            DEBUG_ERROR("changeCarLightSignal(): unreachable state: %d", signal);
            unreachable();
    }
}

void changeCarCountdownSignal(CountdownDisplay signal) {
    for (int pin = 11; pin <= 13; pin++) digitalWrite(pin, LOW);
    switch (signal) {
        case hide:
            switch (carSignal) {
                case green: digitalWrite(carCountdownPin_GREEN, LOW); break;
                case orange: digitalWrite(carCountdownPin_ORANGE, LOW); break;
                case red: digitalWrite(carCountdownPin_RED, LOW); break;
                default:
                    DEBUG_ERROR("changeCarCountdownSignal: unreachable state: %d %d", carSignal, signal);
                    unreachable();
            }
            break;
        case show:
            switch (carSignal) {
                case green: digitalWrite(carCountdownPin_GREEN, HIGH); break;
                case orange: digitalWrite(carCountdownPin_ORANGE, HIGH); break;
                case red: digitalWrite(carCountdownPin_RED, HIGH); break;
                default:
                    DEBUG_ERROR("changeCarCountdownSignal: unreachable state: %d %d", carSignal, signal);
                    unreachable();
            }
            break;
        default:
            DEBUG_ERROR("changeCarCountdownSignal(): unreachable state: %d", signal);
            unreachable();
    }
}

void changePedestrianLightSignal(LightSignal signal) {
    for (int pin = 4; pin <= 5; pin++) digitalWrite(pin, LOW);
    switch (signal) {
        case green: digitalWrite(pedestrianLightPin_GREEN, HIGH); break;
        case red: digitalWrite(pedestrianLightPin_RED, HIGH); break;
        case orange:
        default:
            DEBUG_ERROR("changePedestrianLightSignal: unreachable state: %d", signal);
            unreachable();
    }
}

void changePedestrianCountdownSignal(CountdownDisplay signal) {
    for (int pin = 6; pin <= 7; pin++) digitalWrite(pin, LOW);
    switch (signal) {
        case hide:
            switch (pedestrianSignal) {
                case green: digitalWrite(pedestrianCountdownPin_GREEN, LOW); break;
                case red: digitalWrite(pedestrianCountdownPin_RED, LOW); break;
                case orange:
                default:
                    DEBUG_ERROR("changePedestrianCountdownSignal: unreachable state: %d %d", pedestrianSignal, signal);
                    unreachable();
            }
            break;
        case show:
            switch (pedestrianSignal) {
                case green: digitalWrite(pedestrianCountdownPin_GREEN, HIGH); break;
                case red: digitalWrite(pedestrianCountdownPin_RED, HIGH); break;
                case orange:
                default:
                    DEBUG_ERROR("changePedestrianCountdownSignal: unreachable state: %d %d", pedestrianSignal, signal);
                    unreachable();
            }
            break;
        default:
            DEBUG_ERROR("changePedestrianCountdownSignal: unreachable state: %d", signal);
            unreachable();
    }
}

bool isSignalSequenceFinish() {
    return (carSignal == green) && (carCountdown == hide) && (pedestrianSignal == red) && (pedestrianCountdown == hide);
}

bool isCooldownFinish() {
    return isDue(timingSchedule[4], timingDuration[4]);
}

bool isDue(unsigned long time, unsigned long duration) {
    unsigned long now = millis();
    unsigned long diff = now - time;
    // if((now - time) >= duration) DEBUG_INFO("%lu - %lu >= %lu", now, time, duration);
    // else DEBUG_INFO("%lu - %lu < %lu", now, time, duration);
    if (time <= now) return diff >= duration;
    return (diff <= time) && (diff >= duration);
}

void begSignalInterrupt() {
    // DEBUG_INFO("Receive beg signal");
    begSignal = true;
}
