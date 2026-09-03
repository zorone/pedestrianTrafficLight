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
    digitalWrite(carCountdownPin_RED,    HIGH);
    delay(1000);
    digitalWrite(carCountdownPin_RED,    LOW);
}
