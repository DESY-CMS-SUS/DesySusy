#ifndef AnalysisObject_h
#define AnalysisObject_h

#include "TObject.h"

#include "NtupleTools2_h.h"

using namespace std;

class AnalysisObject: public TObject{

protected:
  int maptotree;
  LorentzM  p4;
  map< string, bool> id;  

public:
  AnalysisObject();
  AnalysisObject(const AnalysisObject&);
  ~AnalysisObject();  

  int IndexInTree() const;
  LorentzM P4() const;
  bool Is(const string& id) const;
  map< string, bool> IDMap() const;

  double P() const;
  double Pt() const;
  double pt() const;
  double Eta() const;
  double eta() const;
  double Phi() const;
  double phi() const;
  double E() const;
  double e() const;
  double Px() const;
  double px() const;
  double Py() const;
  double py() const;
  double Pz() const;
  double pz() const;
  //
  void Set(const int& maptotree_In, const LorentzM& momentum_In);
  void Set(const int& maptotree_In, const LorentzM& momentum_In, const map< string, bool>& id_In);
  void SetMapToTree(const int& maptotree_In);
  void SetP4(const LorentzM& momentum_In);
  void SetID(const string& key, const bool& value);
  void SetIDMap(const map< string, bool>& id_In);

  ClassDef(AnalysisObject,1);
};
#endif
