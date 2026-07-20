#include "common.h"

void unreachable() {
    // Serial.begin(9600);
    DEBUG_ERROR("Error: Hardware have reached unreachable state!!!");
    halt();
}

void halt() {
    for (;;) {}
}