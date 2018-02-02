#include "filter.h"
#include "drivers/actuator_driver.h"
#include "validators/validator.h"
#include "converters/converter.h"

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
  float target_value;
  ActuatorDriver *driver;
  Filter *filter;
  Converter *converter;
  Validator *validator;

public:
  //Constructor and destructor
  Actuator(state_t s, int ch, ActuatorDriver *ad, Filter *f, Converter *c, Validator *v){
    state = s;
    channel = ch;
    driver = ad;
    filter = f;
    converter = c;
    validator = v;
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
    if(validator->checkValue(value)){ //Validate value
      target_value = value; //Update target value
      float filtered_value = filter->filter(value); //Filter value
      uint32_t converted_value = converter->convert(filtered_value); //Convert value from human readable

      driver->set(converted_value, channel);
    } else {
      //Send an error if value cannot be validatedq
    }
  }

  void update(){
    set(filter->filter(target_value));
  }
};
