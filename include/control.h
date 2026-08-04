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
  void changeCarCountdownSignal(CountdownDisplay signal);
  void changePedestrianLightSignal(LightSignal signal);
  void changePedestrianCountdownSignal(CountdownDisplay signal);

  bool isDue(unsigned long time);

  extern LightSignal carSignal;
  extern LightSignal pedestrianSignal;
  extern CountdownDisplay carCountdown;
  extern CountdownDisplay pedestrianCountdown;
#endif