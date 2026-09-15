#include <Arduino.h>
#include <Arduino_DebugUtils.h>

#include "common.h"
#include "pin.h"
#include "control.h"

LightSignal carSignal = green;
LightSignal pedestrianSignal = red;
CountdownDisplay carCountdown = hide;
CountdownDisplay pedestrianCountdown = hide;

const char* operation[] = {"Clear", "Set"};
char* buf_b[11], buf_d[0];

void changeCarLightSignal(LightSignal signal) {
    const char* funcName = "changeCarLightSignal";
    for (int pin = 8; pin <= 10; pin++) {
        digitalWrite(pin, LOW);
    }
    DEBUG_INFO("%31s: %5s: B: %#b, D: %#b", funcName, operation[0], binaryToStr(&PORTB, buf_b), binaryToStr(&PORTD, buf_d));
    
    switch (signal) {
        case green: digitalWrite(carLightPin_GREEN, HIGH); break;
        case orange: digitalWrite(carLightPin_ORANGE, HIGH); break;
        case red: digitalWrite(carLightPin_RED, HIGH); break;
        default:
            Serial.print("changeCarLightSignal(): unreachable state: ");
            Serial.println(signal);
            unreachable();
    }
    DEBUG_INFO("%31s: %5s: B: %#b, D: %#b", funcName, operation[1], PORTB, PORTD);
}

void changeCarCountdownSignal(LightSignal lightSignal, CountdownDisplay displaySignal) {
    const char* funcName = "changeCarCountdownSignal";
    for (int pin = 11; pin <= 13; pin++) digitalWrite(pin, LOW);
    DEBUG_INFO("%31s: %5s: B: %#b, D: %#b", funcName, operation[0], PORTB, PORTD);

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

    DEBUG_INFO("%31s: %5s: B: %#b, D: %#b", funcName, operation[1], PORTB, PORTD);
}

void changePedestrianLightSignal(LightSignal signal) {
    const char* funcName = "changePedestrianLightSignal";
    for (int pin = 4; pin <= 5; pin++) digitalWrite(pin, LOW);
    DEBUG_INFO("%31s: %5s: B: %#b, D: %#b", funcName, operation[0], PORTB, PORTD);
    switch (signal) {
        case green: digitalWrite(pedestrianLightPin_GREEN, HIGH); break;
        case red: digitalWrite(pedestrianLightPin_RED, HIGH); break;
        case orange:
        default:
            Serial.print("changePedestrianLightSignal: unreachable state: ");
            Serial.println(signal);
            unreachable();
    }
    DEBUG_INFO("%31s: %5s: B: %#b, D: %#b", funcName, operation[1], PORTB, PORTD);
}

void changePedestrianCountdownSignal(LightSignal lightSignal, CountdownDisplay displaySignal) {
    const char* funcName = "changePedestrianCountdownSignal";
    for (int pin = 6; pin <= 7; pin++) digitalWrite(pin, LOW);
    DEBUG_INFO("%31s: %5s: B: %#b, D: %#b", funcName, operation[0], PORTB, PORTD);
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
    DEBUG_INFO("%31s: %5s: B: %#b, D: %#b", funcName, operation[1], PORTB, PORTD);
}

bool detectSignal(int pin, unsigned int damp) {
    unsigned int initial = 0;
    while(initial < damp) {
        DEBUG_INFO("%u", initial);
        if(digitalRead(pin) == HIGH) initial++;
        else if(initial > 0) initial--;
    }
    return true;
}

bool isDue(unsigned long time) {
    unsigned long now = millis();
    return (now - time) < time;    // Always true, even when now is overflow
}

char* binaryToStr(uint8_t value, char* buffer) {
    strncpy(buffer, "0x00000000", 11);
    for(int i = 9; i >= 2; i--) {
        buffer[i] = '0' + (0x1 & value);
        value >>= 1;
    }

    return buffer;
}