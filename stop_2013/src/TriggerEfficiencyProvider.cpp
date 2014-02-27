#include "TriggerEfficiencyProvider.h"

#include <cmath> 
#include <math.h> 

#include "TF1.h"

double trigger::GetTriggerEfficiency( Event& event){
  if ( event.Info()->isData)
    return 1.;

  int nMu = event.Muons("Selected")->size();
  int nEle =  event.Electrons("Selected")->size();

  double triggerWeight = 1.;
  
  if (event.FirstLepton() == 0){
    triggerWeight = 0.;
  }
  else if (abs(event.FirstLepton()->PdgID()) == 11){
    triggerWeight = Ele27wp80( event.FirstLepton()->Pt(), 
			       event.FirstLepton()->Eta());
    if (event.SecondLepton() != 0)
      if ( abs(event.SecondLepton()->PdgID()) == 11){
	triggerWeight = triggerWeight + (1 - triggerWeight) * Ele27wp80( event.SecondLepton()->Pt(), 
									 event.SecondLepton()->Eta());
      }
  }
  else if (abs(event.FirstLepton()->PdgID()) == 13){
    if (event.FirstLepton()->Pt() > 25.){
      triggerWeight = IsoMu24( event.FirstLepton()->Pt(), 
			       event.FirstLepton()->Eta());
      if (event.SecondLepton() != 0)
	if (abs(event.SecondLepton()->PdgID()) == 13)
	  triggerWeight = triggerWeight + (1 - triggerWeight) * IsoMu24( event.SecondLepton()->Pt(), 
									 event.SecondLepton()->Eta());	  
    }
    else if (event.FirstLepton()->Pt() > 20.){
      if ( event.Jets("Selected")->size() < 4){
	triggerWeight = 0.;	
      }	
      else {
	triggerWeight = XTrigger_MuonLeg( event.FirstLepton()->Pt(), 
					  event.FirstLepton()->Eta());
	if (event.SecondLepton() != 0){
	  if (abs(event.SecondLepton()->PdgID()) == 13)
	    triggerWeight = triggerWeight + (1 - triggerWeight) * XTrigger_MuonLeg( event.SecondLepton()->Pt(), 
										    event.SecondLepton()->Eta());
	}
	triggerWeight *= XTrigger_HadLeg( event.Jets("Selected")->at(3)->Pt());
      }
    }
  }
    
  return triggerWeight;
}

double trigger::Ele27wp80( double pt, double eta){
  if ( fabs(eta)<1.5) {      
    if ( pt>=20 && pt<22 ) return 0.00;
    if ( pt>=22 && pt<24 ) return 0.00;
    if ( pt>=24 && pt<26 ) return 0.00;
    if ( pt>=26 && pt<28 ) return 0.08;
    if ( pt>=28 && pt<30 ) return 0.61;
    if ( pt>=30 && pt<32 ) return 0.86;
    if ( pt>=32 && pt<34 ) return 0.88;
    if ( pt>=34 && pt<36 ) return 0.90;
    if ( pt>=36 && pt<38 ) return 0.91;
    if ( pt>=38 && pt<40 ) return 0.92;      
    if ( pt>=40 && pt<50 ) return 0.94;
    if ( pt>=50 && pt<60 ) return 0.95;
    if ( pt>=60 && pt<80 ) return 0.96;
    if ( pt>=80 && pt<100 ) return 0.96;
    if ( pt>=100 && pt<150 ) return 0.96;
    if ( pt>=150 && pt<200 ) return 0.97;
    if ( pt>=200 ) return 0.97;
  } else if ( fabs(eta)>=1.5 && fabs(eta)<2.5) {
    if ( pt>=20 && pt<22 ) return 0.00;
    if ( pt>=22 && pt<24 ) return 0.00;
    if ( pt>=24 && pt<26 ) return 0.02;
    if ( pt>=26 && pt<28 ) return 0.18;
    if ( pt>=28 && pt<30 ) return 0.50;
    if ( pt>=30 && pt<32 ) return 0.63;
    if ( pt>=32 && pt<34 ) return 0.68;
    if ( pt>=34 && pt<36 ) return 0.70;
    if ( pt>=36 && pt<38 ) return 0.72;
    if ( pt>=38 && pt<40 ) return 0.74;
    if ( pt>=40 && pt<50 ) return 0.76;
    if ( pt>=50 && pt<60 ) return 0.77;
    if ( pt>=60 && pt<80 ) return 0.78;
    if ( pt>=80 && pt<100 ) return 0.80;
    if ( pt>=100 && pt<150 ) return 0.79;
    if ( pt>=150 && pt<200 ) return 0.76;
    if ( pt>=200 ) return 0.81;
  }
  return 1.0;
}

double trigger::IsoMu24( double pt, double eta){
  if ( fabs(eta)<0.8 ) {
    if (pt>=20 && pt<22)  return  0.85;     
    if (pt>=22 && pt<24)  return  0.85;      
    if (pt>=24 && pt<26)  return  0.87;
    if (pt>=26 && pt<28)  return  0.90;
    if (pt>=28 && pt<30)  return  0.91;
    if (pt>=30 && pt<32)  return  0.91;
    if (pt>=32 && pt<34)  return  0.92;
    if (pt>=34 && pt<36)  return  0.93;
    if (pt>=36 && pt<38)  return  0.93;
    if (pt>=38 && pt<40)  return  0.93;
    if (pt>=40 && pt<50)  return  0.94;
    if (pt>=50 && pt<60)  return  0.95;
    if (pt>=60 && pt<80)  return  0.95;
    if (pt>=80 && pt<100) return 0.94;
    if (pt>=100 && pt<150) return 0.94;
    if (pt>=150 && pt<200) return 0.93;
    if (pt>=200) return 0.92;
  } else if ( fabs(eta)>=0.8 && fabs(eta)<1.5 ) {
    if (pt>=20 && pt<22)  return  0.80;
    if (pt>=22 && pt<24)  return  0.80;
    if (pt>=24 && pt<26)  return  0.78;
    if (pt>=26 && pt<28)  return  0.81;
    if (pt>=28 && pt<30)  return  0.81;
    if (pt>=30 && pt<32)  return  0.81;
    if (pt>=32 && pt<34)  return  0.82;
    if (pt>=34 && pt<36)  return  0.82;
    if (pt>=36 && pt<38)  return  0.83;
    if (pt>=38 && pt<40)  return  0.83;
    if (pt>=40 && pt<50)  return  0.84;
    if (pt>=50 && pt<60)  return  0.84;
    if (pt>=60 && pt<80)  return  0.84;
    if (pt>=80 && pt<100) return 0.84;
    if (pt>=100 && pt<150) return 0.84;
    if (pt>=150 && pt<200) return 0.84;
    if (pt>=200) return 0.82;
  } else if ( fabs(eta)>=1.5 && fabs(eta)<2.1 ) {
    if (pt>=20 && pt<22)  return  0.80;
    if (pt>=22 && pt<24)  return  0.80;
    if (pt>=24 && pt<26)  return  0.80;
    if (pt>=26 && pt<28)  return  0.78;
    if (pt>=28 && pt<30)  return  0.79;
    if (pt>=30 && pt<32)  return  0.80;
    if (pt>=32 && pt<34)  return  0.80;
    if (pt>=34 && pt<36)  return  0.81;
    if (pt>=36 && pt<38)  return  0.81;
    if (pt>=38 && pt<40)  return  0.82;
    if (pt>=40 && pt<50)  return  0.82;
    if (pt>=50 && pt<60)  return  0.83;
    if (pt>=60 && pt<80)  return  0.83;
    if (pt>=80 && pt<100) return 0.83;
    if (pt>=100 && pt<150) return 0.83;
    if (pt>=150 && pt<200) return 0.82;
    if (pt>=200) return 0.82;
  }
  return 1.;
}

double trigger::XTrigger_MuonLeg( double pt, double eta){
  if ( fabs(eta)<0.8 ) {
    if (pt>=20 && pt<22)  return  0.94;     
    if (pt>=22 && pt<24)  return  0.97;      
    if (pt>=24 && pt<26)  return  0.96;
  } 
  else if ( fabs(eta)>=0.8 && fabs(eta)<1.5 ) {
    if (pt>=20 && pt<22)  return  0.87;
    if (pt>=22 && pt<24)  return  0.90;
    if (pt>=24 && pt<26)  return  0.91;
  }
  else if ( fabs(eta)>=1.5 && fabs(eta)<2.1 ) {
    if (pt>=20 && pt<22)  return  0.76;
    if (pt>=22 && pt<24)  return  0.83;
    if (pt>=24 && pt<26)  return  0.87;
  }

  return 1.;
}

double trigger::XTrigger_HadLeg( double pt){
  
  double PFJet30 = 1.45;
  double PFNoPUJet30_30_20 = 5.65;
  double PFNoPUJet30 = 12.39;

  double tot = PFJet30 + PFNoPUJet30_30_20 + PFNoPUJet30;

  double eff = PFJet30 * trigger::PFJet30_TurnOn(pt);
  eff += PFNoPUJet30_30_20 * trigger::PFNoPUJet20_TurnOn(pt);
  eff += PFNoPUJet30 * trigger::PFJet30_TurnOn(pt);

  eff /= tot;
    
  return eff;
}

/// Jet Turn ons PFNoPUJet30 Runs > 199631
double trigger::PFNoPUJet30_TurnOn(double pt){
  TF1 *f1 = new TF1("f1","0.6412/(0.6414 + exp(9.173-x*0.3877))");
  return f1->Eval(pt);
}

/// Jet Turn ons PFNoPUJet20 194256 < Runs < 199631
double trigger::PFNoPUJet20_TurnOn(double pt){
  TF1 *f1 = new TF1("f1","1/(1 + exp(1.393-x*0.2067))");
  return f1->Eval(pt);
}
/// Jet Turn ons PFJet30 Runs < 194231
double trigger::PFJet30_TurnOn(double pt){
  TF1 *f1 = new TF1("f1","1/(1 + exp(10.95-x*0.4503))");
  return f1->Eval(pt);
}
