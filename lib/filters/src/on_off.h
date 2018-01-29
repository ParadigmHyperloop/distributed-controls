#include "filter.h"

/*
This filter takes a float between 0 and 1 and converts it to on/off output.
*/

class OnOffFilter : public Filter {
private:
  float old_value = 0;
  float target_value = 0;
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

  void set_target_value(float n){
    target_value = n;
  }

  float get_target_value(){
    return target_value;
  }

  float filter(float n){
    //Set new target value
    target_value = n;

    //Change n to be +/- stepsize from the old value for smoothing
    if(stepsize <= abs(old_value - target_value)){
      if(target_value > old_value){
        n = old_value + stepsize;
      } else {
        n = old_value - stepsize;
      }
    }

    //Check if the on/off thresholds have been crossed
    int return_val;
    if(old_value > off_threshold && n < off_threshold){
      return_val = 0;
    } else if (old_value < on_threshold && n > on_threshold){
      return_val = 1;
    } else if(n >= 0.5){
      return_val = 1;
    } else {
      return_val = 0;
    }

    //Set the old value to the new value
    old_value = n;

    return return_val;
  }

};
