#ifndef BTagReweight_h
#define BTagReweight_h

#include "NtupleTools2_h.h"

#include "Event.h"
#include "BTagReshaping.h"
#include "Systematic.h"

#include <vector>

namespace Systematics{

  class BTagReweight: public Systematics::Systematic{
  public:
    BTagReweight( TString variation_ = "");
    ~BTagReweight() {};
    
    void Eval( Event& event_);
    void Eval( Event& event_, TString variation_);
    Event* SysEvent();
    
    void Reset();    
  protected:
    TString variation;
    
    BTagShapeInterface* BTagReshape_upBC;
    BTagShapeInterface* BTagReshape_downBC;
    BTagShapeInterface* BTagReshape_upLight;
    BTagShapeInterface* BTagReshape_downLight;
  };
};
#endif
  
