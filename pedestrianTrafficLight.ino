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

void setup() {
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
}

void loop() {
    if (begSignal) {
        registeredBegSignal = begSignal;
        begSignal = false;
    }
    switch (deviceStatus) {
        case ready:
            if (registeredBegSignal == true) {
                scheduleLightSignal();
                registeredBegSignal = false;
                deviceStatus = running;
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
            if (isCooldownFinish()) deviceStatus = ready;
            break;
        default:
            unreachable();
    }
}

void unreachable() {
    halt();
}

void halt() {
    for (;;) {}
}

void scheduleLightSignal() {
    unsigned long now = millis();
    timingSchedule[0] = now +  20 * 1000 + 50;            // Set due time for light signal to become orange, padding for briefly show '0'
    timingSchedule[1] = now;                              // Set due time to start car countdown
    timingSchedule[2] = now +  25 * 1000 + 2 * 50;        // Set due time for pedestrian light to become green, need to pad signal down as it need to account for orange light of car traffic
    timingSchedule[3] = now +   0 * 1000 + 50;            // Set due time to start pedestrian countdown, need to pad down due to orange signal from car traffic
    timingSchedule[4] = now +  42ul * 1000ul + 3 * 50;    // Set when the cycle will end
    timingSchedule[5] = now + 100ul * 1000ul + 3 * 50;    // Set due time for next beg signal, padding for brief '0' of each light signal
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
                unreachable();
        }
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
                            unreachable();
                    }
                }
                break;
            default:
                unreachable();
        }
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
                unreachable();
        }
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
                        unreachable();
                }
                break;
            default:
                unreachable();
        }
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
                    unreachable();
            }
            break;
        case show:
            switch (carSignal) {
                case green: digitalWrite(carCountdownPin_GREEN, HIGH); break;
                case orange: digitalWrite(carCountdownPin_ORANGE, HIGH); break;
                case red: digitalWrite(carCountdownPin_RED, HIGH); break;
                default:
                    unreachable();
            }
            break;
        default:
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
                    unreachable();
            }
            break;
        case show:
            switch (pedestrianSignal) {
                case green: digitalWrite(pedestrianCountdownPin_GREEN, HIGH); break;
                case red: digitalWrite(pedestrianCountdownPin_RED, HIGH); break;
                case orange:
                default:
                    unreachable();
            }
            break;
        default:
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
    begSignal = digitalRead(begSignalPin);
}
