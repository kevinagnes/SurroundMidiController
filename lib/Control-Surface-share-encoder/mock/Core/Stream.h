#ifndef Stream_h
#define Stream_h

#include "Print.h"

class Stream : public Print {
  public:
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual int available() = 0;
};

#endif