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

  Float_t sample;
  Float_t LepFromTop;
  Float_t Charginos;

  Float_t xs;
  Float_t NEvents;
  Float_t FE;
  Float_t GlobalWeight;

  Float_t TriggerWeight;
  
  Float_t PUInter;
  Float_t PUWeight;
  Float_t PUWeight_up;
  Float_t PUWeight_down;
  Float_t NPV;
  Float_t NgoodPV;

  Float_t isrWeight;
  Float_t topPtWeight;

  Float_t evt;
  Float_t run;
  Float_t lumi;

  Float_t mStop;
  Float_t mLSP;
  
  Float_t T2ttL;
  Float_t T2ttR;
  
  Float_t T2bWLL;
  Float_t T2bWLS;
  Float_t T2bWLR;
  
  Float_t T2bWSL;
  Float_t T2bWSS;
  Float_t T2bWSR;
  
  Float_t T2bWRL;
  Float_t T2bWRS;
  Float_t T2bWRR;

  Float_t njets;
  Float_t jet1;
  Float_t jet2;
  Float_t jet3;
  Float_t jet4;

  Float_t nbjets;
  vector<Float_t>* bdisc;
  vector<Float_t>* bdiscNoReshape;
  Float_t bjet1;
  Float_t bjetHighestDisc;
  Float_t discH;
  
  Float_t lPt;
  Float_t lEta;
  Float_t lRelIso;

  Float_t isoTrack;
  Float_t tauVeto;
  
  Float_t rawmet;
  Float_t typeImet;
  Float_t phiCorrMet;
  
  Float_t ht;
  Float_t ht3;
  Float_t ht4;
  Float_t ht5;
  Float_t htRatio;
  Float_t meff;
  Float_t y;
  
  Float_t mt;
  Float_t mlb1;
  Float_t mlb;
  Float_t m3b;
  Float_t m3;
  Float_t centrality;
  Float_t centralityNoLep;
  Float_t mt2w;
  Float_t hadChi2;
  Float_t topness;

  Float_t dphimin;
  Float_t drlb1;
  Float_t drlbmin;

  Int_t pdgIdLep1;
  Int_t pdgIdLep2;

  Bool_t kinRegion;
  Bool_t searchRegionPre;
  Bool_t searchRegionPost;
  Bool_t CR1;
  Bool_t CR4;
  Bool_t CR5;
};
#endif
