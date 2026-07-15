// พร้อมใช้งาน
// ระหว่างคูลดาวน์
// กำลังทำงาน

enum DeviceState {
  ready,
  running,
  cooldown
}

enum LightSignal {
  green,
  orange,
  red
}

enum CountdownDisplay {
  hide,
  show
}

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
}

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
    default unreachable();
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
          case orange: 
            timingSchedule[1] = timingSchedule[0];
            break;
          case red:
            // timingSchedule[1] = timingSchedule[0];
            carCountdown = hide; 
            break;
          default: unreachable();
        }
        break;
      case show: {
        switch(carSignal) {
          case green: break;
          case orange: break;
          case red: break;
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
      case red: break;
      case green: break;
      case orange: default: unreachable();
    }
  }
}

void changePedestrianCountdown() {
  if(isDue(timingSchedule[3])) {
    switch(pedestrianCountdown) {
      case hide: break;
      case show: break;
      default: unreachable();
    }
  }
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