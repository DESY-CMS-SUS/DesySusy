#ifndef Particle_h
#define Particle_h

#include "NtupleTools2_h.h"
#include "AnalysisObject.h"

using namespace std;

class Particle : public AnalysisObject {
  
 protected:
  double  charge;
  int     pdgID;
  double  iso;
  double  dz;

 public:
  Particle();
  Particle(const Particle&);
  ~Particle();  

  double Charge() const;
  int    PdgID() const;
  double Iso() const;
  double RelIso() const;
  bool   IsIso(double ISO_CUT=0.10) const;
  double Dz() const;

  virtual void SetCharge(const double& charge_In);
  virtual void SetPdgID(const int& pdgID_In);
  void SetIso(const double& iso_in); 
  void SetRelIso(const double& reliso_in);
  void SetDz(const double& dz_in);
  
  virtual void Set(const int& maptotree_In, const LorentzM& momentum_In, const int& charge_In, const double& iso_In);
  virtual void Set(const int& maptotree_In, const LorentzM& momentum_In, const int& pdgID_In, const int& charge_In,
		   const double& iso_In, const double& dz_In);
  virtual void Set(const int& maptotree_In, const LorentzM& momentum_In, const int& pdgID_In, const int& charge_In,
		   const double& iso_In, const double& dz_In, const map< string, bool>& id_In);
  ClassDef(Particle,1);
};
#endif
