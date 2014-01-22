#ifndef JES_h
#define JES_h

#include "NtupleTools2_h.h"
#include "Math/VectorUtil.h"

#include "Event.h"
#include "Tools.h"
#include "Systematic.h"

#include <vector>

namespace desy_tools{
  LorentzM Rescale( const LorentzM&, const double&);
};

using namespace ROOT::Math::VectorUtil;

namespace Systematics{

  class JES: public Systematics::Systematic{
  public:
    JES( TString variation_ = "");
    ~JES() {};
    
    void Eval( Event& event_);
    void Eval( Event& event_, TString variation_);
    Event* SysEvent();
    
    void Reset();    
  protected:
    TString variation;
  };
};
#endif
  
