#ifndef PARADIGM_NODE_H
#define PARADIGM_NODE_H

#include <paradigm.h>
#include <sensor.h>
#include <string.h>

#define MAX_SENSORS 24

enum state_t{
  RUN,
  CONFIG,
  EMERGENCY
};

class Node {
  Sensor *sensors[MAX_SENSORS] = {0};
  String name = "";
  state_t state;

public:
  Node(String n, state_t s){
    name = n;
    state = s;
  }

  ~Node(){}

  void setState(state_t s){
    state = s;
  }

  state_t getState(){
    return state;
  }

  bool registerSensor(Sensor *s) {
    for (int i = 0; i < MAX_SENSORS; i++) {
      if (sensors[i] == NULL) {
        sensors[i] = s;
        return true;
      }
    }
    return false;
  }

  bool unregisterSensor(Sensor *s) {
    for (int i = 0; i < MAX_SENSORS; i++) {
      if (sensors[i] == s) {
        sensors[i] = NULL;
        return true;
      }
    }
    return false;
  }

  Sensor * getSensorByName(String s) {
    for (int i = 0; i < MAX_SENSORS; i++) {
      if (sensors[i] && sensors[i]->getName().compareTo(s) == 0) {
        return sensors[i];
      }
    }
    return NULL;
  }

  float getSensorValue(Sensor *s){
    for (int i = 0; i < MAX_SENSORS; i++){
      if(sensors[i] == s){
        return sensors[i]->getValue();
      }
    }
  }

  void engageEmergency(){
    state = EMERGENCY;
    //Code to safe the node here
  }

  void setName(String n) {
    name = n;
  }

  String getName(void) {
    return name;
  }
};

#endif
