#include "Arduino.h"
#include "common.h"
#include "pin.h"
#include "control.h"

LightSignal carSignal = green;
LightSignal pedestrianSignal = red;
CountdownDisplay carCountdown = hide;
CountdownDisplay pedestrianCountdown = hide;

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

bool isDue(unsigned long time) {
    unsigned long now = millis();
    return (now - time) < time;    // Always true, even when now is overflow
}
