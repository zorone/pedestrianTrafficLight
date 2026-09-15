#include <limits.h>
#include <string.h>

#include <Arduino.h>
#include <Arduino_DebugUtils.h>
#include "pin.h"
#include "control.h"

unsigned long timeout = 0;
char buf_b[11], buf_d[11];

char* binaryToStr(volatile uint8_t value, char* buffer);

byte reload = 0x4E;
ISR(TIMER2_COMPA_vect) {
    DEBUG_INFO("B: %s, D: %s", binaryToStr(PORTB, buf_b), binaryToStr(PORTD, buf_d));
    OCR2A = reload;
}

void setup() {
    Serial.begin(9600);
    Debug.timestampOn();

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

    cli();
    TCCR0B = 0; 
    OCR2A = reload;
    TCCR2A = 1<<WGM21;
    TCCR2B = (1<<CS22) | (1<<CS21) | (1<<CS20);
    TIMSK2 = (1<<OCIE2A);
    sei();

    changeCarLightSignal(green);
    changePedestrianLightSignal(red);
    changeCarCountdownSignal(green, hide);
    changePedestrianCountdownSignal(red, hide);

    DEBUG_INFO("Ready");
    detectSignal(begSignalPin, 30);
    DEBUG_INFO("%d", digitalRead(begSignalPin));
    DEBUG_INFO("Start!");
}

void loop() {
    changeCarCountdownSignal(green, show);
    changePedestrianCountdownSignal(red, show);

    delay(20 * 1000 + 50);
    changeCarLightSignal(orange);
    changeCarCountdownSignal(orange, show);

    delay(3 * 1000 + 50);
    changeCarLightSignal(red);
    changeCarCountdownSignal(red, show);

    delay(2 * 1000 + 50);
    changePedestrianLightSignal(green);
    changePedestrianCountdownSignal(green, show);

    delay(15 * 1000 + 50);
    changePedestrianLightSignal(red);
    changePedestrianCountdownSignal(red, hide);

    delay(2 * 1000 + 50);
    changeCarLightSignal(green);
    changeCarCountdownSignal(green, hide);

    timeout = millis() + 60ul * 1000;
    DEBUG_INFO("Ready");
    detectSignal(begSignalPin, 30);
    DEBUG_INFO("%d", digitalRead(begSignalPin));
    while(!isDue(timeout)) {;}
    DEBUG_INFO("Start!");
}

char* binaryToStr(volatile uint8_t value, char* buffer) {
    uint8_t tmp = value;
    strncpy(buffer, "0x00000000", 11);
    for(int i = 9; i >= 2; i--) {
        buffer[i] = '0' + (0x00000001 & tmp);
        tmp >>= 1;
    }
    return buffer;
}
