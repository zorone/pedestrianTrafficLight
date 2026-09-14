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

    PORTD = 0b00100111;  // Pedestrian: Display: off     (00), Light: Red     (10)
    PORTB = 0b11000001;  // Car:        Display: off    (000), Light: Green  (001)

    DEBUG_INFO("Ready");
    detectSignal(begSignalPin, 30);
    DEBUG_INFO("%d", digitalRead(begSignalPin));
    DEBUG_INFO("Start!");
}

void loop() {
    PORTD = 0b10100111;  // Pedestrian: Display: RED     (10), Light: Red     (10)
    PORTB = 0b11001001;  // Car:        Display: GREEN  (001), Light: Green  (001)

    delay(20 * 1000 + 50);
    PORTB = 0b11010010;  // Car:        Display: ORANGE (010), Light: Orange (010)


    delay(3 * 1000 + 50);
    PORTB = 0b11100100;  // Car:        Display: RED    (100), Light: Red    (100)
    
    delay(2 * 1000 + 50);
    PORTD = 0b01010111;  // Pedestrian: Display: GREEN   (01), Light: Green   (01)

    delay(15 * 1000 + 50);
    PORTD = 0b00100111;  // Pedestrian: Display: off     (00), Light: Red     (10)

    delay(2 * 1000 + 50);
    PORTB = 0b11000001;  // Car:        Display: off    (000), Light: Green  (001)
    
    timeout = millis() + 60ul * 1000;
    DEBUG_INFO("Ready");
    detectSignal(begSignalPin, 30);
    DEBUG_INFO("%d", digitalRead(begSignalPin));
    while(!isDue(timeout)) {;}
    DEBUG_INFO("Start!");
}
