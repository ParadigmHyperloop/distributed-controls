#include "filter.h"

/*
This filter takes a float between 0 and 1 and converts it to on/off output.
*/

class OnOffFilter : public Filter {
private:
  float old_value = 0;
  int state = 0;
  float on_threshold;
  float off_threshold;
  float stepsize;

public:
  OnOffFilter(float on_thresh, float off_thresh, float step){
    on_threshold = on_thresh;
    off_threshold = off_thresh;
    stepsize = step;
  }

  ~OnOffFilter(){}

  float get_old_value(){
    return old_value;
  }

  float filter(float n){
    //Change n to be +/- stepsize from the old value for smoothing
    if(stepsize <= abs(old_value - n)){
      if(n > old_value){
        n = old_value + stepsize;
      } else {
        n = old_value - stepsize;
      }
    }

    //Check if the on/off thresholds have been crossed
    if(old_value > off_threshold && n < off_threshold){
      state = 0;
    } else if (old_value < on_threshold && n > on_threshold){
      state = 1;
    }

    //Set the old value to the new value
    old_value = n;

    return state;
  }

  void reset(){
    old_value = 0.0;
    state = 0;
  }

};
