#include "Muon.h"

using namespace std;

//ClassImp(Muon);

Muon::Muon(){
  pdgID=13;
}

Muon::Muon( const Muon& copy){
  maptotree = copy.IndexInTree();
  p4 = copy.P4();
  id = copy.IDMap();

  charge = copy.Charge();
  pdgID = copy.PdgID();
  iso = copy.Iso();
  dz = copy.Dz();
}

Muon::~Muon(){}

void Muon::SetCharge( const double& charge_In){
  Particle::SetCharge( charge_In);
  if (abs(charge)!=1) cout<<"ERROR: Muon.cpp: muon charge set to "<<charge<<"."<<endl;
}

void Muon::SetPdgID( const int& pdgID_In){
  if (abs(pdgID_In)!=13) cout<<"ERROR: Muon.cpp: trying to set to muon pdgID to"<<pdgID_In<<"."<<endl;
  
  if ( charge == 1 && pdgID_In == -13) {
    pdgID=-13;
  }
  else if(charge == -1 && pdgID_In == 13){
    pdgID=13;
  }
  else{
    cout<<"ERROR: Muon.cpp: inconsistency in muon charge and muon pdgID."<<endl;
  }
}
void Muon::Set(const int& maptotree_In, const LorentzM& momentum_In, const int& charge_In, const double& iso_In){
  AnalysisObject::Set( maptotree_In, momentum_In);
  SetCharge( charge_In);
  if ( charge == 1) {
    SetPdgID(-13);
  }
  else if(charge == -1){
    SetPdgID(13);
  }
  SetIso( iso_In);
}
void Muon::Set(const int& maptotree_In, const LorentzM& momuntum_In, const int& charge_In, const double& iso_In,
	       const double& dz_In){
  Set(maptotree_In, momuntum_In, charge_In, iso_In);
  SetDz( dz_In);
}
void Muon::Set(const int& maptotree_In, const LorentzM& momentum_In, const int& pdgID_In, const int& charge_In,
	       const double& iso_In, const double& dz_In){
  Set(maptotree_In, momentum_In, charge_In, iso_In, dz_In);
  SetPdgID( pdgID_In);
}
void Muon::Set(const int& maptotree_In, const LorentzM& momentum_In, const int& pdgID_In, const int& charge_In,
	       const double& iso_In, const double& dz_In, const map< string, bool>& id_In){
  Set( maptotree_In, momentum_In, pdgID_In, charge_In, iso_In, dz_In);
  SetIDMap( id_In); 
}
