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

void changeCarLightSignal(LightSignal signal);
void changeCarCountdownSignal(LightSignal lightSignal, CountdownDisplay displaySignal);
void changePedestrianLightSignal(LightSignal signal);
void changePedestrianCountdownSignal(LightSignal lightSignal, CountdownDisplay displaySignal);
bool detectSignal(int pin, unsigned int damp);
bool isDue(unsigned long time);

unsigned long timeout = 0;

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
    
    // changeCarLightSignal(green);
    // changePedestrianLightSignal(red);
    // changeCarCountdownSignal(green, hide);
    // changePedestrianCountdownSignal(red, hide);

    // detectSignal(begSignalPin, 30);
}

void loop() {
    digitalWrite(pedestrianCountdownPin_GREEN,  HIGH);
    delay(1000);
    digitalWrite(pedestrianCountdownPin_GREEN,  LOW);
    digitalWrite(pedestrianCountdownPin_RED,    HIGH);
    delay(1000);
    digitalWrite(pedestrianCountdownPin_RED,    LOW);
    digitalWrite(carCountdownPin_GREEN,  HIGH);
    delay(1000);
    digitalWrite(carCountdownPin_GREEN,  LOW);
    digitalWrite(carCountdownPin_ORANGE, HIGH);
    delay(1000);
    digitalWrite(carCountdownPin_ORANGE, LOW);
    digitalWrite(carCountdownPin_GREEN,  HIGH);
    delay(1000);
    digitalWrite(carCountdownPin_GREEN,  LOW);

    // changeCarCountdownSignal(green, show);
    // changePedestrianCountdownSignal(red, show);

    // delay(20 * 1000 + 50);
    // changeCarLightSignal(orange);
    // changeCarCountdownSignal(orange, show);

    // delay(3 * 1000 + 50);
    // changeCarLightSignal(red);
    // changeCarCountdownSignal(red, show);

    // delay(2 * 1000 + 50);
    // changePedestrianLightSignal(green);
    // changePedestrianCountdownSignal(green, show);

    // delay(15 * 1000 + 50);
    // changePedestrianLightSignal(red);
    // changePedestrianCountdownSignal(red, hide);

    // delay(2 * 1000 + 50);
    // changeCarLightSignal(green);
    // changeCarCountdownSignal(green, hide);

    // timeout = millis() + 60ul * 1000;
    // detectSignal(begSignalPin, 30);
    // while(!isDue(timeout)) {;}
}

void unreachable() {
    halt();
}

void halt() {
    for (;;) {}
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

void changeCarCountdownSignal(LightSignal lightSignal, CountdownDisplay displaySignal) {
    for (int pin = 11; pin <= 13; pin++) digitalWrite(pin, LOW);
    switch (displaySignal) {
        case hide:
            switch (lightSignal) {
                case green: digitalWrite(carCountdownPin_GREEN, LOW); break;
                case orange: digitalWrite(carCountdownPin_ORANGE, LOW); break;
                case red: digitalWrite(carCountdownPin_RED, LOW); break;
                default:
                    unreachable();
            }
            break;
        case show:
            switch (lightSignal) {
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

void changePedestrianCountdownSignal(LightSignal lightSignal, CountdownDisplay displaySignal) {
    for (int pin = 6; pin <= 7; pin++) digitalWrite(pin, LOW);
    switch (displaySignal) {
        case hide:
            switch (lightSignal) {
                case green: digitalWrite(pedestrianCountdownPin_GREEN, LOW); break;
                case red: digitalWrite(pedestrianCountdownPin_RED, LOW); break;
                case orange:
                default:
                    unreachable();
            }
            break;
        case show:
            switch (lightSignal) {
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

bool detectSignal(int pin, unsigned int damp) {
    unsigned int initial = 0;
    while(initial < damp) {
        if(digitalRead(pin) == HIGH) initial++;
        else if(initial > 0) initial--;
    }
    return true;
}

bool isDue(unsigned long time) {
    unsigned long now = millis();
    return (now - time) < time;    // Always true, even when now is overflow
}
