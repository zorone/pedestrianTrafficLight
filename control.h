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
#endif