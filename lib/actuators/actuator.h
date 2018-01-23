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

public:
  //Constructor and destructor
  Actuator(state_t s){
    state = s;
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

class Valve : public Actuator {

};
