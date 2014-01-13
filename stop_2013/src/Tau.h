#ifndef Tau_h
#define Tau_h

#include "NtupleTools2_h.h"
#include <map>
#include <string>
#include "Particle.h"

using namespace std;

class Tau: public Particle {

 public:
  Tau();
  Tau(const Tau&);
  ~Tau();  

  virtual void SetCharge(const double& charge_In);
  virtual void SetPdgID(const int& pdgID_In);

  virtual void Set(const int& maptotree_In, const LorentzM& momentum_In, const int& charge_In, const double& iso_In);
  virtual void Set(const int& maptotree_In, const LorentzM& momentum_In, const int& charge_In, const double& iso_In, 
		   const double& dz_In);
  virtual void Set(const int& maptotree_In, const LorentzM& momentum_In, const int& pdgID_In, const int& charge_In,
		   const double& iso_In, const double& dz_In);
  virtual void Set(const int& maptotree_In, const LorentzM& momentum_In, const int& pdgID_In, const int& charge_In,
		   const double& iso_In, const double& dz_In, const map< string, bool>& id_In);
  //ClassDef(Tau,1)
};
#endif
