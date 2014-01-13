#ifndef TriggerStudyTree_h
#define TriggerStudyTree_h

#include "SubTree.h"

#include "TFile.h"
#include "TDirectory.h"
#include "TString.h"
#include "TTree.h"
#include "TMath.h"

#include "Event.h"
#include "Particle.h"
#include "Muon.h"
#include "Electron.h"
#include "Tau.h"
#include "Jet.h"
#include "GenJet.h"
#include "IsoTrackVeto.h"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;
using namespace ROOT::Math::VectorUtil;

class TriggerStudyTree: public SubTree {
 public:
  TriggerStudyTree();
  TriggerStudyTree( TDirectory*, TString);
  ~TriggerStudyTree(){};

  void Fill() { Fill( 0, 0);};
  void Fill( Event* event) { Fill( event, 0);};
  void Fill( EasyChain* chain) { Fill( 0, chain);};
  void Fill( Event* event, EasyChain* chain);

 protected:
  void Reset();
  void SetBranches();
  
  //these are the leaves to write into a root file
  int eventNum;
  int run;
  int PUInter;
  double PUWeight;
  double weight;

  int foundDiMuon;
  vector<LorentzM> muonP4;
  vector<int> muonCharge;
  vector<int> muonIsSelected;

  int foundDiElectron;
  vector<LorentzM> electronP4;
  vector<int> electronCharge;
  vector<int> electronIsSelected;

  vector<LorentzM> jetP4;
  vector<int> jetIsSelected;

  double HT;
  double MET;

  vector<string> DESYtriggerElMatchedTriggerFilter;
  vector<string> DESYtriggerMuMatchedTriggerFilter;
  map<string, string> DESYtriggerNameMap;

  map<string, int> prescaled;
  map<string, bool> triggered;
};
#endif
