#include "Electron.h"

using namespace std;

ClassImp(Electron);

Electron::Electron(){
  charge=99.;
  pdgID=11;
  iso=-1.;
  dz = -1.;
}

Electron::Electron( const Electron& copy){
  maptotree = copy.IndexInTree();
  p4 = copy.P4();
  id = copy.IDMap();

  charge = copy.Charge();
  pdgID = copy.PdgID();
  iso = copy.Iso();
  dz = copy.Dz();
}

Electron::~Electron(){}

void Electron::SetCharge( const double& charge_In){
  Particle::SetCharge( charge_In);
  if (abs(charge)!=1) cout<<"ERROR: Electron.cpp: muon charge set to "<<charge<<"."<<endl;
}

void Electron::SetPdgID( const int& pdgID_In){
  if (abs(pdgID_In)!=11) cout<<"ERROR: Electron.cpp: trying to set to electron pdgID to"<<pdgID_In<<"."<<endl;
  
  if ( charge == 1 && pdgID_In == -11) {
    pdgID=-11;
  }
  else if(charge == -1 && pdgID_In == 11){
    pdgID=11;
  }
  else{
    cout<<"ERROR: Electron.cpp: inconsistency in electron charge and electron pdgID."<<endl;
  }
}
void Electron::Set(const int& maptotree_In, const LorentzM& momentum_In, const int& charge_In, const double& iso_In){
  AnalysisObject::Set( maptotree_In, momentum_In);
  SetCharge( charge_In);
  if ( charge == 1) {
    SetPdgID(-11);
  }
  else if(charge == -1){
    SetPdgID(11);
  }
  SetIso( iso_In);
}
void Electron::Set(const int& maptotree_In, const LorentzM& momuntum_In, const int& charge_In, const double& iso_In,
		   const double& dz_In){
  Set(maptotree_In, momuntum_In, charge_In, iso_In);
  SetDz( dz_In);
}
void Electron::Set(const int& maptotree_In, const LorentzM& momuntum_In, const int& pdgID_In, const int& charge_In,
		   const double& iso_In, const double& dz_In){
  Set(maptotree_In, momuntum_In, charge_In, iso_In, dz_In);
  SetPdgID( pdgID_In);
}
void Electron::Set(const int& maptotree_In, const LorentzM& momuntum_In, const int& pdgID_In, const int& charge_In,
		   const double& iso_In, const double& dz_In, const map< string, bool>& id_In){
  Set( maptotree_In, momuntum_In, pdgID_In, charge_In, iso_In, dz_In);
  SetIDMap( id_In); 
}
