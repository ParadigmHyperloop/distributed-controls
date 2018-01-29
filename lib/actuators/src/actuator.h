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
  Actuator(state_t s, int ch, ActuatorDriver *ad, Filter *f, Converter *c, Governor *g){
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

  void set(float value){
    if(governor->checkValue(value)){ //Validate value
      float filtered_value = filter->filter(value); //Filter value
      uint32_t converted_value = converter->convert(filtered_value); //Convert value from human readable

      //If channel is 0, this is a DACtuator
      if(channel == 0){
        driver->set(converted_value);
      } else {
        driver->set(converted_value, channel);
      }
    } else {
      //Send an error
    }
  }
};
