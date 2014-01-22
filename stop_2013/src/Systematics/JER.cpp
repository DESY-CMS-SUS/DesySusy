#include "JER.h"
#include "Jet.h"
#include "GenJet.h"

Systematics::JER::JER( TString variation_){
  InEvent = 0;
  OutEvent = Event();

  variation = variation_;
  variation.ToLower();

  rand = TRandom3(0);
}

void Systematics::JER::Reset(){
  InEvent = 0;
  OutEvent = Event();
}

void Systematics::JER::Eval( Event& event_){
  InEvent = &event_;
  OutEvent = Event(event_);

  if ( event_.Info()->isData)
    return;

  double err_factor = 0.;
  bool gen = 0;

  if ( variation.First("gen") == 0){
    gen = 1;
    if      ( variation == "genup")      err_factor =  1.;
    else if ( variation == "gencentral") err_factor =  0.;
    else if ( variation == "gendown")    err_factor = -1.;
    else {
      std::cout<<"JER: invalid variation type."<<std::endl;
      return;
    }
  }
  else if ( variation.First("reco") == 0){
    gen = 0;
    if      ( variation == "recoup")      err_factor =  1.;
    else if ( variation == "recocentral") err_factor =  0.;
    else if ( variation == "recodown")    err_factor = -1.;
    else {
      std::cout<<"JER: invalid variation type."<<std::endl;
      return;
    }
  }
  else {
    std::cout<<"JER: invalid variation type."<<std::endl;
    return;
  }

  const vector<const Jet*>* jets = event_.Jets();
  vector<Jet> jets_rescaled;

  Jet tmpJet;
  LorentzM tmpGenJet;
  double correction;
  double dMETx = 0;
  double dMETy = 0;
  for( unsigned int ijet = 0; ijet < jets->size(); ijet++)
    {
      correction = 1.;
      tmpJet = *jets->at(ijet);

      if ( gen){
	if ( tmpJet.IsMatched()){
	  tmpGenJet = tmpJet.MatchedGenJetP4();
	  correction = this->GetCorrectionGenMethod( tmpJet.P4(), tmpGenJet, err_factor);
	}
      }
      else{
	correction = this->GetCorrectionRecoMethod( tmpJet.P4(), err_factor);
      }
      
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
  OutEvent.SetTypeIMET( typeIMET_rescaled);
  OutEvent.SetTypeIPhiCorrMET( typeIPhiCorrMET_rescaled);
}

double Systematics::JER::GetCorrectionGenMethod( const LorentzM& jet, 
						 const LorentzM& genJet, 
						 const double& err_factor){
  if ( DeltaR( jet, genJet) > 0.3)
    return 1.;
  
  double JerSF = desy_tools::getJerSF(jet.eta(), err_factor);
  double correction = genJet.pt() + JerSF * ( jet.pt() - genJet.pt());

  if ( correction > 0.)
    return correction / jet.pt();
  
  return 1.;
}

double Systematics::JER::GetCorrectionRecoMethod( const LorentzM& jet, 
						  const double& err_factor){
  double jerSF = desy_tools::getJerSF(jet.eta(), err_factor);
  if (jerSF < 1.0)
    return 1.;

  double jetRes = desy_tools::getJetResolution(jet.pt(), jet.eta());
  double sigma = jetRes * sqrt(jerSF * jerSF - 1.);
  
  return 1. + rand.Gaus(0., sigma) / jet.E();
}

void Systematics::JER::Eval( Event& event_, TString variation_){
  variation = variation_;
  variation.ToLower();
  this->Eval(event_);
}

Event* Systematics::JER::SysEvent(){
  return &OutEvent;
}
