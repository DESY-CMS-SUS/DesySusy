#include "BTagReweight.h"
#include "Jet.h"

Systematics::BTagReweight::BTagReweight( TString variation_){
  InEvent = 0;
  OutEvent = Event();

  variation = variation_;
  variation.ToLower();
  
  BTagReshape_upBC      = new BTagShapeInterface( "data/csvdiscr.root", 1.0, 0.0);
  BTagReshape_downBC    = new BTagShapeInterface( "data/csvdiscr.root",-1.0, 0.0);
  BTagReshape_upLight   = new BTagShapeInterface( "data/csvdiscr.root", 0.0, 1.0);
  BTagReshape_downLight = new BTagShapeInterface( "data/csvdiscr.root", 0.0,-1.0);
}


void Systematics::BTagReweight::Reset(){
  InEvent = 0;
  OutEvent = Event();
}

void Systematics::BTagReweight::Eval( Event& event_){
  InEvent = &event_;
  OutEvent = Event(event_);
  
  if ( event_.Info()->isData)
    return;
  
  BTagShapeInterface* reshape;  
  if      ( variation == "upbc")      reshape = BTagReshape_upBC;
  else if ( variation == "downbc")    reshape = BTagReshape_downBC;
  else if ( variation == "uplight")   reshape = BTagReshape_upLight;
  else if ( variation == "downlight") reshape = BTagReshape_downLight;
  else {
    std::cout<<"BTagReweight: invalid variation type."<<std::endl;
    return;
  }
  
  vector<Jet> jets = event_.CopyJets();
  double BDisc_old, BDisc_new;
  for( unsigned int ijet = 0; ijet < jets.size(); ijet++)
    {
      BDisc_old = jets.at(ijet).BJetDisc("CSV_NoReshape");
      BDisc_new = reshape->reshape( jets.at(ijet).Eta(),
				    jets.at(ijet).Pt(),
				    BDisc_old,
				    jets.at(ijet).PartonFlavor());
      jets.at(ijet).SetBJetDisc("CSV", BDisc_new);
    }
  OutEvent.SetJets(jets);
}

void Systematics::BTagReweight::Eval( Event& event_, TString variation_){
  variation = variation_;
  variation.ToLower();
  this->Eval(event_);
}

Event* Systematics::BTagReweight::SysEvent(){
  return &OutEvent;
}
