#ifndef GenJet_h
#define GenJet_h

#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <map>
#include <TString.h>
#include "AnalysisObject.h"
#include "Jet.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

class Jet;

class GenJet: public AnalysisObject {
  
 protected:
  string type;

  bool  isMatched;
  int   matchedJetIndex;
  const Jet*  matchedJet;

 public:
  GenJet();
  GenJet(const GenJet& copy);
  ~GenJet();

  string Type() const;

  bool IsMatched() const;
  int MatchedJetIndex() const;
  LorentzM MatchedJetP4() const;
  const Jet* MatchedJet() const;

  void SetType(const string& type_In);

  void SetIsMatched(const bool& isMatched_In);
  void SetMatchedJetIndex( const int& matchedJetIndex_In);
  void SetMatchedJet( const Jet* matchedJet_In);

  void Set(const int maptotree_In, const LorentzM& momentum_In, const string& type_In="");

  //ClassDef(GenJet,1)
};
#endif
