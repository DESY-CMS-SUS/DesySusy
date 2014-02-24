#ifndef Bonsai_h
#define Bonsai_h

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

class Bonsai: public SubTree {
 public:
  Bonsai();
  Bonsai(TDirectory*, TString);
  ~Bonsai(){ delete tree;};


  virtual void Fill() { this->Fill( 0, 0);};
  virtual void Fill( Event* event) { this->Fill( event, 0);};
  virtual void Fill( EasyChain* chain) { this->Fill( 0, chain);};
  virtual void Fill( Event* event, EasyChain* chain);

 protected:
  void Reset();
  void SetBranches();

  double sample;
  double LepFromTop;
  double Charginos;

  double xs;
  double NEvents;
  double FE;
  double GlobalWeight;

  double TriggerWeight;
  
  double PUInter;
  double PUWeight;
  double PUWeight_up;
  double PUWeight_down;
  double NPV;
  double NgoodPV;

  double isrWeight;
  double topPtWeight;

  double evt;
  double run;
  double lumi;

  double mStop;
  double mLSP;
  
  double T2ttL;
  double T2ttR;
  
  double T2bWLL;
  double T2bWLS;
  double T2bWLR;
  
  double T2bWSL;
  double T2bWSS;
  double T2bWSR;
  
  double T2bWRL;
  double T2bWRS;
  double T2bWRR;

  int Region;
  int Selection; 

  double njets;
  double jet1;
  double jet2;
  double jet3;
  double jet4;

  double nbjets;
  double bjet1;
  double disc1;
  double bjetHighestDisc;
  double discH;
  
  double lPt;
  double lEta;
  double lRelIso;

  double isoTrack;
  double tauVeto;
  
  double rawmet;
  double typeImet;
  double phiCorrMet;
  
  double ht;
  double htRatio;
  double meff;
  double y;
  
  double mt;
  double mlb1;
  double mlb;
  double m3b;
  double mt2w;
  double hadChi2;
  double topness;

  double dphimin;
  double drlb;
};
#endif
