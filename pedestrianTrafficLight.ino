// พร้อมใช้งาน
// ระหว่างคูลดาวน์
// กำลังทำงาน

enum DeviceState {
  ready,
  running,
  cooldown
}

DeviceState deviceStatus = ready;
bool begSignal = false;
unsigned long timingSchedule[] = {
  0, // 0: Time when car light signal is switched
  0, // 1: Time when car countdown signal is switched
  0, // 2: Time when pedestrian light signal is switched
  0  // 3: Time when pedestrian countdown signal is switched
}

void setup() {
  // put your setup code here, to run once:

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
  // put your main code here, to run repeatedly:
  // รอสัญญาณ
  // block interupt
  // คอยค่า
  // แสดงไฟนับถอยหลัง
  // ปรับสี
  // แสดงไฟนับถอยหลัง
  // ปรับสี
  // แสดงไฟนับถอยหลัง
  // ปรับสี
  // ตั้งระยะเวลาบล็อก

}

void scheduleLightSignal() {
  unsigned long now = millis();
  timingSchedule[0] = now + 20*1000 + 50; // Set due time for light signal to become orange, padding for briefly show '0'
  timingSchedule[1] = now +  3*1000;      // Set due time to start car countdown
  timingSchedule[2] = now + 28*1000 + 50; // Set due time for pedestrian light to become green
  timingSchedule[3] = now;                // Set due time to start pedestrian countdown
}

bool isDue(unsigned long time) {
  unsigned long now = millis();
  return abs(now - time)
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