#ifndef makeMET_h
#define makeMET_h

#include "NtupleTools2_h.h"
#include "Jet.h"

extern bool pcp;

template <class MET> LorentzM makeMET( EasyChain* tree, const char* metName);
LorentzM makeType1MET( EasyChain* tree, const LorentzM& met_in, const vector<Jet*>& jets);
LorentzM makePhiCorrectedMET( EasyChain* tree, const LorentzM& met_in);

template <class MET>
LorentzM makeMET( EasyChain* tree, const char* metName){
  MET& met = tree->Get( &met, metName);

  LorentzM met_out = LorentzM(); met_out.SetPxPyPzE ( met.Px(), met.Py(), met.Pz(), met.E());

  if(pcp) cout<<metName<<": pt="<<met_out.Pt()<<"; phi="<<met_out.Phi()<<"; eta="<<met_out.Eta()<<"; "<<met.Pt()-met_out.Pt()<<endl;

  return met_out;
}
#endif
