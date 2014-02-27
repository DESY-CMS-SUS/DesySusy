#include "Event.h"
#include "Tools.h"
#include "TriggerEfficiencyProvider.cpp"

#include "HadChi2/hadChi2.h"
#include "Topness/topness.h"

#include <iostream>

void Event::SetInfo ( const EventInfo& in) { info = in;}

void Event::SetTracks( const vector<Particle>& in) { 
  tracksMap.clear();     
  tracks     = in;   
  std::sort( tracks.begin(), tracks.end(), desy_tools::compare_Object_Pt< Particle, Particle>);
}
void Event::SetTracks( const vector<Particle*>& in) {  SetTracks( objFromPointers(in));}

void Event::SetMuons( const vector<Muon>& in) { 
  muonsMap.clear();     
  muons     = in;   
  std::sort( muons.begin(), muons.end(), desy_tools::compare_Object_Pt< Muon, Muon>);
  SetLeptonPair();
}
void Event::SetMuons( const vector<Muon*>& in) {  SetMuons( objFromPointers(in));}

void Event::SetElectrons( const vector<Electron>& in) { 
  electronsMap.clear();     
  electrons     = in;   
  std::sort( electrons.begin(), electrons.end(), desy_tools::compare_Object_Pt< Electron, Electron>);
  SetLeptonPair();
}
void Event::SetElectrons( const vector<Electron*>& in) {  SetElectrons( objFromPointers(in));}

void Event::SetTaus (const vector<Tau>& in) { 
  tausMap.clear();     
  taus     = in;   
  std::sort( taus.begin(), taus.end(), desy_tools::compare_Object_Pt< Tau, Tau>);
}
void Event::SetTaus( const vector<Tau*>& in) {  SetTaus( objFromPointers(in));}

void Event::SetJets (const vector<Jet>& in) { 
  jetsMap.clear();     
  jets     = in;   
  std::sort( jets.begin(), jets.end(), desy_tools::compare_Object_Pt< Jet, Jet>);
  this->MatchGenJets();
}
void Event::SetJets( const vector<Jet*>& in) {  SetJets( objFromPointers(in));}

void Event::SetGenJets (const vector<GenJet>& in) { 
  genJetsMap.clear();     
  genJets     = in;   
  std::sort( genJets.begin(), genJets.end(), desy_tools::compare_Object_Pt< GenJet, GenJet>);
  this->MatchGenJets();
}

void Event::MatchGenJets(){ desy_tools::matchGenJets( jets, genJets);}

void Event::SetGenJets( const vector<GenJet*>& in) {  SetGenJets( objFromPointers(in));}

void Event::SetUnclusteredEnergy( const LorentzM& in)         { unclusteredEnergy = in;}
void Event::SetRawMET           ( const LorentzM& in)	      { rawMET            = in;}
void Event::SetTypeIMET         ( const LorentzM& in)	      { typeIMET          = in;}
void Event::SetTypeIPhiCorrMET  ( const LorentzM& in)	      { typeIPhiCorrMET   = in;}
void Event::SetMvaMET           ( const LorentzM& in) 	      { mvaMET            = in;}
void Event::SetCaloMET          ( const LorentzM& in)	      { caloMET           = in;}
									   

const vector< const Particle* >* Event::Tracks    (const string& key) { selectObjects(    tracks,    tracksMap, key); return &tracksMap[key];}
const vector< const Muon* >*     Event::Muons     (const string& key) { selectObjects(     muons,     muonsMap, key); return &muonsMap[key];}
const vector< const Electron* >* Event::Electrons (const string& key) { selectObjects( electrons, electronsMap, key); return &electronsMap[key];}
const vector< const Tau* >*      Event::Taus      (const string& key) { selectObjects(      taus,      tausMap, key); return &tausMap[key];}
const vector< const Jet* >*      Event::Jets      (const string& key) { selectObjects(      jets,      jetsMap, key); return &jetsMap[key];}
const vector< const GenJet* >*   Event::GenJets   (const string& key) { selectObjects(   genJets,   genJetsMap, key); return &genJetsMap[key];}
const EventInfo*   Event::Info()              const { return &info;}
const LorentzM*    Event::UnclusteredEnergy() const { return &unclusteredEnergy;}
const LorentzM*    Event::RawMET()            const { return &rawMET;}
const LorentzM*    Event::TypeIMET()          const { return &typeIMET;}
const LorentzM*    Event::TypeIPhiCorrMET()   const { return &typeIPhiCorrMET;}
const LorentzM*    Event::MvaMET()            const { return &mvaMET;}
const LorentzM*    Event::CaloMET()           const { return &caloMET;}

void Event::SetLeptonPair() {
  firstLepton = 0;
  secondLepton = 0;

  const vector< const Muon*>* selectedMuons = Event::Muons("Selected");
  const vector< const Electron*>* selectedElectrons = Event::Electrons("Selected");
  
  vector<const Particle*> selectedLeptons;
  selectedLeptons.clear();

  for ( int imu = 0; imu < selectedMuons->size() && imu < 2; imu++)
    selectedLeptons.push_back(selectedMuons->at(imu));
  for ( int iele = 0; iele < selectedElectrons->size() && iele < 2; iele++)
    selectedLeptons.push_back(selectedElectrons->at(iele));

  std::sort( selectedLeptons.begin(), selectedLeptons.end(), desy_tools::compare_Pointer_Pt< const Particle, const Particle>);

  if (selectedLeptons.size() > 0)
    firstLepton = (Particle*) selectedLeptons.at(0);
  if (selectedLeptons.size() > 1)
    secondLepton = (Particle*) selectedLeptons.at(1); 
}

const Particle* Event::FirstLepton() { return firstLepton;}
const Particle* Event::SecondLepton() { return secondLepton;}

const vector< const Jet*>* Event::BJets( const string& key, const double& disc_cut) { return BJetsPtOrdered( key, disc_cut);}
const vector< const Jet*>* Event::BJetsPtOrdered( const string& key, const double& disc_cut) {
  bJets.clear();
  
  const vector< const Jet*>* selectedJets = Jets("Selected");
  for ( int ijet = 0; ijet < selectedJets->size(); ijet++){
    if ( selectedJets->at(ijet)->IsBJet( key, disc_cut))
      bJets.push_back( selectedJets->at(ijet));
  }
  return &bJets;
}
const vector< const Jet*>* Event::BJetsBDiscOrdered( const string& key, const double& disc_cut){
  
  bJetsBDiscOrdered = *BJetsPtOrdered( key, disc_cut);

  std::sort( bJetsBDiscOrdered.begin(), bJetsBDiscOrdered.end(), desy_tools::compare_Jet_BDisc(key));
  
  return &bJetsBDiscOrdered;
}

double Event::HT() { return desy_tools::HT( Jets("Selected")); }
double Event::HTratio() { return desy_tools::HTratio( Jets("Selected"), &typeIPhiCorrMET );}
double Event::Meff() { return desy_tools::Meff( HT(), firstLepton->Pt(), typeIPhiCorrMET.Pt()); }

double Event::MHT()  { return desy_tools::MHT ( Jets("Good")); }
double Event::MHTx() { return desy_tools::MHTx( Jets("Good")); }
double Event::MHTy() { return desy_tools::MHTy( Jets("Good")); }
  
int Event::nJets(){ return (int) Jets("Selected")->size();}

int Event::nBJets(const string& key, const double& disc_cut){ 
  int count = 0;
  const vector< const Jet*>* selectedJets = Jets("Selected");
  
  for ( int ijet = 0; ijet < selectedJets->size(); ijet++){
    if ( selectedJets->at(ijet)->IsBJet( key, disc_cut) && fabs( selectedJets->at(ijet)->Eta()) < 2.4) {
      count++;
    }
  }
  
  return count;
}

double Event::Y(){ return desy_tools::Y( typeIPhiCorrMET.Pt(),  HT());}

double Event::MT() { return desy_tools::MT( firstLepton, &typeIPhiCorrMET); }

double Event::Mlb1(){ 
  const vector< const Jet*>* bJets = BJetsBDiscOrdered( "CSV");

  if ( bJets->size() > 0 ){
    return ( firstLepton->P4() + bJets->at(0)->P4()).M();
  }

  return -1.;
}
double Event::Mlb(){ return desy_tools::Mlb( firstLepton->P4(), this->BJets());}
double Event::M3b(){ return desy_tools::M3b( firstLepton->P4(), Jets("Selected"));}
double Event::MT2W(){ 
  mt2w_bisect::mt2w_interface mt2w_calc; 
  LorentzM tmpV = LorentzM( typeIPhiCorrMET.Pt(), typeIPhiCorrMET.Eta(), typeIPhiCorrMET.Phi(), typeIPhiCorrMET.M());
  vector< Jet*> tmpJets;
  
  for ( int ijet = 0; ijet < jets.size(); ijet++ ){
    if (jets.at(ijet).Is("Selected")){
      tmpJets.push_back( &jets.at(ijet));
    }
  }

  return mt2w_calc.get_mt2w( firstLepton, tmpJets, tmpV);
}

double Event::DeltaPhiMinj12m(){ return desy_tools::DeltaPhiMinj12m( &typeIPhiCorrMET, Jets("Selected"));}
double Event::DeltaRlb1(){ 
  const vector< const Jet*>* bJets = BJetsBDiscOrdered( "CSV");
  if ( bJets->size() > 0 ){
    return ROOT::Math::VectorUtil::DeltaR( firstLepton->P4(), bJets->at(0)->P4());
  }
  return -1.;
}
double Event::DeltaRlbmin(){ 
  double drlb = 99.;
  double drlb_new= 99.;
  const vector< const Jet*>* bJets = BJetsBDiscOrdered( "CSV");

  if ( bJets->size() == 0 ) return -1;

  for (int ibjet = 0; ibjet < bJets->size(); ibjet++ ){
    drlb_new = ROOT::Math::VectorUtil::DeltaR( firstLepton->P4(), bJets->at(ibjet)->P4());
    if (drlb_new < drlb) drlb = drlb_new;
  }
  return drlb;
}


double Event::DeltaPhiCaloTypeI(){ 
  return ROOT::Math::VectorUtil::DeltaPhi( caloMET, typeIPhiCorrMET);
}

double Event::HadChi2()
{
  const vector< const Jet*>* selectedJets = Jets("Selected");
  std::vector<TLorentzVector> selJets;
  std::vector<float> bDiscs;

  for ( int ijet = 0; ijet < selectedJets->size(); ijet++)
    {
      const Jet* j = selectedJets->at(ijet);

      TLorentzVector tmp;
      tmp.SetPxPyPzE( j->Px(), j->Py(), j->Pz(), j->E());
      selJets.push_back( tmp);
      bDiscs.push_back( j->BJetDisc("CSV"));
    }
  HadChi2::HadChi2 hadChi2;
  return hadChi2.GetChi2( selJets, bDiscs, info.isData);
}

double Event::Topness()
{
  const vector< const Jet*>* selectedJets = Jets("Selected");
  std::vector<TLorentzVector> selJets;
  std::vector<float> bDiscs;

  for ( int ijet = 0; ijet < selectedJets->size(); ijet++)
    {
      const Jet* j = selectedJets->at(ijet);

      TLorentzVector tmp;
      tmp.SetPxPyPzE( j->Px(), j->Py(), j->Pz(), j->E());
      selJets.push_back( tmp);
      bDiscs.push_back( j->BJetDisc("CSV"));
    }
  
  TLorentzVector Lep;
  Lep.SetPxPyPzE( firstLepton->Px(),
		  firstLepton->Py(),
		  firstLepton->Pz(),
		  firstLepton->E());

  TLorentzVector MET;
  MET.SetPxPyPzE( typeIPhiCorrMET.Px(),
		  typeIPhiCorrMET.Py(),
		  typeIPhiCorrMET.Pz(),
		  typeIPhiCorrMET.E());

  Topness::Topness top;
  //return -1.;
  return log(top.GetTopness( selJets, bDiscs, Lep, MET));
}

double Event::GlobalWeight(){
  if (info.isData) return 1.;
  else return info.xs * info.FE / (double) info.NEvents;
}

double Event::TriggerEfficiency(){
  if (info.isData) return 1.;
  else return trigger::GetTriggerEfficiency(*this);
}

double Event::EventWeight(){
  if (info.isData) return 1.;

  double weight =  GlobalWeight() * TriggerEfficiency() * info.PUWeight;

  if ( info.Sample.find("T2t") == 0 || info.Sample.find("T2bw") == 0 )
    return weight * info.isrWeight;
  
  if ( info.Sample.find("TTJets") == 0)
    return weight * info.topPtWeight;

  return weight;
}
