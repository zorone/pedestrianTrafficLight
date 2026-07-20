#include "common.h"

void unreachable() {
    // Serial.begin(9600);
    Serial.println("Error: Hardware have reached unreachable state!!!");
    halt();
}

void halt() {
    for (;;) {}
}