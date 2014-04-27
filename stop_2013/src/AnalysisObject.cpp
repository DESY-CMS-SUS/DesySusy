#include "AnalysisObject.h"
#include "Math/VectorUtil.h"

using namespace ROOT::Math::VectorUtil;

ClassImp(AnalysisObject);

AnalysisObject::AnalysisObject() {
  maptotree=-1;
}

AnalysisObject::AnalysisObject(const AnalysisObject& copy){
  p4 = copy.P4();
  maptotree = copy.IndexInTree();
  id = copy.IDMap();
}

AnalysisObject::~AnalysisObject(){}

LorentzM AnalysisObject::P4() const{return p4;}

int AnalysisObject::IndexInTree() const {return maptotree;}

bool AnalysisObject::Is(const string& key) const {
  map< string, bool>::const_iterator it = id.find(key);

  if (it == id.end() ) {
    cout << "WARNING : AnalysisObject::Is() : Trying to get Id '" << key << "', which does not exist!" << endl;
    return false;
  }
  else {
    return it->second;
  }

  return false;
}

map< string, bool> AnalysisObject::IDMap() const {return id;}
double AnalysisObject::P() const {return p4.P();}
double AnalysisObject::Pt() const {return p4.Pt();}
double AnalysisObject::pt() const {return p4.Pt();}
double AnalysisObject::Eta() const {return p4.Eta();}
double AnalysisObject::eta() const {return p4.Eta();}
double AnalysisObject::Phi() const {return p4.Phi();}
double AnalysisObject::phi() const {return p4.Phi();}
double AnalysisObject::E() const {return p4.E();}
double AnalysisObject::e() const {return p4.E();}
double AnalysisObject::Px() const {return p4.Px();}
double AnalysisObject::px() const {return p4.Px();}
double AnalysisObject::Py() const {return p4.Py();}
double AnalysisObject::py() const {return p4.Py();}
double AnalysisObject::Pz() const {return p4.Pz();}
double AnalysisObject::pz() const {return p4.Pz();}

void AnalysisObject::Set(const int& maptotree_In, const LorentzM& momentum_In){
  SetMapToTree( maptotree_In);
  SetP4( momentum_In);
}
void AnalysisObject::Set(const int& maptotree_In, const LorentzM& momentum_In, const map< string, bool>& id_In){
  Set( maptotree_In, momentum_In);
  SetIDMap( id_In);
}

void AnalysisObject::SetP4(const LorentzM& momentum_In){p4=momentum_In;}


void AnalysisObject::SetMapToTree(const int& maptotree_In){maptotree=maptotree_In;}

void AnalysisObject::SetID(const string& key, const bool& value){
  id[key]=value;
  return;
}

void AnalysisObject::SetIDMap(const map< string, bool>& id_In){id=id_In;}
