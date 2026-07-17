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
bool begSignal = false;
unsigned long timingSchedule[] = {
  0, // 0: Time when car light signal is switched
  0, // 1: Time when car countdown signal is switched
  0, // 2: Time when pedestrian light signal is switched
  0, // 3: Time when pedestrian countdown signal is switched
  0  // 4: Time when beg signal will available again
};

void scheduleLightSignal();
void changeCarSignal();
void changeCarCountdown();
void changePedestrianSignal();
void changePedestrianCountdown();
bool isCooldownFinish();

bool isDue(unsigned long time);
void begSignalInterrupt();
void unreachable();

void halt();

void setup() {

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
      changeCarSignal();
      changeCarCountdown();
      changePedestrianSignal();
      changePedestrianCountdown();
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

void changeCarSignal() {
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

void changeCarCountdown() {
  if(isDue(timingSchedule[1])) {
    switch(carCountdown) {
      case hide: 
        switch(carSignal) {
          case green: 
            carCountdown = show;
            timingSchedule[1] = timingSchedule[0];  // Assume that changeCarCountdown() happens after changeCarSignal()
            break;
          case orange: unreachable(); // Assume that changeCarSignal() happens beforehand, and doesn't get changed anywhere else.
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

void changePedestrianSignal() {
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

void changePedestrianCountdown() {
  if(isDue(timingSchedule[3])) {
    switch(pedestrianCountdown) {
      case hide:
        switch(pedestrianSignal) {
          case red:
            timingSchedule[3] = timingSchedule[2]; // Assume that changePedestrianCountdown() happens after changePedestrianSignal()
            pedestrianCountdown = show;
            break;
          case green: unreachable(); // Assume that changePedestrianSignal() happens beforehand, and doesn't get changed anywhere else.
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