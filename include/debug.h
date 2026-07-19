#ifndef __PEDESTRIAN_LIGHT_DEBUG_
  #define __PEDESTRIAN_LIGHT_DEBUG_
  #include "control.h"
  
  void printDeviceStatus();
  void printCarStatus();
  void printCarStatusTransition(LightSignal lastCarSignal);
  void printCarStatusTransition(LightSignal lastCarSignal, CountdownDisplay lastCarCountdown);
  void printPedestrianStatus();
  void printCurrentInput();
#endif