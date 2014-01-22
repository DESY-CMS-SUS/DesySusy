#ifndef NoSystematic_h
#define NoSystematic_h

#include "NtupleTools2_h.h"

#include "Event.h"
#include "BTagReshaping.h"
#include "Systematic.h"

#include <vector>

namespace Systematics{

  class NoSystematic: public Systematics::Systematic{
  public:  
    NoSystematic();
    ~NoSystematic() {};
    
    void Eval( Event& event_);
    Event* SysEvent();
    
    Event* InEvent;
    void Reset();
    
  protected:
    Event  OutEvent;
    BTagShapeInterface* BTagReshape_nominal;
  };
};
#endif
  
