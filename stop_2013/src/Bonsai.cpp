#include "Bonsai.h"

#include "TObjArray.h"
#include "TObjString.h"

Bonsai::Bonsai(): SubTree(){
  home=0;
  tree= new TTree("bonsai", "Bonsai subTree");

  this->Reset();
  this->SetBranches();
}

Bonsai::Bonsai(TDirectory* indir, TString newdir): SubTree( indir, newdir){
  TDirectory* keep = gDirectory->GetDirectory("");
  if( home!= 0)
    home->cd();

  tree= new TTree("bonsai", "Bonsai subTree");
  
  this->Reset();
  this->SetBranches();
  
  keep->cd();
}

void Bonsai::Reset()
{
  sample = 0.;
  xs = 0.;
  NEvents = 0.;
  GlobalWeight = 0.;

  TriggerWeight = 0.;

  PUInter = 0.;
  PUWeight = 0.;
  PUWeight_up = 0.;
  PUWeight_down = 0.;
  NPV = 0.;
  NgoodPV = 0.;

  isrWeight = 0.;
  topPtWeight = 0.;

  evt = 0.;
  run = 0.;
  lumi = 0.;

  njets = 0.;
  jet1 = 0.;
  jet2 = 0.;
  jet3 = 0.;
  jet4 = 0.;
  
  nbjets = 0.;
  bjet1 = 0.;
  disc1 = 0.;
  bjetHighestDisc = 0.;
  discH = 0.;
  
  lPt = 0.;
  lEta = 0.;
  lRelIso = 0.;
  eoverp = 0.;
  
  isoTrack = 0.;
  tauVeto = 0.;
  
  rawmet = 0.;
  typeImet = 0.;
  phiCorrMet = 0.;
  
  ht = 0.;
  meff = 0.;
  y = 0.;

  mt = 0.;
  mlb1 = 0.;
  mlb = 0.;
  m3b = 0.;
  mt2w = 0.;
  hadChi2 = 0.;
  topness = 0.;

  dphimin = 0.;
  drlb = 0.;

  mY = 0.;
  mLsp = 0.;
  x = 0.;
  
  T2ttL = 0.;
  T2ttR = 0.;
  
  T2bWLL = 0.;
  T2bWLS = 0.;
  T2bWLR = 0.;
  
  T2bWSL = 0.;
  T2bWSS = 0.;
  T2bWSR = 0.;
  
  T2bWRL = 0.;
  T2bWRS = 0.;
  T2bWRR = 0.;
}


void Bonsai::SetBranches()
{

  tree->Branch("sample",&sample,"sample/D");
  tree->Branch("xs",&xs,"xs/D");
  tree->Branch("NEvents",&NEvents,"NEvents/D");
  tree->Branch("GlobalWeight",&GlobalWeight,"GlobalWeight/D");

  tree->Branch("TriggerWeight",&TriggerWeight,"TriggerWeight/D");

  tree->Branch("PUInter",&PUInter,"PUInter/D");
  tree->Branch("PUWeight",&PUWeight,"PUWeight/D");
  tree->Branch("PUWeight_up",&PUWeight_up,"PUWeight_up/D");
  tree->Branch("PUWeight_down",&PUWeight_down,"PUWeight_down/D");
  tree->Branch("NPV",&NPV,"NPV/D");
  tree->Branch("NgoodPV",&NgoodPV,"NgoodPV/D");

  tree->Branch("isrWeight",&isrWeight,"isrWeight/D");
  tree->Branch("topPtWeight",&topPtWeight,"topPtWeight/D");

  tree->Branch("evt",&evt,"evt/D");
  tree->Branch("run",&run,"run/D");
  tree->Branch("lumi",&lumi,"lumi/D");
  
  tree->Branch("njets",&njets,"njets/D");
  tree->Branch("jet1", &jet1, "jet1/D");
  tree->Branch("jet2", &jet2, "jet2/D");
  tree->Branch("jet3", &jet3, "jet3/D");
  tree->Branch("jet4", &jet4, "jet4/D");
  
  tree->Branch("nbjets",&nbjets,"nbjets/D");
  tree->Branch("bjet1", &bjet1, "bjet1/D");
  tree->Branch("disc1", &disc1, "disc1/D");
  tree->Branch("bjetHighestDisc", &bjetHighestDisc, "bjetHighestDisc/D");
  tree->Branch("discH", &discH, "discH/D");
  
  tree->Branch("lPt",&lPt,"lPt/D");
  tree->Branch("lEta",&lEta,"lEta/D");
  tree->Branch("lRelIso",&lRelIso,"lRelIso/D");
  tree->Branch("eoverp",&eoverp,"eoverp/D");
  
  tree->Branch("isoTrack",&isoTrack,"isoTrack/D");
  tree->Branch("tauVeto", &tauVeto, "tauVeto/D");

  tree->Branch("rawmet",&rawmet,"rawmet/D");
  tree->Branch("typeImet", &typeImet, "typeImet/D");
  tree->Branch("phiCorrMet", &phiCorrMet, "phiCorrMet/D");
  
  tree->Branch("ht",&ht,"ht/D");
  tree->Branch("meff",&meff,"meff/D");
  tree->Branch("y",&y,"y/D");

  tree->Branch("mt",&mt,"mt/D");
  tree->Branch("mlb1",&mlb1,"mlb1/D");
  tree->Branch("mlb",&mlb,"mlb/D");
  tree->Branch("m3b",&m3b,"m3b/D");
  tree->Branch("mt2w",&mt2w,"mt2w/D");
  tree->Branch("hadChi2",&hadChi2,"hadChi2/D");
  tree->Branch("topness",&topness,"topness/D");

  tree->Branch("dphimin",&dphimin,"dphimin/D");
  tree->Branch("drlb",&drlb,"drlb/D");
  /*
  tree->Branch("mY",&mY,"mY/D");
  tree->Branch("mLsp",&mLsp,"mLsp/D");
  tree->Branch("x",&x,"x/D");

  tree->Branch("T2ttL",&T2ttL,"T2ttL/D");
  tree->Branch("T2ttR",&T2ttR,"T2ttR/D");

  tree->Branch("T2bWLL",&T2bWLL,"T2bWLL/D");
  tree->Branch("T2bWLS",&T2bWLS,"T2bWLS/D");
  tree->Branch("T2bWLR",&T2bWLR,"T2bWLR/D");

  tree->Branch("T2bWSL",&T2bWSL,"T2bWSL/D");
  tree->Branch("T2bWSS",&T2bWSS,"T2bWSS/D");
  tree->Branch("T2bWSR",&T2bWSR,"T2bWSR/D");

  tree->Branch("T2bWRL",&T2bWRL,"T2bWRL/D");
  tree->Branch("T2bWRS",&T2bWRS,"T2bWRS/D");
  tree->Branch("T2bWRR",&T2bWRR,"T2bWRR/D");*/
}  

void Bonsai::Fill( Event* event, EasyChain* chain)
{
  this->Reset();
  
  sample = 0.;
  xs = event->Info()->xs;
  NEvents = event->Info()->NEvents;
  GlobalWeight = event->Info()->GlobalWeight;

  TriggerWeight = event->Info()->TriggerWeight;

  PUInter = event->Info()->PUInter;
  PUWeight = event->Info()->PUWeight;
  PUWeight_up = event->Info()->PUWeight_up;
  PUWeight_down = event->Info()->PUWeight_down;
  NPV = event->Info()->NPV;
  NgoodPV = event->Info()->NgoodPV;

  isrWeight = event->Info()->isrWeight;
  topPtWeight = event->Info()->topPtWeight;

  evt = event->Info()->Event;
  run = event->Info()->Run;
  lumi = event->Info()->Lumi;
  
  njets = event->nJets();
  if (njets > 0.)
    jet1 = event->Jets("Selected")->at(0)->Pt();
  if (njets > 1.)
    jet2 = event->Jets("Selected")->at(1)->Pt();
  if (njets > 2.)
    jet3 = event->Jets("Selected")->at(2)->Pt();
  if (njets > 3.)
    jet4 = event->Jets("Selected")->at(3)->Pt();
  
  nbjets = event->nBJets();
  if (nbjets > 0) {
    bjet1 = event->BJets("CSV")->at(0)->Pt();
    disc1 = event->BJets("CSV")->at(0)->BJetDisc("CSV");
    bjetHighestDisc = event->BJetsBDiscOrdered("CSV")->at(0)->Pt();
    discH = event->BJetsBDiscOrdered("CSV")->at(0)->BJetDisc("CSV");
  }
  else{
    bjet1 = -1.;
    disc1 = -1.;
    bjetHighestDisc = -1.;
    discH = -1.;
  }
   
  lPt = event->FirstLepton()->Pt();
  lEta = event->FirstLepton()->Eta();
  lRelIso = event->FirstLepton()->RelIso();

  
  isoTrack = !isoTrackVeto::IsoTrackVetoV4( event->FirstLepton(), event->Tracks());
  tauVeto = event->Taus("Veto")->size() == 0;
  
  rawmet = event->RawMET()->Pt();
  typeImet = event->TypeIMET()->Pt();
  phiCorrMet = event->TypeIPhiCorrMET()->Pt();
  
  ht = event->HT();
  meff = event->Meff();
  y = event->Y();

  mt = event->MT();
  mlb1 = event->Mlb1();
  if (njets > 2){
    mlb = event->Mlb();
    m3b = event->M3b();
  }
  else{
    mlb = -1;
    m3b = -1.;
  }
  mt2w = event->MT2W();
  hadChi2 = event->HadChi2();
  topness = event->Topness();
  
  dphimin = event->DeltaPhiMinj12m();
  drlb = event->DeltaRlb1();
  
  mY = event->Info()->mY;
  mLsp = event->Info()->mLsp;
  x = event->Info()->x;
  
  T2ttL = event->Info()->T2ttL;
  T2ttR = event->Info()->T2ttR;
  
  T2bWLL = event->Info()->T2bWLL;
  T2bWLS = event->Info()->T2bWLS;
  T2bWLR = event->Info()->T2bWLR;
  
  T2bWSL = event->Info()->T2bWSL;
  T2bWSS = event->Info()->T2bWSS;
  T2bWSR = event->Info()->T2bWSR;
  
  T2bWRL = event->Info()->T2bWRL;
  T2bWRS = event->Info()->T2bWRS;
  T2bWRR = event->Info()->T2bWRR;
  
  tree->Fill();
}

