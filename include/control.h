#ifndef __PEDESTRIAN_LIGHT_CONTROL_
  #define __PEDESTRIAN_LIGHT_CONTROL_
  
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

  extern DeviceState deviceStatus = ready;
  extern LightSignal carSignal = green;
  extern LightSignal pedestrianSignal = red;
  extern CountdownDisplay carCountdown = hide;
  extern CountdownDisplay pedestrianCountdown = hide;
  extern volatile bool begSignal = false;
  extern unsigned long timingSchedule[] = {
      0,    // 0: Time when car light signal is switched
      0,    // 1: Time when car countdown signal is switched
      0,    // 2: Time when pedestrian light signal is switched
      0,    // 3: Time when pedestrian countdown signal is switched
      0     // 4: Time when beg signal will available again
  };

  extern bool prevInput = false, currentInput = false;
  extern unsigned long inputTimeframe = 0;

  extern volatile unsigned long debuggingSchedule = 0;
  extern bool enableTextOutput = true;
#endif