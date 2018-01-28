#include "filter.h"
#include "drivers/actuator_drivers.h"
#include "governors/governors.h"
#include "converters/converters.h"

class Actuator {
  enum state_t{
    ACTIVE,
    ACTIVATING,
    INACTIVE,
    DEACTIVATING,
    FAULT
  };

private:
  state_t state;
  int channel;
  ActuatorDriver *driver;
  Filter *filter;
  Converter *converter;
  Governor *governor;

public:
  //Constructor and destructor
  Actuator(state_t s, int ch; ActuatorDriver *ad, Filter *f, Converter *c, Governor *g){
    state = s;
    channel = ch;
    driver = ad;
    filter = f;
    converter = c;
    governor = g;
  }

  ~Actuator(){};

  //Getters and setters
  state_t getState(){
    return state;
  }

  void setState(state_t newState){
    state = newState;
  }

  void activate(){
    //If actuator is inactive, activate it
    if(state == 0){
      setState(ACTIVE);
    }
  }

  void deactivate(){
    //If actuator is active or in fault state, deactivate it
    if(state == 1 || state == 2){
      setState(INACTIVE);
    }
  }
};
