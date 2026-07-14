#ifndef __PEDESTRIAN_LIGHT_DEBUG_
  #define __PEDESTRIAN_LIGHT_DEBUG_
  #include "control.h"
  
  void printDeviceStatus();
  void printCarStatus();
  void printCarStatusTransition(LightSignal lastCarSignal);
  void printCarCountdownTransition(CountdownDisplay lastCarCountdown);
  void printPedestrianStatus();
  void printPedestrianStatusTransition(LightSignal lastPedestrianSignal);
  void printPedestrianCountdownTransition(CountdownDisplay lastPedestrianCountdown);
#endif