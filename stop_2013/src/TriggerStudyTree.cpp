#include "TriggerStudyTree.h"

#include "TObjArray.h"
#include "TObjString.h"

using namespace std;

TriggerStudyTree::TriggerStudyTree(): SubTree(){
  home=0;
  tree= new TTree("triggerStudyTree", "TriggerStudy subTree");

  this->Reset();
  this->SetBranches();
}

TriggerStudyTree::TriggerStudyTree(TDirectory* indir, TString newdir): SubTree( indir, newdir){
  TDirectory* keep = gDirectory->GetDirectory("");
  if( home!= 0)
    home->cd();

  tree= new TTree("triggerStudyTree", "TriggerStudy subTree");
  
  this->Reset();
  this->SetBranches();
  
  keep->cd();
}

void TriggerStudyTree::SetBranches(){
  tree->Branch("Event",&eventNum,"eventNum/I");
  tree->Branch("Run",&run,"run/I");
  tree->Branch("PUInter",&PUInter);
  tree->Branch("PUWeight",&PUWeight,"PUWeight/D");
  tree->Branch("Weight",&weight,"weight/D");

  tree->Branch("FoundDiMuon",&foundDiMuon,"foundDiMuon/I");
  tree->Branch("muonP4", &muonP4);
  tree->Branch("muonCharge",&muonCharge);
  tree->Branch("muonIsSelected",&muonIsSelected);

  tree->Branch("FoundDiElectron",&foundDiElectron,"foundDiElectron/I");
  tree->Branch("electronP4", &electronP4);
  tree->Branch("electronCharge",&electronCharge);
  tree->Branch("electronIsSelected",&electronIsSelected);
  
  tree->Branch("jetP4", &jetP4);
  tree->Branch("jetIsSelected",&jetIsSelected);

  tree->Branch("HT",&HT,"HT/D");
  tree->Branch("MET",&MET,"MET/D");

  tree->Branch("DESYtriggerElMatchedTriggerFilter",&DESYtriggerElMatchedTriggerFilter);
  tree->Branch("DESYtriggerMuMatchedTriggerFilter",&DESYtriggerMuMatchedTriggerFilter);  
  tree->Branch("DESYtriggerNameMap",&DESYtriggerNameMap);
  tree->Branch("prescaled",&prescaled);
  tree->Branch("triggered",&triggered);
}


void TriggerStudyTree::Fill( Event* event, EasyChain* chain) {
  //initialize the variables values; e.g. clear vectors
  Reset();
  
  bool foundDiMuon=false;
  bool foundDiElectron=false;

  const Muon* imu = 0;
  const Muon* jmu = 0;
  if( event->Muons("Selected")->size()<2)
    foundDiMuon=false;
  else {
    for(Int_t i=0, N=event->Muons()->size(); i<N-1; ++i){
      imu = event->Muons()->at(i);
      if( !imu->Is("Selected")) continue;
      for(Int_t j=i+1; j<N; ++j){
	jmu = event->Muons()->at(j);
	if( imu->Charge() * jmu->Charge() > 0) continue;
	double diLepMass = ( imu->P4() + jmu->P4()).M();
	if(diLepMass > 60. && diLepMass < 120.){
	  foundDiMuon=true;
	  break;
	}
      }
      if(foundDiMuon) break;
    }
  }

  const Electron* iel = 0;
  const Electron* jel = 0;
  if( event->Electrons("Selected")->size()<2)
    foundDiElectron=false;
  else {
    for(Int_t i=0, N=event->Electrons()->size(); i<N-1; ++i){
      iel = event->Electrons()->at(i);
      if( !iel->Is("Selected")) continue;
      for(Int_t j=i+1; j<N; ++j){
	jel = event->Electrons()->at(j);
	if( iel->Charge() * jel->Charge() > 0) continue;
	double diLepMass = ( iel->P4() + jel->P4()).M();
	if(diLepMass > 60. && diLepMass < 120.){
	  foundDiElectron=true;
	  break;
	}
      }
      if(foundDiElectron) break;
    }
  }

  if( foundDiMuon || foundDiElectron){
    eventNum = event->Info()->Event;
    run      = event->Info()->Run;
    PUInter  = event->Info()->PUInter;
    PUWeight = event->Info()->PUWeight;
    weight   = event->GlobalWeight() * PUWeight;

    for(Int_t i=0,N=event->Muons()->size(); i<N; ++i){
      imu = event->Muons()->at(i);
      muonP4.push_back( imu->P4());
      muonCharge.push_back( (int)imu->Charge());
      muonIsSelected.push_back( (int)imu->Is("Selected"));
    }
    for(Int_t i=0,N=event->Electrons()->size(); i<N; ++i){
      iel = event->Electrons()->at(i);
      electronP4.push_back( iel->P4());
      electronCharge.push_back( (int)iel->Charge());
      electronIsSelected.push_back( (int)iel->Is("Selected"));
    }
    
    for(Int_t i=0,N=event->Jets()->size(); i<N; ++i){
      jetP4.push_back(event->Jets()->at(i)->P4());
      jetIsSelected.push_back(event->Jets()->at(i)->Is("Selected"));
    }

    HT = event->HT();
    MET = event->TypeIPhiCorrMET()->Et();

    //This info is not stored in the current NTuples.
    //A new NTupling campain is needed to make triggerStudies
    //if( foundDiElectron) DESYtriggerElMatchedTriggerFilter = 
    //  chain->Get(&DESYtriggerElMatchedTriggerFilter, "DESYtriggerElMatchedTriggerFilter");
    //if( foundDiMuon) DESYtriggerMuMatchedTriggerFilter = 
    //  chain->Get(&DESYtriggerMuMatchedTriggerFilter, "DESYtriggerMuMatchedTriggerFilter");
    DESYtriggerNameMap                = chain->Get(&DESYtriggerNameMap, "DESYtriggerNameMap");
    prescaled                         = chain->Get(&prescaled, "prescaled");
    triggered                         = chain->Get(&triggered, "triggered");
    
    tree->Fill();
  }
};


void TriggerStudyTree::Reset(){
  eventNum = 0;
  run = 0;
  PUInter = 0;
  PUWeight = 0.;

  foundDiMuon = 0;
  muonP4.clear();
  muonCharge.clear();
  muonIsSelected.clear();

  foundDiElectron = 0;
  electronP4.clear();
  electronCharge.clear();
  electronIsSelected.clear();

  jetP4.clear();
  jetIsSelected.clear();

  HT = 0.;
  MET = 0.;

  DESYtriggerElMatchedTriggerFilter.clear();
  DESYtriggerMuMatchedTriggerFilter.clear();
  DESYtriggerNameMap.clear();
  prescaled.clear();
  triggered.clear();
};
