#include <limits.h>

#include <Arduino.h>
#include <Arduino_DebugUtils.h>
#include "pin.h"
#include "control.h"

// PORTD 0b0000x0xx
//       7: ^ Pedestrian: Countdown: Red
//        6: ^ Pedestrian: Countdown: Green
//         5: ^ Pedestrian: Light: Red
//          4: ^ Pedestrian: Light: Green
//           3: ^ UNUSED
//            2: ^ Trigger

// PORTB 0bxx000000
//       13: ^ Car: Countdown: Red
//        12: ^ Car: Countdown: Orange
//         11: ^ Car: Countdown: Green
//          10: ^ Car: Light: Red
//            9: ^ Car: Light: Orange
//             8: ^ Car: Light: Green

// AND operation
// a  b  a&b
// 0  0   0
// 0  1   0
// 1  0   0
// 1  1   1
// x  0   0
// x  1   x
// x  x   x

// OR operation
// a  b  a|b
// 0  0   0
// 0  1   1
// 1  0   1
// 1  1   1
// x  0   x
// x  1   1
// x  x   x

// XOR operation
// a  b  a|b
// 0  0   0
// 0  1   1
// 1  0   1
// 1  1   0
// x  0   x
// x  1  ~x
// x  x   0

// NOT operation
// a  ~a
// 0   1
// 1   0
// x  ~x


unsigned long timeout = 0;

char buf_b[11], buf_d[11];
volatile uint8_t PORTB_PRE, PORTB_POST, PORTD_PRE, PORTD_POST;

char* binaryToStr(volatile uint8_t value, char* buffer);

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
    DEBUG_INFO("%15s: B: %s, D: %s", "start", binaryToStr(PORTB, buf_b), binaryToStr(PORTD, buf_d));

    PORTD &= 0b00100111;  // Pedestrian: Display: off     (00), Light: Red     (10)
    DEBUG_INFO("%15s: B: %s, D: %s", "PED: RED: off:", binaryToStr(PORTB, buf_b), binaryToStr(PORTD, buf_d));
    PORTB &= 0b11000001;  // Car:        Display: off    (000), Light: Green  (001)
    DEBUG_INFO("%15s: B: %s, D: %s", "CAR: GRN: off:", binaryToStr(PORTB, buf_b), binaryToStr(PORTD, buf_d));

    DEBUG_INFO("Ready");
    detectSignal(begSignalPin, 30);
    DEBUG_INFO("%d", digitalRead(begSignalPin));
    DEBUG_INFO("Start!");
}

void loop() {
    PORTD &= 0b10100111;  // Pedestrian: Display: RED     (10), Light: Red     (10)
    DEBUG_INFO("%15s: B: %s, D: %s", "PED: RED: RED:", binaryToStr(PORTB, buf_b), binaryToStr(PORTD, buf_d));
    PORTB &= 0b11001001;  // Car:        Display: GREEN  (001), Light: Green  (001)
    DEBUG_INFO("%15s: B: %s, D: %s", "CAR: GRN: GRN:", binaryToStr(PORTB, buf_b), binaryToStr(PORTD, buf_d));

    delay(20 * 1000 + 50);
    PORTB &= 0b11010010;  // Car:        Display: ORANGE (010), Light: Orange (010)
    DEBUG_INFO("%15s: B: %s, D: %s", "CAR: ORN: ORN:", binaryToStr(PORTB, buf_b), binaryToStr(PORTD, buf_d));

    delay(3 * 1000 + 50);
    PORTB &= 0b11100100;  // Car:        Display: RED    (100), Light: Red    (100)
    DEBUG_INFO("%15s: B: %s, D: %s", "CAR: RED: RED:", binaryToStr(PORTB, buf_b), binaryToStr(PORTD, buf_d));
    
    delay(2 * 1000 + 50);
    PORTD &= 0b01010111;  // Pedestrian: Display: GREEN   (01), Light: Green   (01)
    DEBUG_INFO("%15s: B: %s, D: %s", "PED: GRN: GRN:", binaryToStr(PORTB, buf_b), binaryToStr(PORTD, buf_d));

    delay(15 * 1000 + 50);
    PORTD &= 0b00100111;  // Pedestrian: Display: off     (00), Light: Red     (10)
    DEBUG_INFO("%15s: B: %s, D: %s", "PED: RED: off:", binaryToStr(PORTB, buf_b), binaryToStr(PORTD, buf_d));

    delay(2 * 1000 + 50);
    PORTB &= 0b11000001;  // Car:        Display: off    (000), Light: Green  (001)
    DEBUG_INFO("%15s: B: %s, D: %s", "CAR: GRN: off:", binaryToStr(PORTB, buf_b), binaryToStr(PORTD, buf_d));
    
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