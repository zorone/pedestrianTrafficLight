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
bool debug = true;
bool enableTextOutput = debug;

void scheduleLightSignal() {
    unsigned long now = millis();
    timingSchedule[0] = now + 20 * 1000 + 50;            // Set due time for light signal to become orange, padding for briefly show '0'
    timingSchedule[1] = now;                             // Set due time to start car countdown
    timingSchedule[2] = now + 25 * 1000 + 2 * 50;        // Set due time for pedestrian light to become green, need to pad signal down as it need to account for orange light of car traffic
    timingSchedule[3] = now +  0 * 1000 + 50;            // Set due time to start pedestrian countdown, need to pad down due to orange signal from car traffic
    timingSchedule[4] = now + 40ul * 1000ul + 3 * 50;    // Set due time for next beg signal, padding for brief '0' of each light signal
}

void changeCarSignalState() {
    if (isDue(timingSchedule[0])) { 
        LightSignal lastCarSignal = carSignal;
        switch (carSignal) {
            case green:
                timingSchedule[0] +=  3 * 1000 + 50;
                carSignal = orange;
                break;
            case orange:
                timingSchedule[0] += 19 * 1000 + 50;
                carSignal = red;
                break;
            case red:
                timingSchedule[0] +=  timingSchedule[4];  // Set to finishing time, to circumvent from rerunning this function before reaching the end of cycle
                carSignal = green;
                break;
            default:
                Serial.print("changeCarSignalState: unreachable state: ");
                Serial.println(carSignal);
                unreachable();
        }
        printCarStatusTransition(lastCarSignal);
    }
}

void changeCarCountdownState() {
    if (isDue(timingSchedule[1])) {
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
                        Serial.print("changeCarCountdownState: unreachable state: ");
                        Serial.print(carSignal);
                        Serial.print(" ");
                        Serial.println(carCountdown);
                        unreachable();
                }
                break;
            case show:
                {
                    switch (carSignal) {
                        case orange:
                            timingSchedule[1] = timingSchedule[0];    // Assume that carCountdown signal doesn't get changed anywhere else
                            break;
                        case red:
                            timingSchedule[1] = timingSchedule[4];    // Return to idle state, set to end of cycle time
                            carCountdown = hide;
                            break;
                        case green:
                        default:
                            Serial.print("changeCarCountdownState: unreachable state: ");
                            Serial.print(carSignal);
                            Serial.print(" ");
                            Serial.println(carCountdown);
                            unreachable();
                    }
                }
                break;
            default:
                Serial.print("changeCarCountdownState: unreachable state: ");
                Serial.println(carCountdown);
                unreachable();
        }
        printCarStatusTransition(lastCarSignal);
        printCarCountdownTransition(lastCarCountdown);
    }
}

void changePedestrianSignalState() {
    if (isDue(timingSchedule[2])) {
        LightSignal lastPedestrianSignal = pedestrianSignal;
        switch (pedestrianSignal) {
            case red:
                timingSchedule[2] += 15 * 1000 + 50;
                pedestrianSignal = green;
                break;
            case green:
                timingSchedule[2] += timingSchedule[4];
                pedestrianSignal = red;
                break;
            case orange:
            default:
                Serial.print("changePedestrianSignalState: unreachable state: ");
                Serial.println(pedestrianSignal);
                unreachable();
        }
        printPedestrianStatusTransition(lastPedestrianSignal);
    }
}

void changePedestrianCountdownState() {
    if (isDue(timingSchedule[3])) {
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
                        Serial.print("changePedestrianCountdownState: unreachable state: ");
                        Serial.print(pedestrianSignal);
                        Serial.print(" ");
                        Serial.println(pedestrianCountdown);
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
                        Serial.print("changePedestrianCountdownState(): unreachable state: ");
                        Serial.print(pedestrianSignal);
                        Serial.print(" ");
                        Serial.println(pedestrianCountdown);
                        unreachable();
                }
                break;
            default:
                Serial.print("changePedestrianCountdownState(): unreachable state: ");
                Serial.println(pedestrianCountdown);
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
            Serial.print("changeCarLightSignal(): unreachable state: ");
            Serial.println(signal);
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
                    Serial.print("changeCarCountdownSignal: unreachable state: ");
                    Serial.print(carSignal);
                    Serial.print(" ");
                    Serial.print(signal);
                    unreachable();
            }
            break;
        case show:
            switch (carSignal) {
                case green: digitalWrite(carCountdownPin_GREEN, HIGH); break;
                case orange: digitalWrite(carCountdownPin_ORANGE, HIGH); break;
                case red: digitalWrite(carCountdownPin_RED, HIGH); break;
                default:
                    Serial.print("changeCarCountdownSignal(): unreachable state: ");
                    Serial.print(carSignal);
                    Serial.print(" ");
                    Serial.println(signal);
                    unreachable();
            }
            break;
        default:
            Serial.print("changeCarCountdownSignal(): unreachable state: ");
            Serial.println(signal);
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
            Serial.print("changePedestrianLightSignal: unreachable state: ");
            Serial.println(signal);
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
                    Serial.print("changePedestrianCountdownSignal: unreachable state: ");
                    Serial.print(pedestrianSignal);
                    Serial.print(" ");
                    Serial.println(signal);
                    unreachable();
            }
            break;
        case show:
            switch (pedestrianSignal) {
                case green: digitalWrite(pedestrianCountdownPin_GREEN, HIGH); break;
                case red: digitalWrite(pedestrianCountdownPin_RED, HIGH); break;
                case orange:
                default:
                    Serial.print("changePedestrianCountdownSignal: unreachable state: ");
                    Serial.print(pedestrianSignal);
                    Serial.print(" ");
                    Serial.println(signal);
                    unreachable();
            }
            break;
        default:
            Serial.print("changePedestrianCountdownSignal: unreachable state: ");
            Serial.println(signal);
            unreachable();
    }
}

bool isSignalSequenceFinish() {
    return (carSignal == green) && (carCountdown == hide) && (pedestrianSignal == red) && (pedestrianCountdown == hide);
}

bool isCooldownFinish() {
    return isDue(timingSchedule[4]);
}

bool isDue(unsigned long time) {
    unsigned long now = millis();
    return (now - time) < time;    // Always true, even when now is overflow
}

void begSignalInterrupt() {
    begSignal = true;
}
