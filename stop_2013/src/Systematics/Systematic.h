#ifndef Systematic_h
#define Systematic_h

#include "Event.h"
#include "NtupleTools2.h"

#include <vector>

namespace Systematics{
  
  class Systematic{
  public:
    virtual void Eval( Event& event) = 0;
    virtual Event* SysEvent() = 0;
    
    Event* InEvent;
    virtual void Reset() = 0;  
    
  protected:
    Event  OutEvent;
  };
};
#endif
