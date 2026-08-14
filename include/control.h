#ifndef __PEDESTRIAN_LIGHT_CONTROL_
  #define __PEDESTRIAN_LIGHT_CONTROL_

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
  bool isDue(unsigned long time);

  extern LightSignal carSignal;
  extern LightSignal pedestrianSignal;
  extern CountdownDisplay carCountdown;
  extern CountdownDisplay pedestrianCountdown;
#endif