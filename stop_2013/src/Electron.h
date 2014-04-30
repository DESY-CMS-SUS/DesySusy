#ifndef Electron_h
#define Electron_h

#include "NtupleTools2_h.h"
#include <map>
#include <string>
#include "Particle.h"

using namespace std;

class Electron: public Particle {

 public:
  Electron();
  Electron(const Electron&);
  ~Electron();  

  virtual void SetCharge(const double& charge_In);
  virtual void SetPdgID(const int& pdgID_In);

  virtual void Set(const int& maptotree_In, const LorentzM& momuntum_In, const int& charge_In, const double& iso_In);
  virtual void Set(const int& maptotree_In, const LorentzM& momuntum_In, const int& charge_In, const double& iso_In, 
		   const double& dz_In);
  virtual void Set(const int& maptotree_In, const LorentzM& momuntum_In, const int& pdgID_In, const int& charge_In,
		   const double& iso_In, const double& dz_In);
  virtual void Set(const int& maptotree_In, const LorentzM& momuntum_In, const int& pdgID_In, const int& charge_In,
		   const double& iso_In, const double& dz_In, const map< string, bool>& id_In);

  ClassDef(Electron,1);
};
#endif
