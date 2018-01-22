#ifndef PARADIGM_HELPERS_H
#define PARADIGM_HELPERS_H

#include "paradigm.h"

class Validatable {
public:
  ~Validatable() {}
  virtual bool isValid(void) = 0;
};

template <class T>
class Retriable {
    uint32_t retries = 0;
protected:
    virtual T run_once(void) = 0;

public:
    virtual ~Retriable() {}
    virtual void setRetries(uint32_t n) {
      retries = n;
    };
    virtual uint32_t getRetries(void) {
      return retries;
    };

    T run(void) {
      uint32_t attempts = retries + 1;

      while (1) {
        T ret = run_once();

        if (ret.isValid()) {
          return ret;
        }

        attempts--;

        if (attempts == 0) {
          return ret;
        }
      };

    };
};

#endif
