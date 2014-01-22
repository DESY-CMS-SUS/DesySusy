#include "NoSystematic.h"

Systematics::NoSystematic::NoSystematic(){
  InEvent = 0;

  BTagReshape_nominal = new BTagShapeInterface( "csvdiscr.root", 1.0, 0.0);
}

void Systematics::NoSystematic::Reset(){
  InEvent = 0;
  OutEvent = Event();
}

void Systematics::NoSystematic::Eval( Event& event_){
  InEvent=&event_;  
  OutEvent = Event(event_);

  if ( event_.Info()->isData)
    return;

  vector<Jet> jets = event_.CopyJets();
  for( unsigned int ijet = 0; ijet < jets.size(); ijet++)
    {
      double BDisc_old = jets.at(ijet).BJetDisc("CSV");
      double BDisc_new = BTagReshape_nominal->reshape( jets.at(ijet).Eta(),
						       jets.at(ijet).Pt(),
						       BDisc_old,
						       jets.at(ijet).PartonFlavor());
      jets.at(ijet).SetBJetDisc("CSV", BDisc_new);
    }
  OutEvent.SetJets(jets);
}

Event* Systematics::NoSystematic::SysEvent(){
  return &OutEvent;
}

