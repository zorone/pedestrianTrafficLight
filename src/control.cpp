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

void changeCarCountdownSignal(LightSignal lightSignal, CountdownDisplay displaySignal) {
    for (int pin = 11; pin <= 13; pin++) digitalWrite(pin, LOW);
    switch (displaySignal) {
        case hide:
            switch (lightSignal) {
                case green: digitalWrite(carCountdownPin_GREEN, LOW); break;
                case orange: digitalWrite(carCountdownPin_ORANGE, LOW); break;
                case red: digitalWrite(carCountdownPin_RED, LOW); break;
                default:
                    Serial.print("changeCarCountdownSignal: unreachable state: ");
                    Serial.print(lightSignal);
                    Serial.print(" ");
                    Serial.print(displaySignal);
                    unreachable();
            }
            break;
        case show:
            switch (lightSignal) {
                case green: digitalWrite(carCountdownPin_GREEN, HIGH); break;
                case orange: digitalWrite(carCountdownPin_ORANGE, HIGH); break;
                case red: digitalWrite(carCountdownPin_RED, HIGH); break;
                default:
                    Serial.print("changeCarCountdownSignal(): unreachable state: ");
                    Serial.print(lightSignal);
                    Serial.print(" ");
                    Serial.println(displaySignal);
                    unreachable();
            }
            break;
        default:
            Serial.print("changeCarCountdownSignal(): unreachable state: ");
            Serial.println(displaySignal);
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

void changePedestrianCountdownSignal(LightSignal lightSignal, CountdownDisplay displaySignal) {
    for (int pin = 6; pin <= 7; pin++) digitalWrite(pin, LOW);
    switch (displaySignal) {
        case hide:
            switch (lightSignal) {
                case green: digitalWrite(pedestrianCountdownPin_GREEN, LOW); break;
                case red: digitalWrite(pedestrianCountdownPin_RED, LOW); break;
                case orange:
                default:
                    Serial.print("changePedestrianCountdownSignal: unreachable state: ");
                    Serial.print(lightSignal);
                    Serial.print(" ");
                    Serial.println(displaySignal);
                    unreachable();
            }
            break;
        case show:
            switch (lightSignal) {
                case green: digitalWrite(pedestrianCountdownPin_GREEN, HIGH); break;
                case red: digitalWrite(pedestrianCountdownPin_RED, HIGH); break;
                case orange:
                default:
                    Serial.print("changePedestrianCountdownSignal: unreachable state: ");
                    Serial.print(lightSignal);
                    Serial.print(" ");
                    Serial.println(displaySignal);
                    unreachable();
            }
            break;
        default:
            Serial.print("changePedestrianCountdownSignal: unreachable state: ");
            Serial.println(displaySignal);
            unreachable();
    }
}

bool detectSignal(int pin, unsigned int damp) {
    unsigned int initial = 0;
    while(initial < damp) {
        DEBUG_INFO("%u", initial);
        if(digitalRead(pin) != HIGH) initial++;
        else if(initial > 0) initial--;
    }
    return true;
}

bool isDue(unsigned long time) {
    unsigned long now = millis();
    return (now - time) < time;    // Always true, even when now is overflow
}
