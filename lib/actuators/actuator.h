class Actuator {
private:
  enum state_t{
    ACTIVE = 0,
    ACTIVATING = 1,
    INACTIVE = 2,
    DEACTIVATING = 3,
    FAULT = 4
  };

  state_t state;
/*
State Codes:
0 - inactive
1 - active
2 - fault
*/

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
