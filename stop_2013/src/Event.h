#ifndef Event_h
#define Event_h

#include "EventInfo.h"
#include "Particle.h"
#include "Muon.h"
#include "Electron.h"
#include "Tau.h"
#include "Jet.h"
#include "GenJet.h"
#include "mt2w_interface.h"

#include "TString.h"
#include "NtupleTools2_h.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

class Event{

 private:
  EventInfo        info;
  vector<Particle> tracks;
  vector<Muon>     muons;
  vector<Electron> electrons;
  vector<Tau>      taus;
  vector<Jet>      jets;
  vector<GenJet>   genJets;
  LorentzM         unclusteredEnergy;
  LorentzM         rawMET;
  LorentzM         typeIMET;
  LorentzM         typeIPhiCorrMET;
  LorentzM         mvaMET;
  LorentzM         caloMET;       
  
  Particle*        firstLepton;
  Particle*        secondLepton;

  map< TString, vector< const Particle* > > tracksMap;
  map< TString, vector< const Muon* > >     muonsMap;
  map< TString, vector< const Electron* > > electronsMap;
  map< TString, vector< const Tau* > >      tausMap;
  map< TString, vector< const Jet* > >      jetsMap;
  map< TString, vector< const GenJet* > >   genJetsMap;

  vector< const Jet* > bJets;
  vector< const Jet* > bJetsBDiscOrdered;

 public:
  Event() {};
  Event( const Event& copy){
    SetInfo             ( *copy.Info());
    SetTracks           (  copy.CopyTracks());
    SetMuons            (  copy.CopyMuons());
    SetElectrons        (  copy.CopyElectrons());
    SetTaus             (  copy.CopyTaus());
    SetJets             (  copy.CopyJets());
    SetGenJets          (  copy.CopyGenJets());
    MatchGenJets();
    SetUnclusteredEnergy( *copy.UnclusteredEnergy());
    SetRawMET           ( *copy.RawMET());
    SetTypeIMET         ( *copy.TypeIMET());
    SetTypeIPhiCorrMET  ( *copy.TypeIPhiCorrMET());
    SetMvaMET           ( *copy.MvaMET()); 
    SetCaloMET          ( *copy.CaloMET());
  };

  Event& operator= (const Event& copy){
    SetInfo             ( *copy.Info());
    SetTracks           (  copy.CopyTracks());
    SetMuons            (  copy.CopyMuons());
    SetElectrons        (  copy.CopyElectrons());
    SetTaus             (  copy.CopyTaus());
    SetJets             (  copy.CopyJets());
    SetGenJets          (  copy.CopyGenJets());
    MatchGenJets();
    SetUnclusteredEnergy( *copy.UnclusteredEnergy());
    SetRawMET           ( *copy.RawMET());
    SetTypeIMET         ( *copy.TypeIMET());
    SetTypeIPhiCorrMET  ( *copy.TypeIPhiCorrMET());
    SetMvaMET           ( *copy.MvaMET()); 
    SetCaloMET          ( *copy.CaloMET());
    
    return *this;
  }

  void SetInfo             ( const EventInfo& in);
  void SetTracks           ( const vector<Particle>& in);
  void SetMuons            ( const vector<Muon>& in);
  void SetElectrons        ( const vector<Electron>& in);
  void SetTaus             ( const vector<Tau>& in);
  void SetJets             ( const vector<Jet>& in);
  void SetGenJets          ( const vector<GenJet>& in);
  void SetUnclusteredEnergy( const LorentzM& in);
  void SetRawMET           ( const LorentzM& in);
  void SetTypeIMET         ( const LorentzM& in);
  void SetTypeIPhiCorrMET  ( const LorentzM& in);
  void SetMvaMET           ( const LorentzM& in); 
  void SetCaloMET          ( const LorentzM& in);

  void SetTracks           ( const vector<Particle*>& in);
  void SetMuons            ( const vector<Muon*>& in);
  void SetElectrons        ( const vector<Electron*>& in);
  void SetTaus             ( const vector<Tau*>& in);
  void SetJets             ( const vector<Jet*>& in);
  void SetGenJets          ( const vector<GenJet*>& in);
  void SetLeptonPair();

  void MatchGenJets();

  const EventInfo*   Info      () const;
  const vector< const Particle* >* Tracks    (const string& key="All");
  const vector< const Muon* >*     Muons     (const string& key="All");
  const vector< const Electron* >* Electrons (const string& key="All");
  const vector< const Tau* >*      Taus      (const string& key="All");
  const vector< const Jet* >*      Jets      (const string& key="All");
  const vector< const GenJet* >*   GenJets   (const string& key="All");
  const LorentzM*           UnclusteredEnergy () const;
  const LorentzM*           RawMET    ()         const;
  const LorentzM*           TypeIMET  ()         const;
  const LorentzM*           TypeIPhiCorrMET()    const;
  const LorentzM*           MvaMET    ()         const;
  const LorentzM*           CaloMET   ()         const;

  vector< Particle > CopyTracks()    const { return tracks;};
  vector< Muon >     CopyMuons()     const { return muons;};
  vector< Electron > CopyElectrons() const { return electrons;};
  vector< Tau >      CopyTaus()      const { return taus;};
  vector< Jet >      CopyJets()      const { return jets;};
  vector< GenJet >   CopyGenJets()   const { return genJets;};

  const Particle*           FirstLepton();
  const Particle*           SecondLepton();
  const vector< const Jet*>* BJets( const string& key="CSV", const double& disc_cut=0.679);
  const vector< const Jet*>* BJetsPtOrdered( const string& key="CSV", const double& disc_cut=0.679);
  const vector< const Jet*>* BJetsBDiscOrdered( const string& key="CSV", const double& disc_cut=0.679);

  double GlobalWeight(); 
  double TriggerEfficiency();
  double EventWeight();

 protected:
  template <class T> vector< const T*> selectObjects( vector<T>& objs, map< TString, vector< const T* > >& objsMap, const string& key){

    typename map< TString, vector< const T* > >::iterator it = objsMap.find( key);
    
    if( it == objsMap.end()){
      vector< const T* > tmp;
      tmp.clear();
      
      for ( int iobj = 0; iobj < objs.size(); iobj++){
	if ( key.compare("All") == 0) {
	  tmp.push_back( &objs.at(iobj));
	}
	else if ( objs.at(iobj).Is( key) ){
	  tmp.push_back( &objs.at(iobj));
	}
      }
      
      objsMap[key] = tmp;
    }
    
    return objsMap[key];
  };
 
  template <class T> vector<T> objFromPointers( const vector<T*>& objs) const{
    vector< T> tmp;
    for ( int iobj = 0; iobj < objs.size(); iobj++)
      tmp.push_back( *objs.at(iobj));
    
    return tmp;
  };
  
 public:
  
  double HT();
  double HT3();
  double HT4();
  double HT5();
  double HTratio();
  double Meff();

  double MHT();
  double MHTx();
  double MHTy();
  
  int nJets();
  int nBJets( const string& key="CSV", const double& disc_cut = 0.679);

  double Y();
  
  double MT();

  double Mlb1();
  double Mlb();
  double M3b();
  double M3();
  double Centrality();
  double MT2W();

  double DeltaPhiMinj12m();
  double DeltaRlb1();
  double DeltaRlbmin();

  double DeltaPhiCaloTypeI();
  double HadChi2();
  double Topness();
};
#endif
