#ifndef PUReweight_h
#define PUReweight_h

#include "NtupleTools2_h.h"

#include "Event.h"
#include "Systematic.h"

#include <vector>

namespace Systematics{

  class PUReweight: public Systematics::Systematic{
  public:
    PUReweight( TString variation_ = "");
    ~PUReweight() {};
    
    void Eval( Event& event_);
    void Eval( Event& event_, TString variation_);
    Event* SysEvent();
    
    void Reset();    
  protected:
    TString variation;
  };
};
#endif
  
