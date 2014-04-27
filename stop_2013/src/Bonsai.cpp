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

  tree= new TTree( "bonsai", "Bonsai subTree");
  
  bdisc = new vector<Float_t>();
  bdiscNoReshape = new vector<Float_t>();
  this->Reset();
  this->SetBranches();
  
  keep->cd();
}

void Bonsai::Reset()
{
  sample = 0.;
  LepFromTop = 0.;
  Charginos  = 0.;

  xs = 0.;
  NEvents = 0.;
  FE = 0.;
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
  bdisc->clear();
  bdiscNoReshape->clear();
  bjet1 = 0.;
  bjetHighestDisc = 0.;
  discH = 0.;
  
  lPt = 0.;
  lEta = 0.;
  lRelIso = 0.;
  
  isoTrack = 0.;
  tauVeto = 0.;
  
  rawmet = 0.;
  typeImet = 0.;
  phiCorrMet = 0.;
  
  ht = 0.;
  ht3 = 0.;
  ht4 = 0.;
  ht5 = 0.;
  htRatio = 0.;
  meff = 0.;
  y = 0.;

  mt = 0.;
  mlb1 = 0.;
  mlb = 0.;
  m3b = 0.;
  m3 = 0.;
  centrality = 0.;
  centralityNoLep = 0.;
  mt2w = 0.;
  hadChi2 = 0.;
  topness = 0.;

  dphimin = 0.;
  drlb1 = 0.;
  drlbmin = 0.;

  mStop = 0.;
  mLSP = 0.;
  
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

  pdgIdLep1 = 0;
  pdgIdLep2 = 0;

  kinRegion = false;
  searchRegionPre = false;
  searchRegionPost = false;
  CR1 = false;
  CR4 = false;
  CR5 = false;
}


void Bonsai::SetBranches()
{
  tree->Branch("sample",&sample,"sample/F");
  tree->Branch("LepFromTop",&LepFromTop,"LepFromTop/F");
  tree->Branch("Charginos",&Charginos,"Charginos/F");

  tree->Branch("xs",&xs,"xs/F");
  tree->Branch("NEvents",&NEvents,"NEvents/F");
  tree->Branch("FE",&FE,"FE/F");
  tree->Branch("GlobalWeight",&GlobalWeight,"GlobalWeight/F");

  tree->Branch("TriggerWeight",&TriggerWeight,"TriggerWeight/F");

  tree->Branch("PUInter",&PUInter,"PUInter/F");
  tree->Branch("PUWeight",&PUWeight,"PUWeight/F");
  tree->Branch("PUWeight_up",&PUWeight_up,"PUWeight_up/F");
  tree->Branch("PUWeight_down",&PUWeight_down,"PUWeight_down/F");
  tree->Branch("NPV",&NPV,"NPV/F");
  tree->Branch("NgoodPV",&NgoodPV,"NgoodPV/F");

  tree->Branch("isrWeight",&isrWeight,"isrWeight/F");
  tree->Branch("topPtWeight",&topPtWeight,"topPtWeight/F");

  tree->Branch("evt",&evt,"evt/F");
  tree->Branch("run",&run,"run/F");
  tree->Branch("lumi",&lumi,"lumi/F");
  
  tree->Branch("njets",&njets,"njets/F");
  tree->Branch("jet1", &jet1, "jet1/F");
  tree->Branch("jet2", &jet2, "jet2/F");
  tree->Branch("jet3", &jet3, "jet3/F");
  tree->Branch("jet4", &jet4, "jet4/F");
  
  tree->Branch("nbjets",&nbjets,"nbjets/F");
  tree->Branch("bdisc", "std::vector<Float_t>", &bdisc);
  tree->Branch("bdiscNoReshape", "std::vector<Float_t>", &bdiscNoReshape);
  tree->Branch("bjet1", &bjet1, "bjet1/F");
  tree->Branch("bjetHighestDisc", &bjetHighestDisc, "bjetHighestDisc/F");
  tree->Branch("discH", &discH, "discH/F");
  
  tree->Branch("lPt",&lPt,"lPt/F");
  tree->Branch("lEta",&lEta,"lEta/F");
  tree->Branch("lRelIso",&lRelIso,"lRelIso/F");
  
  tree->Branch("isoTrack",&isoTrack,"isoTrack/F");
  tree->Branch("tauVeto", &tauVeto, "tauVeto/F");

  tree->Branch("rawmet",&rawmet,"rawmet/F");
  tree->Branch("typeImet", &typeImet, "typeImet/F");
  tree->Branch("phiCorrMet", &phiCorrMet, "phiCorrMet/F");
  
  tree->Branch("ht",&ht,"ht/F");
  tree->Branch("ht3",&ht3,"ht3/F");
  tree->Branch("ht4",&ht4,"ht4/F");
  tree->Branch("ht5",&ht5,"ht5/F");
  tree->Branch("htRatio",&htRatio,"htRatio/F");
  tree->Branch("meff",&meff,"meff/F");
  tree->Branch("y",&y,"y/F");

  tree->Branch("mt",&mt,"mt/F");
  tree->Branch("mlb1",&mlb1,"mlb1/F");
  tree->Branch("mlb",&mlb,"mlb/F");
  tree->Branch("m3b",&m3b,"m3b/F");
  tree->Branch("m3",&m3,"m3/F");
  tree->Branch("centrality",&centrality,"centrality/F");
  tree->Branch("centralityNoLep",&centralityNoLep,"centralityNoLep/F");
  tree->Branch("mt2w",&mt2w,"mt2w/F");
  tree->Branch("hadChi2",&hadChi2,"hadChi2/F");
  tree->Branch("topness",&topness,"topness/F");

  tree->Branch("dphimin",&dphimin,"dphimin/F");
  tree->Branch("drlb1",&drlb1,"drlb1/F"); 
  tree->Branch("drlbmin",&drlbmin,"drlbmin/F"); 
 
  tree->Branch("mStop",&mStop,"mStop/F");
  tree->Branch("mLSP",&mLSP,"mLSP/F");

  tree->Branch("T2ttL",&T2ttL,"T2ttL/F");
  tree->Branch("T2ttR",&T2ttR,"T2ttR/F");

  tree->Branch("T2bWLL",&T2bWLL,"T2bWLL/F");
  tree->Branch("T2bWLS",&T2bWLS,"T2bWLS/F");
  tree->Branch("T2bWLR",&T2bWLR,"T2bWLR/F");

  tree->Branch("T2bWSL",&T2bWSL,"T2bWSL/F");
  tree->Branch("T2bWSS",&T2bWSS,"T2bWSS/F");
  tree->Branch("T2bWSR",&T2bWSR,"T2bWSR/F");

  tree->Branch("T2bWRL",&T2bWRL,"T2bWRL/F");
  tree->Branch("T2bWRS",&T2bWRS,"T2bWRS/F");
  tree->Branch("T2bWRR",&T2bWRR,"T2bWRR/F");

  tree->Branch("pdgIdLep1",&pdgIdLep1,"pdgIdLep1/I");
  tree->Branch("pdgIdLep2",&pdgIdLep2,"pdgIdLep2/I");

  tree->Branch("kinRegion",&kinRegion,"kinRegion/B");
  tree->Branch("searchRegionPre",&searchRegionPre,"searchRegionPre/B");
  tree->Branch("searchRegionPost",&searchRegionPost,"searchRegionPost/B");
  tree->Branch("CR1",&CR1,"CR1/B");
  tree->Branch("CR4",&CR4,"CR4/B");
  tree->Branch("CR5",&CR5,"CR5/B");
}  

void Bonsai::Fill( Event* event, EasyChain* chain)
{
  this->Reset();
  
  sample = 0.;
  LepFromTop = event->Info()->LepFromTop;
  Charginos  = event->Info()->Charginos;

  xs = event->Info()->xs;
  NEvents = event->Info()->NEvents;
  FE = event->Info()->FE;
  GlobalWeight = event->GlobalWeight();

  TriggerWeight = event->TriggerEfficiency();

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

  for ( int ijet = 0; ijet < njets; ijet++){
    if( !event->Info()->isData){
      bdisc->push_back(event->Jets("Selected")->at(ijet)->BJetDisc("CSV_Reshape"));  
      bdiscNoReshape->push_back(event->Jets("Selected")->at(ijet)->BJetDisc("CSV_NoReshape"));
    }
    else {
      bdisc->push_back(event->Jets("Selected")->at(ijet)->BJetDisc("CSV"));  
      bdiscNoReshape->push_back(event->Jets("Selected")->at(ijet)->BJetDisc("CSV"));
    }
  }

  nbjets = event->nBJets();
  if (nbjets > 0) {
    bjet1 = event->BJets("CSV")->at(0)->Pt();
    bjetHighestDisc = event->BJetsBDiscOrdered("CSV")->at(0)->Pt();
    discH = event->BJetsBDiscOrdered("CSV")->at(0)->BJetDisc("CSV");
  }
  else{
    bjet1 = -1.;
    bjetHighestDisc = -1.;
    discH = -1.;
  }

  lPt = event->FirstLepton()->Pt();
  lEta = event->FirstLepton()->Eta();
  lRelIso = event->FirstLepton()->RelIso();

  isoTrack = isoTrackVeto::IsoTrackVetoV4( event->FirstLepton(), event->Tracks());
  tauVeto = event->Taus("Veto")->size();

  rawmet = event->RawMET()->Pt();
  typeImet = event->TypeIMET()->Pt();
  phiCorrMet = event->TypeIPhiCorrMET()->Pt();
  
  ht = event->HT();
  ht3 = event->HT3();
  ht4 = event->HT4();
  ht5 = event->HT5();
  htRatio = event->HTratio();
  meff = event->Meff();
  y = event->Y();

  mt = event->MT();
  mlb1 = event->Mlb1();
  if (njets > 2){
    mlb = event->Mlb();
    m3b = event->M3b();
    m3 = event->M3();
  }
  else{
    mlb = -1;
    m3b = -1.;
    m3 = -1;
  }
  centrality = event->Centrality();
  centralityNoLep = event->CentralityNoLep();

  mt2w = event->MT2W();
  //hadChi2 = event->HadChi2();
  //topness = event->Topness();

  dphimin = event->DeltaPhiMinj12m();
  drlb1 = event->DeltaRlb1();
  drlbmin = event->DeltaRlbmin();
  
  mStop = event->Info()->mStop;
  mLSP = event->Info()->mLSP;
  
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

  pdgIdLep1 = 0;
  if(event->FirstLepton() != 0)
    pdgIdLep1 = event->FirstLepton()->PdgID();

  pdgIdLep2 = 0;
  if(event->SecondLepton() != 0)
    pdgIdLep2 = event->SecondLepton()->PdgID();
  
  bool OK = false;  
  //Showing kinematic variables
  OK = event->Muons("Selected")->size() + event->Electrons("Selected")->size() == 1;
  OK = OK && njets >= 3;
  OK = OK && nbjets > 0;
  OK = OK && phiCorrMet > 80.;
  kinRegion = OK;

  // Search Region Pre IsoTrack Veto
  OK = event->Muons("Selected")->size() + event->Electrons("Selected")->size() > 0.001;
  OK = OK && njets >= 3;
  OK = OK && nbjets > 0;
  OK = OK && phiCorrMet > 80.;
  searchRegionPre = OK;
  
  // Search Region Post IsoTrack Veto
  OK = event->Muons("Selected")->size() + event->Electrons("Selected")->size() == 1;
  OK = OK && !(isoTrack > 0.);
  OK = OK && !(tauVeto > 0.);
  OK = OK && njets >= 3;
  OK = OK && nbjets > 0;
  OK = OK && phiCorrMet > 80.;
  searchRegionPost = OK;

  // CR1
  OK = event->Muons("Selected")->size() + event->Electrons("Selected")->size() == 1;
  OK = OK && !(isoTrack > 0.);
  OK = OK && !(tauVeto > 0.);
  OK = OK && njets >= 3;
  OK = OK && nbjets == 0;
  OK = OK && phiCorrMet > 80.;
  CR1 = OK;

  // CR4
  OK = event->Muons("Selected")->size() + event->Electrons("Selected")->size() == 2;
  if (OK){
    OK = event->FirstLepton()->Charge() * event->SecondLepton()->Charge() < 0;
    OK = OK && fabs((event->FirstLepton()->P4() + event->FirstLepton()->P4()).M() -91) > 15;
    OK = OK && nbjets > 0;
    OK = OK && phiCorrMet > 50.;
  }
  CR4 = OK;
  
  // CR5
  OK = event->Muons("Selected")->size() + event->Electrons("Selected")->size() > 0;
  OK = OK && ( isoTrack > 0. || tauVeto > 0.);
  OK = OK && njets >= 3;
  OK = OK && nbjets > 0;
  OK = OK && phiCorrMet > 80.;
  CR5 = OK;
  
  tree->Fill();
}

