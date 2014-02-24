#include "Trigger_Stop2012.h"

const std::vector<const char*>* Trigger_Stop2012::TriggerNames( const string& sample, 
								const int& run, 
								const Particle* lep)
{
  triggerNames.clear();

  if (lep == 0)
    return &triggerNames;

  if (abs(lep->PdgID())== 11){
    if (sample.compare("SingleElectron") == 0){
      triggerNames.push_back("HLT_Ele27_WP80");
    }
  }
  else if (abs(lep->PdgID())== 13){
    if (lep->pt() > 26){
      if (sample.compare("SingleMu") == 0){
	triggerNames.push_back("HLT_IsoMu24");
	triggerNames.push_back("HLT_IsoMu24_eta2p1");
      }
    }
    else if (lep->pt() > 20){
      if (sample.compare("MuHad") == 0){
	if (run <= 194231)
	  triggerNames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFJet30");	
      }
      if (sample.compare("SingleMu") == 0){
	if (run > 199631)
	  triggerNames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30");
	else if (run > 194231)
	  triggerNames.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20");
      }
    }
  }

  return &triggerNames;
};

bool Trigger_Stop2012::Fired(EasyChain* tree, const string& sample, const int& run, const Particle* lep){
 
  extern bool pcp;
  this->TriggerNames( sample, run, lep);
 
  map<string,int>&    HLTprescaled = tree->Get(&HLTprescaled, "prescaled");
  map<string,bool>&   HLTtrigger   = tree->Get(&HLTtrigger, "triggered");
  map<string,string>& TriggerMap   = tree->Get(&TriggerMap, "DESYtriggerNameMap");

  bool OK=false;

  for (unsigned itr=0;itr<triggerNames.size(); ++itr){
    if(pcp)cout<<"checking if the trigger "<< triggerNames.at(itr)<<endl;

    bool isTriggered = false;
    int trigPrescale = -1;
    string tname = "";

    map<string,string>::const_iterator tnameIt = TriggerMap.find(triggerNames.at(itr));
    if ( tnameIt != TriggerMap.end() ) {
      tname = tnameIt->second;
      isTriggered = HLTtrigger[tname];
      trigPrescale = HLTprescaled[tname];
    }

    if (isTriggered && trigPrescale == 1){
	OK=true;
	break;
    }
  }

  return OK;
};

