#ifndef JER_h
#define JER_h

#include "NtupleTools2_h.h"
#include "TRandom3.h"
#include "Math/VectorUtil.h"

#include "Event.h"
#include "Tools.h"
#include "Systematic.h"

#include <vector>

namespace desy_tools{
  double getJerSF(double eta, double err_factor);
  double getJetResolution(double pT, double eta);
  LorentzM Rescale( const LorentzM&, const double&);
};

using namespace ROOT::Math::VectorUtil;

namespace Systematics{

  class JER: public Systematics::Systematic{
  public:
    JER( TString variation_ = "");
    ~JER() {};
    
    double GetCorrectionGenMethod( const LorentzM& jet, const LorentzM& genJet, const double& err_factor);
    double GetCorrectionRecoMethod( const LorentzM& jet, const double& err_factor);

    void Eval( Event& event_);
    void Eval( Event& event_, TString variation_);
    Event* SysEvent();
    
    void Reset();    
  protected:
    TString variation;
    TRandom3 rand;
  };
};
#endif
  
