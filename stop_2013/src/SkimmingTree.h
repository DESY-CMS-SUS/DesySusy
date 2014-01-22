#ifndef SkimmingTree_h
#define SkimmingTree_h

#include "SubTree.h"
#include "NtupleTools2_h.h"

#include "Event.h"
#include "EventInfo.h"
#include "Particle.h"
#include "Muon.h"
#include "Electron.h"
#include "Tau.h"
#include "Jet.h"
#include "GenJet.h"

#include "TDirectory.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TMath.h"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;
using namespace ROOT::Math::VectorUtil;

class SkimmingTree: public SubTree {
 public:
  
  SkimmingTree();
  SkimmingTree(TDirectory* = 0, TString = "");

  ~SkimmingTree(){
    delete(tracks);
    delete(muons);
    delete(electrons);
    delete(taus);
    delete(jets);
    delete(genJets);
    delete(tree);
  };


  virtual void Fill() { this->Fill( 0, 0);};
  virtual void Fill( Event* event) { this->Fill( event, 0);};
  virtual void Fill( EasyChain* chain) { this->Fill( 0, chain);};
  virtual void Fill(Event* event, EasyChain* chain);

  EventInfo*        info;
  vector<Particle>* tracks;
  vector<Muon>*     muons;
  vector<Electron>* electrons;
  vector<Tau>*      taus;
  vector<Jet>*      jets;
  vector<GenJet>*   genJets;
  LorentzM*         unclusteredEnergy;
  LorentzM*         rawMET;
  LorentzM*         typeIMET;
  LorentzM*         typeIPhiCorrMET;
  LorentzM*         mvaMET;
  LorentzM*         caloMET;

 protected:
  void Reset();
  void SetBranches();
};
#endif
