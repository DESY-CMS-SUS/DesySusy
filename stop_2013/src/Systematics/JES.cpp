#include "JES.h"
#include "Jet.h"

Systematics::JES::JES( TString variation_){
  InEvent = 0;
  OutEvent = Event();

  variation = variation_;
  variation.ToLower();
}

void Systematics::JES::Reset(){
  InEvent = 0;
  OutEvent = Event();
}

void Systematics::JES::Eval( Event& event_){
  InEvent = &event_;
  OutEvent = Event(event_);

  if ( event_.Info()->isData)
    return;
  
  if ( variation != "up" && variation != "down"){
    std::cout<<"JES: invalid variation type."<<std::endl;
    return;
  }

  vector<Jet> jets = event_.CopyJets();
  vector<Jet> jets_rescaled;

  Jet tmpJet;
  double correction;
  double dMETx = 0;
  double dMETy = 0;
  for( unsigned int ijet = 0; ijet < jets.size(); ijet++)
    {
      tmpJet = jets.at(ijet);
   
      if ( variation == "up")
	correction = 1 + tmpJet.JECorrUncertainty("JES");
      else if ( variation == "down")
	correction = 1 - tmpJet.JECorrUncertainty("JES");

      tmpJet.SetJECorrFactor( "JES", correction);
      
      LorentzM P4_rescaled = desy_tools::Rescale( tmpJet.P4(), correction);

      if ( tmpJet.Is("Good")) {
	dMETx -= tmpJet.Px() - P4_rescaled.Px();
	dMETy -= tmpJet.Py() - P4_rescaled.Py();
      }

      if ( tmpJet.Is("Good") && P4_rescaled.Pt() > 10)
	tmpJet.SetID("Good", 1);
      else
	tmpJet.SetID("Good", 0);
      
      if ( tmpJet.Is("Selected") && P4_rescaled.Pt() > 30)
	tmpJet.SetID("Selected", 1);
      else
	tmpJet.SetID("Selected", 0);

      tmpJet.SetP4( P4_rescaled);
      jets_rescaled.push_back( tmpJet);
    }

  LorentzM unclusteredEnergy = *event_.UnclusteredEnergy();
  LorentzM unclusteredEnergy_rescaled = LorentzM();
  if ( variation == "up")
    unclusteredEnergy_rescaled = desy_tools::Rescale( unclusteredEnergy, 1.1);
  else if ( variation == "down")
    unclusteredEnergy_rescaled = desy_tools::Rescale( unclusteredEnergy, 0.9);

  dMETx -= unclusteredEnergy_rescaled.Px() - unclusteredEnergy.Px();
  dMETy -= unclusteredEnergy_rescaled.Py() - unclusteredEnergy.Py();

  double typeIMETx = event_.TypeIMET()->Px() + dMETx;
  double typeIMETy = event_.TypeIMET()->Py() + dMETy;

  LorentzM typeIMET_rescaled = LorentzM();
  typeIMET_rescaled.SetPxPyPzE( typeIMETx, 
				typeIMETy, 
				0, 
				sqrt( typeIMETx*typeIMETx + typeIMETy*typeIMETy));

  double typeIPhiCorrMETx = event_.TypeIPhiCorrMET()->Px() + dMETx;
  double typeIPhiCorrMETy = event_.TypeIPhiCorrMET()->Py() + dMETy;
  LorentzM typeIPhiCorrMET_rescaled = LorentzM();
  typeIPhiCorrMET_rescaled.SetPxPyPzE( typeIPhiCorrMETx, 
				       typeIPhiCorrMETy, 
				       0, 
				       sqrt( typeIPhiCorrMETx*typeIPhiCorrMETx + 
					     typeIPhiCorrMETy*typeIPhiCorrMETy));

  OutEvent.SetJets(jets_rescaled);
  OutEvent.SetUnclusteredEnergy( unclusteredEnergy_rescaled);
  OutEvent.SetTypeIMET( typeIMET_rescaled);
  OutEvent.SetTypeIPhiCorrMET( typeIPhiCorrMET_rescaled);
}

void Systematics::JES::Eval( Event& event_, TString variation_){
  variation = variation_;
  variation.ToLower();
  this->Eval(event_);
}

Event* Systematics::JES::SysEvent(){
  return &OutEvent;
}
