#include "Particle.h"

#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"

using namespace ROOT::Math::VectorUtil;

//ClassImp( Particle);
Particle::Particle(){
  charge = 99.;
  pdgID = 0;
  iso = -1.;
  dz = -1.;
}

Particle::Particle( const Particle& copy){
  //AnalysisObject( (AnalysisObject) copy);
  maptotree = copy.IndexInTree();
  p4 = copy.P4();
  id = copy.IDMap();
  charge = copy.Charge();
  pdgID = copy.PdgID();
  iso = copy.Iso();
  dz = copy.Dz();
}

Particle::~Particle(){}

double Particle::Charge() const {return charge;}
int    Particle::PdgID()  const {return pdgID;}
double Particle::Iso()    const {return iso;}
double Particle::RelIso() const {return (iso/this->pt());}
bool   Particle::IsIso(double ISO_CUT) const {return this->RelIso()<ISO_CUT;}
double Particle::Dz()     const {return dz;}

void Particle::Set(const int& maptotree_In, const LorentzM& momentum_In, const int& charge_In, const double& iso_In){
  AnalysisObject::Set( maptotree_In, momentum_In);
  SetCharge( charge_In);
  SetIso( iso_In);
}

void Particle::Set(const int& maptotree_In, const LorentzM& momentum_In, const int& pdgId_In, const int& charge_In,
	 const double& iso_In, const double& dz_In){
  Set(maptotree_In, momentum_In, charge_In, iso_In);
  SetPdgID( pdgId_In);
  SetDz( dz_In);
}
void Particle::Set(const int& maptotree_In, const LorentzM& momentum_In, const int& pdgId_In, const int& charge_In,
	 const double& iso_In, const double& dz_In, const map< string, bool>& id_In){
  Set( maptotree_In, momentum_In, pdgId_In, charge_In, iso_In, dz_In);
  SetIDMap( id_In); 
}
void Particle::SetCharge(const double& charge_In){charge=charge_In;}
void Particle::SetPdgID(const int& pdgID_In){pdgID=pdgID_In;};
void Particle::SetIso(const double& iso_In){iso=iso_In;}
void Particle::SetRelIso(const double& reliso_In){iso=reliso_In * this->pt();}
void Particle::SetDz(const double& dz_In){dz=dz_In;}
