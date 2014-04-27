#include "SkimmingTree.h"

#include "TObjArray.h"
#include "TObjString.h"

SkimmingTree::SkimmingTree(): SubTree(){
  home=0;
  tree = new TTree("skimmingTree", "Skimming subTree");

  this->Reset();
  this->SetBranches();
}

SkimmingTree::SkimmingTree(TDirectory* indir, TString newdir): SubTree( indir, newdir){
  TDirectory* keep = gDirectory->GetDirectory("");
  if( home!= 0)
    home->cd();

  tree = new TTree("skimmingTree", "Skimming subTree");
  
  this->Reset();
  this->SetBranches();
  
  keep->cd();
}

void SkimmingTree::Reset(){
  info              = 0;
  tracks            = 0;
  muons             = 0;
  electrons         = 0;
  taus              = 0;
  jets              = 0;
  genJets           = 0;
  unclusteredEnergy = 0;
  rawMET            = 0;
  typeIMET          = 0;
  typeIPhiCorrMET   = 0;
  mvaMET            = 0;
  caloMET           = 0;
}

void SkimmingTree::SetBranches(){
  //tree->Branch(            "Info",             "EventInfo",            &info, );

  tree->Branch(            "Info",             "EventInfo",            &info, 32000, 0);
  tree->Branch(          "Tracks", "std::vector<Particle>",          &tracks, 32000, 0);
  tree->Branch(           "Muons",     "std::vector<Muon>",           &muons, 32000, 0);
  tree->Branch(       "Electrons", "std::vector<Electron>",       &electrons, 32000, 0);
  tree->Branch(            "Taus",      "std::vector<Tau>",            &taus, 32000, 0);
  tree->Branch(            "Jets",      "std::vector<Jet>",            &jets, 32000, 0);
  tree->Branch(         "GenJets",   "std::vector<GenJet>",         &genJets, 32000, 0);
  tree->Branch(          "RawMET",              "LorentzM",          &rawMET, 32000, 0);
  tree->Branch(        "TypeIMET",              "LorentzM",        &typeIMET, 32000, 0);
  tree->Branch( "TypeIPhiCorrMET",              "LorentzM", &typeIPhiCorrMET, 32000, 0);
  tree->Branch(          "MvaMET",              "LorentzM",          &mvaMET, 32000, 0);
  tree->Branch(         "CaloMET",              "LorentzM",         &caloMET, 32000, 0);
}  

void SkimmingTree::Fill(Event* event, EasyChain* chain){
  tree->Fill();
}
