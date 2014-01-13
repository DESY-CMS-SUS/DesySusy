//=============================================
//TRIGGERS
//=============================================

#include "ConfigReader.h"
#include "SampleInformation.h"
#include <iostream>

using namespace std;
//using namespace ROOT::Math::VectorUtil;

void SetTriggers_RA4b(SampleInformation mySampleInformation, vector<const char*>& triggernames){

  triggernames.clear();

  static ConfigReader config;
  static bool isData=config.getBool("isData");

  if (mySampleInformation.GetEstimation()=="STOP-mu"){
    cout<<"SetTriggers.cpp: set STOP-mu Triggers"<<endl;
    triggernames.push_back("HLT_IsoMu24");
    triggernames.push_back("HLT_IsoMu24_eta2p1");
  }
  else if (mySampleInformation.GetEstimation()=="STOP-el"){
    cout<<"SetTriggers.cpp: set STOP-el Triggers"<<endl;
    triggernames.push_back("HLT_Ele27_WP80");
  }
  else if (mySampleInformation.GetEstimation()=="STOP-dimu"){
    cout<<"SetTriggers.cpp: set STOP-dimu Triggers"<<endl;
    triggernames.push_back("HLT_Mu17_Mu8");
  }
  else if (mySampleInformation.GetEstimation()=="STOP-diel"){
    cout<<"SetTriggers.cpp: set STOP-diel Triggers"<<endl;
    triggernames.push_back("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL");
  }
  else if (mySampleInformation.GetEstimation()=="STOP-muel"){
    cout<<"SetTriggers.cpp: set STOP-muel Triggers"<<endl;
    triggernames.push_back("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL");
    triggernames.push_back("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL");
  }
  else{
    cout<<"SetTriggers.cpp: warning - Triggers not set"<<endl;
  }
}
