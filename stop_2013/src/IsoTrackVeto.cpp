#include "IsoTrackVeto.h"
#include "Math/VectorUtil.h"

// This filter has been sincronized and tested
// Parameters of this filter are hard coded. Please don't change them

void isoTrackVeto::IsoTracksV4 ( vector<Particle>& allTracks) {

  float iso = 0.;
  bool isLepton = false;
  float deltaR = 0.;
  
  vector<Particle> tmpTracks;

  for (int itrack=0; itrack<(int)allTracks.size(); itrack++ ) {

    iso = 0.;
    isLepton = abs( allTracks.at(itrack).PdgID())==11 || abs( allTracks.at(itrack).PdgID())==13;

    if ( allTracks.at(itrack).Pt() < 5.) continue;

    for (int jtrack=0; jtrack<(int)allTracks.size(); jtrack++ ) {
      
      if (itrack == jtrack) continue;

      if (allTracks.at(jtrack).Dz() >= 0.1) continue;
      
      if ( isLepton) {
	if ( abs( allTracks.at(jtrack).PdgID())==11 || abs( allTracks.at(jtrack).PdgID())==13) continue;
      }

      deltaR= ROOT::Math::VectorUtil::DeltaR( allTracks.at(itrack).P4(), allTracks.at(jtrack).P4());
      if ( deltaR >= 0.3) continue;

      iso += allTracks.at(jtrack).pt();
    } 

    allTracks.at(itrack).SetIso( iso);
    tmpTracks.push_back( allTracks.at(itrack));
  }

  allTracks = tmpTracks;
  return;
}

bool isoTrackVeto::IsoTrackVetoV4 ( Particle lepton, vector<Particle>& allTracks){

  vector< const Particle*> tracks;
  for (int itrack=0; itrack<(int)allTracks.size(); itrack++ )
    tracks.push_back( &allTracks.at(itrack));
  return IsoTrackVetoV4( &lepton, &tracks);
}


bool isoTrackVeto::IsoTrackVetoV4 ( const Particle* lepton, const vector< const Particle*>* allTracks){

  const Particle* track;
  float deltaR = 0.;

  for (int itrack=0; itrack<(int)allTracks->size(); itrack++ ){
   
    track = allTracks->at(itrack);
    
    if ( track->Charge() == 0) continue;
    
    deltaR= ROOT::Math::VectorUtil::DeltaR( lepton->P4(), track->P4());
    if ( deltaR <= 0.4) continue;

    if ( abs( track->PdgID())==11 || abs( track->PdgID())==13) {
      
      if ( track->pt() <= 5.) continue;
      
      if ( track->RelIso() > 0.2) continue;

      return true;
    }    
    else {
      
      if ( track->Charge() * lepton->Charge() > 0.) continue;  
      
      if ( track->pt() <= 10.) continue;
      
      if ( track->RelIso() > 0.1) continue;
  
      return true;
    }
  }
  
  return false;
}
