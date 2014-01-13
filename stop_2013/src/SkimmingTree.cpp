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
  tree->Branch(            "Info",             "EventInfo",            &info);
  tree->Branch(          "Tracks", "std::vector<Particle>",          &tracks);
  tree->Branch(           "Muons",     "std::vector<Muon>",           &muons);
  tree->Branch(       "Electrons", "std::vector<Electron>",       &electrons);
  tree->Branch(            "Taus",      "std::vector<Tau>",            &taus);
  tree->Branch(            "Jets",      "std::vector<Jet>",            &jets);
  tree->Branch(         "GenJets",   "std::vector<GenJet>",         &genJets);
  tree->Branch(          "RawMET",              "LorentzM",          &rawMET);
  tree->Branch(        "TypeIMET",              "LorentzM",        &typeIMET);
  tree->Branch( "TypeIPhiCorrMET",              "LorentzM", &typeIPhiCorrMET);
  tree->Branch(          "MvaMET",              "LorentzM",          &mvaMET);
  tree->Branch(         "CaloMET",              "LorentzM",         &caloMET);
}  

void SkimmingTree::Fill(Event* event, EasyChain* chain){
  tree->Fill();
}
