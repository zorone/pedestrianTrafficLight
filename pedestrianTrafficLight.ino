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

DeviceState deviceStatus = ready;
LightSignal carSignal        = green;
LightSignal pedestrianSignal = red;
CountdownDisplay carCountdown        = hide;
CountdownDisplay pedestrianCountdown = hide;
volatile bool begSignal = false;
unsigned long timingSchedule[] = {
  0, // 0: Time when car light signal is switched
  0, // 1: Time when car countdown signal is switched
  0, // 2: Time when pedestrian light signal is switched
  0, // 3: Time when pedestrian countdown signal is switched
  0  // 4: Time when beg signal will available again
};

void scheduleLightSignal();
void changeCarSignalState();
void changeCarCountdownState();
void changePedestrianSignalState();
void changePedestrianCountdownState();
bool isCooldownFinish();

void changeCarLightSignal(LightSignal signal);
void changeCarCountdownSignal(CountdownDisplay signal);
void changePedestrianLightSignal(LightSignal signal);
void changePedestrianCountdownSignal(CountdownDisplay signal);

bool isDue(unsigned long time);
void begSignalInterrupt();
void unreachable();

void halt();

#define begSignalPin 2
#define pedestrianLightPin_GREEN 3
#define pedestrianLightPin_RED 4
#define pedestrianCountdownPin_GREEN 5
#define pedestrianCountdownPin_RED 6

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
  attachInterrupt(digitalPinToInterrupt(), begSignalInterrupt, RISING);

  changeCarLightSignal(carSignal);
  changeCarCountdownSignal(carCountdown);
  changePedestrianLightSignal(pedestrianSignal);
  changePedestrianCountdownSignal(pedestrianCountdown);
}

void loop() {
  switch(deviceStatus) {
    case ready:
      if(begSignal == true) {
        scheduleLightSignal();
        begSignal = false;
        deviceStatus = running;
      }
      break;
    case running:
      changeCarSignalState();
      changeCarLightSignal(carSignal);
      changePedestrianSignalState();
      changeCarCountdownSignal(carCountdown);
      changeCarCountdownState();
      changePedestrianLightSignal(pedestrianSignal);
      changePedestrianCountdownState();
      changePedestrianCountdownSignal(pedestrianCountdown);
      break;
    case cooldown:
      if(isCooldownFinish()) deviceStatus = ready;
      break;
    default: unreachable();
  }
}

void scheduleLightSignal() {
  unsigned long now = millis();
  timingSchedule[0] = now + 20*1000 +   50; // Set due time for light signal to become orange, padding for briefly show '0'
  timingSchedule[1] = now;                  // Set due time to start car countdown
  timingSchedule[2] = now + 28*1000 + 2*50; // Set due time for pedestrian light to become green, need to pad signal down as it need to account for orange light of car traffic
  timingSchedule[3] = now +  3*1000 +   50; // Set due time to start pedestrian countdown, need to pad down due to orange signal from car traffic
  timingSchedule[4] = now + 43*1000 + 3*50; // Set due time for next beg signal, padding for brief '0' of each light signal
}

void changeCarSignalState() {
  if(isDue(timingSchedule[0])) {
    switch(carSignal) {
      case green: 
        timingSchedule[0] +=  3*1000 + 50;
        carSignal = orange;
        break;
      case orange: 
        timingSchedule[0] += 25*1000 + 50;
        carSignal = red;
        break;
      case red: 
        // timingSchedule[0] +=  0*1000 + 50;
        carSignal = green;
        break;
      default: unreachable();
    }
  }
}

void changeCarCountdownState() {
  if(isDue(timingSchedule[1])) {
    switch(carCountdown) {
      case hide: 
        switch(carSignal) {
          case green: 
            carCountdown = show;
            timingSchedule[1] = timingSchedule[0];  // Assume that changeCarCountdownState() happens after changeCarSignalState()
            break;
          case orange: unreachable(); // Assume that changeCarSignalState() happens beforehand, and doesn't get changed anywhere else.
          case red: unreachable();
          default: unreachable();
        }
        break;
      case show: {
        switch(carSignal) {
          case green: unreachable();
          case orange:
            timingSchedule[1] = timingSchedule[0];    // Assume that carCountdown signal doesn't get changed anywhere else
            break;
          case red:
            // timingSchedule[1] = timingSchedule[0]; // Return to idle state, no need to set
            carCountdown = hide;
            break;
          default: unreachable();
        }
      } 
        break;
      default: unreachable();
    }
  }
}

void changePedestrianSignalState() {
  if(isDue(timingSchedule[2])) {
    switch(pedestrianSignal) {
      case red:
        timingSchedule[2] += 15*1000 + 50;
        pedestrianSignal = green;
        break;
      case green:
        // timingSchedule[2] += 0*1000 + 50;
        pedestrianSignal = red;
        break;
      case orange: default: unreachable();
    }
  }
}

void changePedestrianCountdownState() {
  if(isDue(timingSchedule[3])) {
    switch(pedestrianCountdown) {
      case hide:
        switch(pedestrianSignal) {
          case red:
            timingSchedule[3] = timingSchedule[2]; // Assume that changePedestrianCountdownState() happens after changePedestrianSignalState()
            pedestrianCountdown = show;
            break;
          case green: unreachable(); // Assume that changePedestrianSignalState() happens beforehand, and doesn't get changed anywhere else.
          case orange: default: unreachable();
        }
        break;
      case show:
        switch(pedestrianSignal) {
          case red: unreachable();
          case green:
            // imingSchedule[3] = timingSchedule[2]; // Return to idle state, no need to set
            pedestrianCountdown = hide;
            break;
          case orange: default: unreachable();
        }
        break;
      default: unreachable();
    }
  }
}

void changeCarLightSignal(LightSignal signal) {
  for(int pin = 8; pin <= 10; pin++) {
    digitalWrite(pin, LOW);
  }
  switch(signal) {
    case green: digitalWrite(carLightPin_GREEN, HIGH);
    case orange: digitalWrite(carLightPin_ORANGE, HIGH);
    case red: digitalWrite(carLightPin_RED, HIGH);
    default: unreachable();
  }
}

void changeCarCountdownSignal(CountdownDisplay signal) {
  for(int pin = 11; pin <= 13; pin++) digitalWrite(pin, LOW);
  switch(signal) {
    case hide:
      switch(carSignal) {
        case green: digitalWrite(carCountdownPin_GREEN, LOW); break;
        case orange: digitalWrite(carCountdownPin_ORANGE, LOW); break;
        case red: digitalWrite(carCountdownPin_RED, LOW); break;
        default: unreachable();
      }
    case show:
      switch(carSignal) {
        case green: digitalWrite(carCountdownPin_GREEN, HIGH); break;
        case orange: digitalWrite(carCountdownPin_ORANGE, HIGH); break;
        case red: digitalWrite(carCountdownPin_RED, HIGH); break;
        default: unreachable();
      }
  }
}

void changePedestrianLightSignal(LightSignal signal) {
  for(int pin = 3; pin <= 4; pin++) digitalWrite(pin, LOW);
  switch(signal) {
    case green: digitalWrite(pedestrianLightPin_GREEN, HIGH);
    case red: digitalWrite(pedestrianLightPin_RED, HIGH);
    case orange: default: unreachable();
  }
}

void changePedestrianCountdownSignal(CountdownDisplay signal) {
  for(int pin = 5; pin <= 6; pin++) digitalWrite(pin, LOW);
  switch(signal) {
    case hide:
      switch(pedestrianSignal) {
        case green: digitalWrite(pedestrianCountdownPin_GREEN, LOW); break;
        case red: digitalWrite(pedestrianCountdownPin_RED, LOW); break;
        case orange: default: unreachable();
      }
    case show:
      switch(pedestrianSignal) {
        case green: digitalWrite(pedestrianCountdownPin_GREEN, HIGH); break;
        case red: digitalWrite(pedestrianCountdownPin_RED, HIGH); break;
        case orange: default: unreachable();
      }
  }
}

bool isCooldownFinish() {
  return isDue(timingSchedule[4]);
}

bool isDue(unsigned long time) {
  unsigned long now  = millis();
  return now - time;                      // Always true, even when now is overflow
}

void begSignalInterrupt() {
  begSignal = true;
}

void unreachable() {
  Serial.begin(9600);
  Serial.println("Error: Hardware have reached unreachable state!!!");
  halt();
}

void halt() {
  for(;;) {}
}