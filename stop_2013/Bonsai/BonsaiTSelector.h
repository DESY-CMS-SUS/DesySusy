//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Aug  1 18:41:07 2013 by ROOT version 5.32/00
// from TTree skimmingTree/the subTree
// found on file: Data.root
//////////////////////////////////////////////////////////

#ifndef BonsaiTSelector_h
#define BonsaiTSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TProofOutputFile.h>
#include <TTree.h>
#include <TSystem.h>

#include <vector>
#include <utility>

class TProofOutputFile;
class EventInfo;
class Particle;
class Muon;
class Electron;
class Tau;
class Jet;
class GenJet;
class Event;
class LorentzM;
class TTree;
class Bonsai;
namespace Systematics;
class Systematics::Systematic;
class Systematics::NoSystematic;
class Systematics::SystematicFactory;

class BonsaiTSelector : public TSelector {
public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  
  TFile* outFile;
  std::vector<Bonsai*> bonsai;

  std::vector<std::string> sysName;
  std::vector< Systematics::Systematic*> sys;

  TProofOutputFile *fProofFile;

  Event event;
  EventInfo*              info;
  std::vector< Particle>* tracks;
  std::vector< Muon>*     muons;
  std::vector< Electron>* electrons;
  std::vector< Tau>*      taus;
  std::vector< Jet>*      jets;
  std::vector< GenJet>*   genJets;
  LorentzM*               rawMET;
  LorentzM*               typeIMET;
  LorentzM*               typeIPhiCorrMET;
  LorentzM*               mvaMET;
  LorentzM*               caloMET;
  
  BonsaiTSelector(TTree * /*tree*/ =0) : fChain(0) {
    sysName.push_back("NoSystematic");
    //sysName.push_back("NoSystematic");

    sysName.push_back("PUReweight_Up");
    /*sysName.push_back("PUReweight_Down");
    
    sysName.push_back("BTagReweight_UpBC");
    sysName.push_back("BTagReweight_DownBC");
    sysName.push_back("BTagReweight_UpLight");
    sysName.push_back("BTagReweight_DownLight");*/
  }

  virtual ~BonsaiTSelector() { }
  virtual Int_t   Version() const { return 2; }
  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual Bool_t  Notify();
  virtual Bool_t  Process(Long64_t entry);
  virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate();
  virtual void    Terminate();
  
  ClassDef(BonsaiTSelector,0);
};

#endif

#ifdef BonsaiTSelector_cxx
void BonsaiTSelector::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).
  
  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fChain->SetMakeClass(0); 
  /*fChain->SetBranchStatus("*",0);

  fChain->SetBranchStatus(            "Info", 1);
  //fChain->SetBranchStatus(          "Tracks", 1);
  fChain->SetBranchStatus(           "Muons", 1);
  /*fChain->SetBranchStatus(       "Electrons", 1);
  fChain->SetBranchStatus(            "Taus", 1);
  fChain->SetBranchStatus(            "Jets", 1);
  fChain->SetBranchStatus(         "GenJets", 1);
  fChain->SetBranchStatus(          "RawMET", 1);
  fChain->SetBranchStatus(        "TypeIMET", 1);
  fChain->SetBranchStatus( "TypeIPhiCorrMET", 1);
  fChain->SetBranchStatus(          "MvaMET", 1);
  fChain->SetBranchStatus(         "CaloMET", 1);*/

  fChain->SetBranchAddress(            "Info",            &info);
  fChain->SetBranchAddress(          "Tracks",          &tracks);
  fChain->SetBranchAddress(           "Muons",           &muons);
  fChain->SetBranchAddress(       "Electrons",       &electrons);
  fChain->SetBranchAddress(            "Taus",            &taus);
  fChain->SetBranchAddress(            "Jets",            &jets);
  fChain->SetBranchAddress(         "GenJets",         &genJets);
  fChain->SetBranchAddress(          "RawMET",          &rawMET);
  fChain->SetBranchAddress(        "TypeIMET",        &typeIMET);
  fChain->SetBranchAddress( "TypeIPhiCorrMET", &typeIPhiCorrMET);
  fChain->SetBranchAddress(          "MvaMET",          &mvaMET);
  fChain->SetBranchAddress(         "CaloMET",         &caloMET); 
  
}

Bool_t BonsaiTSelector::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.
  
  return kTRUE;
}

#endif // #ifdef BonsaiTSelector_cxx
