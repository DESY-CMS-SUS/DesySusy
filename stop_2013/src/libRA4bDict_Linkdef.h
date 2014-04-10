#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;
#pragma link C++ defined_in "NtupleTools2.h";
#pragma link C++ typedef LorentzM;
#pragma link C++ defined_in "EventInfo.h";
#pragma link C++ class EventInfo+;
#pragma link C++ defined_in "AnalysisObject.h";
#pragma link C++ defined_in "Particle.h";
#pragma link C++ defined_in "Muon.h";
#pragma link C++ defined_in "Electron.h";
#pragma link C++ defined_in "Tau.h";
#pragma link C++ defined_in "Jet.h";
#pragma link C++ defined_in "GenJet.h";
#pragma link C++ class AnalysisObject+;
#pragma link C++ class Particle+;
#pragma link C++ class Muon+;
#pragma link C++ class Electron+;
#pragma link C++ class Tau+;
#pragma link C++ class Jet+;
#pragma link C++ class GenJet+;
#pragma link C++ class std::vector<AnalysisObject>+;
#pragma link C++ class std::vector<Particle>+;
#pragma link C++ class std::vector<Muon>+;
#pragma link C++ class std::vector<Electron>+;
#pragma link C++ class std::vector<Tau>+;
#pragma link C++ class std::vector<Jet>+;
#pragma link C++ class std::vector<GenJet>+;
#pragma link C++ class std::vector<AnalysisObject*>+;
#pragma link C++ class std::vector<Particle*>+;
#pragma link C++ class std::vector<Muon*>+;
#pragma link C++ class std::vector<Electron*>+;
#pragma link C++ class std::vector<Tau*>+;
#pragma link C++ class std::vector<Jet*>+;
#pragma link C++ class std::vector<GenJet*>+;

#pragma link C++ class std::vector< const AnalysisObject*>+;
#pragma link C++ class std::vector< const Particle*>+;
#pragma link C++ class std::vector< const Muon*>+;
#pragma link C++ class std::vector< const Electron*>+;
#pragma link C++ class std::vector< const Tau*>+;
#pragma link C++ class std::vector< const Jet*>+;
#pragma link C++ class std::vector< const GenJet*>+;

//#pragma link C++ class std::string+;
//#pragma link C++ class std::map< std::string, bool>+;
//#pragma link C++ class std::map< std::string, bool>::iterator;
//#pragma link C++ class std::pair< std::string, string>+;

#pragma link C++ defined_in "mt2w_bisect.h";
#pragma link C++ class mt2w+;

#pragma link C++ defined_in "Event.h";
#pragma link C++ class Event+;

#pragma link C++ defined_in "Tools.h";

#pragma link C++ namespace gen_tools+;
#pragma link C++ class gen_tools::SUSYGenParticle+;
#pragma link C++ class std::vector<gen_tools::SUSYGenParticle>+;

#pragma link C++ namespace desy_tools+;
#pragma link C++ function desy_tools::Rescale( const LorentzM&, const double&);
#pragma link C++ function desy_tools::MT( const Particle*, const LorentzM*);
#pragma link C++ function desy_tools::MT( const Muon*, const LorentzM*);
#pragma link C++ function desy_tools::MT( const Electron*, const LorentzM*);
#pragma link C++ function desy_tools::MT( Particle*, LorentzM*);
#pragma link C++ function desy_tools::GetBJetSF( double, double, TString, TString, int);
#pragma link C++ function desy_tools::getJerSF(double, double);
#pragma link C++ function desy_tools::getJetResolution(double, double);

#pragma link C++ function desy_tools::matchGenJets( vector<Jet>&,  vector<GenJet>&);
#pragma link C++ function desy_tools::matchGenJets(vector<Jet*>&, vector<GenJet*>&);


//#pragma link off all globals defined_in "makeTracks.h";
//#pragma link C++ functins defined_in "Tools.h";
#pragma link C++ defined_in "IsoTrackVeto.h";
#pragma link C++ function isoTrackVeto::IsoTrackVetoV4 ( Particle, vector<Particle>&);
#pragma link C++ function isoTrackVeto::IsoTrackVetoV4 ( const Particle*, const vector< const Particle*>*);
#pragma link C++ defined_in "myBTagReshaping.h";
#pragma link C++ class myBTagReshaping+;

#pragma link C++ defined_in "SubTree.h";
#pragma link C++ class SubTree+;
#pragma link C++ defined_in "Bonsai.h";
#pragma link C++ class Bonsai+;
#pragma link C++ class std::vector<Bonsai*>+;

#endif
