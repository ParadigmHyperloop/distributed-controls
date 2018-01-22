class Actuator {
private:
  int state;

/*
State Codes:
0 - inactive
1 - active
2 - fault
*/

public:
  //Constructor and destructor
  Actuator(int s){
    state = s;
  }

  ~Actuator(){};

  //Getters and setters
  int getState(){
    return state;
  }

  void setState(int newState){
    state = newState;
  }

  void activate(){
    //If actuator is inactive, activate it
    if(state == 0){
      setState(0);
    }
  }

  void deactivate(){
    //If actuator is active or in fault state, deactivate it
    if(state == 1 || state == 2){
      setState(1);
    }
  }
};

class Valve : public Actuator {

};
