#include <Arduino_DebugUtils.h>
  
void unreachable();
void halt();

#define begSignalPin 2

#define pedestrianLightPin_GREEN 4
#define pedestrianLightPin_RED 5
#define pedestrianCountdownPin_GREEN 6
#define pedestrianCountdownPin_RED 7

#define carLightPin_GREEN 8
#define carLightPin_ORANGE 9
#define carLightPin_RED 10
#define carCountdownPin_GREEN 11
#define carCountdownPin_ORANGE 12
#define carCountdownPin_RED 13

typedef enum {
    ready,
    running,
    cooldown
} DeviceState;

typedef enum {
    green,
    orange,
    red
} LightSignal;

typedef enum {
    hide,
    show
} CountdownDisplay;

void scheduleLightSignal();
void changeCarSignalState();
void changeCarCountdownState();
void changePedestrianSignalState();
void changePedestrianCountdownState();
bool isSignalSequenceFinish();
bool isCooldownFinish();

void changeCarLightSignal(LightSignal signal);
void changeCarCountdownSignal(CountdownDisplay signal);
void changePedestrianLightSignal(LightSignal signal);
void changePedestrianCountdownSignal(CountdownDisplay signal);

bool isDue(unsigned long time);
void begSignalInterrupt();

DeviceState deviceStatus = ready;
LightSignal carSignal = green;
LightSignal pedestrianSignal = red;
CountdownDisplay carCountdown = hide;
CountdownDisplay pedestrianCountdown = hide;
volatile bool begSignal = false, registeredBegSignal = false;
unsigned long timingSchedule[] = {
    0,    // 0: Time when car light signal is switched
    0,    // 1: Time when car countdown signal is switched
    0,    // 2: Time when pedestrian light signal is switched
    0,    // 3: Time when pedestrian countdown signal is switched
    0,    // 4: Time when the cycle will end
    0     // 5: Time when beg signal will available again
};

volatile unsigned long debuggingSchedule = 0;
bool debug = true;
bool enableTextOutput = debug;

void printDeviceStatus();
void printCarStatus();
void printCarStatusTransition(LightSignal lastCarSignal);
void printCarCountdownTransition(CountdownDisplay lastCarCountdown);
void printPedestrianStatus();
void printPedestrianStatusTransition(LightSignal lastPedestrianSignal);
void printPedestrianCountdownTransition(CountdownDisplay lastPedestrianCountdown);

void setup() {
    Serial.begin(9600);
    debuggingSchedule = millis() + 250;

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
    attachInterrupt(digitalPinToInterrupt(2), begSignalInterrupt, CHANGE);

    changeCarLightSignal(carSignal);
    changeCarCountdownSignal(carCountdown);
    changePedestrianLightSignal(pedestrianSignal);
    changePedestrianCountdownSignal(pedestrianCountdown);

    printDeviceStatus();
    printCarStatus();
    printPedestrianStatus();

    Debug.timestampOn();
    enableTextOutput = false;
}

void loop() {
    if (isDue(debuggingSchedule)) {
        enableTextOutput = true;
        debuggingSchedule += 250;
    }
    if (begSignal) {
        registeredBegSignal = begSignal;
        begSignal = false;
    }
    // printDeviceStatus();
    switch (deviceStatus) {
        case ready:
            printCarStatus();
            printPedestrianStatus();
            if (registeredBegSignal == true) {
                scheduleLightSignal();
                registeredBegSignal = false;
                deviceStatus = running;
                enableTextOutput = true;
            }
            break;
        case running:
            if (isSignalSequenceFinish()) {
                deviceStatus = cooldown;
                carSignal = green;
                carCountdown = hide;
                pedestrianSignal = red;
                pedestrianCountdown = hide;
                changeCarCountdownSignal(carCountdown);
                changeCarLightSignal(carSignal);
                changePedestrianCountdownSignal(pedestrianCountdown);
                changePedestrianLightSignal(pedestrianSignal);
                Serial.println("Finish the sequence");
                break;
            }
            changeCarCountdownState();
            changeCarSignalState();
            changeCarCountdownSignal(carCountdown);
            changeCarLightSignal(carSignal);
            changePedestrianCountdownState();
            changePedestrianSignalState();
            changePedestrianCountdownSignal(pedestrianCountdown);
            changePedestrianLightSignal(pedestrianSignal);
            break;
        case cooldown:
            printCarStatus();
            printPedestrianStatus();
            if (isCooldownFinish()) deviceStatus = ready;
            break;
        default:
            Serial.print("loop(): unreachable state: ");
            Serial.println(deviceStatus);
            unreachable();
    }
    enableTextOutput = false;
}

void unreachable() {
    // Serial.begin(9600);
    Serial.println("Error: Hardware have reached unreachable state!!!");
    halt();
}

void halt() {
    for (;;) {}
}

void scheduleLightSignal() {
    DEBUG_INFO("Beg signal is registered");
    DEBUG_INFO("Schedule Light Signal");
    unsigned long now = millis();
    timingSchedule[0] = now +  20 * 1000 + 50;            // Set due time for light signal to become orange, padding for briefly show '0'
    timingSchedule[1] = now;                              // Set due time to start car countdown
    timingSchedule[2] = now +  25 * 1000 + 2 * 50;        // Set due time for pedestrian light to become green, need to pad signal down as it need to account for orange light of car traffic
    timingSchedule[3] = now +   0 * 1000 + 50;            // Set due time to start pedestrian countdown, need to pad down due to orange signal from car traffic
    timingSchedule[4] = now +  42ul * 1000ul + 3 * 50;    // Set when the cycle will end
    timingSchedule[5] = now + 100ul * 1000ul + 3 * 50;    // Set due time for next beg signal, padding for brief '0' of each light signal
    DEBUG_INFO("timingSchedule[0] = %lu", timingSchedule[0]);
    DEBUG_INFO("timingSchedule[1] = %lu", timingSchedule[1]);
    DEBUG_INFO("timingSchedule[2] = %lu", timingSchedule[2]);
    DEBUG_INFO("timingSchedule[3] = %lu", timingSchedule[3]);
    DEBUG_INFO("timingSchedule[4] = %lu", timingSchedule[4]);
    DEBUG_INFO("timingSchedule[5] = %lu", timingSchedule[5]);
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
                timingSchedule[0] =  timingSchedule[4];  // Set to finishing time, to circumvent from rerunning this function before reaching the end of cycle
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
                        timingSchedule[1] = timingSchedule[4];    // Assume that changeCarCountdownState happens after changeCarSignalState()
                        break;
                    case red:
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
                        case red:
                            // timingSchedule[1] = timingSchedule[4];    // Return to idle state, set to end of cycle time
                            carCountdown = hide;
                            break;
                        case green:
                        case orange:
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
                timingSchedule[2] = timingSchedule[4];
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
                        timingSchedule[3] = timingSchedule[4];    // Assume that changePedestrianCountdownState happens after changePedestrianSignalState()
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
                        // timingSchedule[3] = timingSchedule[4]; // Return to idle state, set to end cycle time
                        pedestrianCountdown = hide;
                        break;
                    case red:
                        timingSchedule[3] = timingSchedule[4];
                        break;
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
    return isDue(timingSchedule[4]);
}

bool isCooldownFinish() {
    return isDue(timingSchedule[5]);
}

bool isDue(unsigned long time) {
    unsigned long now = millis();
    return (now - time) < time;    // Always true, even when now is overflow
}

void begSignalInterrupt() {
    Serial.println("Interrupted!");
    begSignal = digitalRead(begSignalPin);
}

void printDeviceStatus() {
    if (!enableTextOutput) return;
    DEBUG_INFO("deviceStatus = %d", deviceStatus);
}

void printCarStatus() {
    if (!enableTextOutput) return;
    DEBUG_INFO("carSignal = %d, carCountdown = %d", carSignal, carCountdown);
}

// TODO: How would I track its real function name?
void printCarStatusTransition(LightSignal lastCarSignal) {
    if (!debug) return;
    if (carSignal == lastCarSignal) return;
    DEBUG_INFO("changeCarSignalState: transition carSignal from %d to %d", lastCarSignal, carSignal);
    DEBUG_INFO("changeCarSignalState: Next Schedule: %lu", timingSchedule[0]);
}

void printCarCountdownTransition(CountdownDisplay lastCarCountdown) {
    if (!debug) return;
    if (carCountdown == lastCarCountdown) return;
    DEBUG_INFO("changeCarCountdownState: transition carCountdown from %d to %d", lastCarCountdown, carCountdown);
    DEBUG_INFO("changeCarCountdownState: Next Schedule: %lu", timingSchedule[1]);
}

void printPedestrianStatusTransition(LightSignal lastPedestrianSignal) {
    if (!debug) return;
    if (pedestrianSignal == lastPedestrianSignal) return;
    DEBUG_INFO("changePedestrianSignalState: transition PedestrianSignal from %d to %d", lastPedestrianSignal, pedestrianSignal);
    DEBUG_INFO("changePedestrianSignalState: Next Schedule: %lu", timingSchedule[2]);
}

void printPedestrianCountdownTransition(CountdownDisplay lastPedestrianCountdown) {
    if (!debug) return;
    if (pedestrianCountdown == lastPedestrianCountdown) return;
    DEBUG_INFO("changePedestrianCountdownState: transition PedestrianCountdown from %d to %d", lastPedestrianCountdown, pedestrianCountdown);
    DEBUG_INFO("changePedestrianCountdownState: Next Schedule: %lu", timingSchedule[3]);
}

void printPedestrianStatus() {
    if (!enableTextOutput) return;
    DEBUG_INFO("pedestrianSignal = %d, pedestrianCountdown = %d", pedestrianSignal, pedestrianCountdown);
}
