#ifndef Jet_h
#define Jet_h

#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <map>
#include <TString.h>
#include "AnalysisObject.h"
#include "GenJet.h"

class GenJet;

using namespace std;
using namespace ROOT::Math::VectorUtil;

class Jet: public AnalysisObject {
  
 protected:
  string type;

  static map<string, map<string, double> > bJetWP;
  map<string, double>  bJetDisc;

  map<string, double>  jECorrFactor;
  map<string, double>  jECorrUncertainty;

  bool  isMatched;
  int matchedGenJetIndex;
  int partonFlavor;
  const GenJet* matchedGenJet;

 public:
  Jet();
  Jet(const Jet&);
  ~Jet();

  string Type()const;

  map< string, map< string, double> > GetbJetWP() const;
  map< string, double>  BJetDisc() const;
  double BJetDisc(const string& key) const;
  bool IsBJet(const string& key, const string& WP) const;
  bool IsBJet(const string& key="CSV", const double& disc_cut=0.679) const;

  map< string, double> JECorrFactor() const;
  map< string, double> JECorrUncertainty() const;
  double JECorrFactor( const string& corr) const;
  double JECorrUncertainty( const string& corr) const;

  bool IsMatched() const;
  int MatchedGenJetIndex() const;
  int PartonFlavor() const;
  LorentzM MatchedGenJetP4() const;
  const GenJet* MatchedGenJet() const;

  void SetType(const string& type_In);

  void SetBJetDisc(const map<string, double>& bJetDisc_In);
  void SetBJetDisc(const string& key, const double& value);
  static void SetWP(const string& cme="8TeV");

  void SetJECorrFactor(const map<string, double>& jECorrFactor_In);
  void SetJECorrUncertainty(const map<string, double>& jECorrUncertainty_In);
  void SetJECorrFactor(const string& key, const double& value);
  void SetJECorrUncertainty(const string& key, const double& value);

  void SetIsMatched(const bool& isMatched_In);
  void SetMatchedGenJetIndex(const int& matchedGenJetIndex_In);
  void SetPartonFlavor(const int& partonFlavor_In);
  void SetMatchedGenJet( const GenJet* matchedGenJet_In);

  void Set(const int maptotree_In, const LorentzM& momentum_In, const double& scaleCorrFactor_In=1., const string& type_In="");

  ClassDef(Jet,1)
};
#endif
