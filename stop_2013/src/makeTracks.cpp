#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "TDirectory.h"
#include "THTools.h"
#include "makeTracks.h"

#include "Math/VectorUtil.h"

#ifndef CUTFLOW
#define CUTFLOW( cut, obj, flow_in)                 \
  static CutSet newFlow( cut+"_"+obj+"_Selection"); \
  newFlow.autodump=true;                            \
  CutSet* flow = &newFlow;                          \
  if (flow_in) {                                    \
    newFlow.autodump = false;                       \
    flow = flow_in;                                 \
  }
#endif

using namespace std;

extern bool pcp;

//======================================================
// make All Tracks
//======================================================

vector<Particle> makeAllTracks(EasyChain* tree, CutSet* flow_in){

  if(pcp)cout<<"inside makeAllTracks"<<endl;

  //====================================================================
  // DEFINE CUT FLOW
  //====================================================================
  CUTFLOW( TString("All"), TString("Tracks"), flow_in)
  //=end=initialize=cut=flow============================================

  //====================================================================
  // DEFINE CUTS
  //====================================================================
  ConfigReader config;

  static float PTMIN = config.getFloat( "AllTracks_PTMIN", 5. );
  static float DZMAX = config.getFloat( "AllTracks_DZMAX",  .1);
  //=end=define=cuts====================================================

  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  bool OK = false;
  double iso = 0.;
  bool isLepton = false;
  double deltaR = 999.;

  vector<Particle>  allTracks;
  Particle          dummyTrack;
  vector<LorentzM>& p4      = tree->Get( &p4,     "DESYtracksP4PF");
  vector<int>&      charge  = tree->Get( &charge, "DESYtracksChgPF");
  vector<int>&      pdgID   = tree->Get( &charge, "DESYtracksPdgIdPF");
  vector<float>&    dz      = tree->Get( &dz,     "DESYtracksDzPF");
  //=end=variables=initialization=======================================

  //=============================================
  // LOOP OVER THE TRACKS
  //=============================================
  for (int itrack=0; itrack<(int)p4.size(); itrack++ ) {

    if(pcp){
      cout<<"Track "<<itrack<<": ";
      cout<<"Pt = "<<p4.at(itrack).Pt()<<";";
      cout<<"Eta = "<<p4.at(itrack).Eta()<<";";
      cout<<"Phi = "<<p4.at(itrack).Phi()<<".";
    }

    OK=p4.at(itrack).pt() > PTMIN;
    if (!flow->keepIf("tr_All_PTMIN",OK) ) continue;
    //
    OK=fabs( dz.at(itrack) ) < 0.1;
    if (!flow->keepIf("tr_All_DZMAX",OK) ) continue;
    //
    OK=charge.at(itrack)  !=  0;
    if (!flow->keepIf("tr_All_CHARGED",OK) ) continue;
    
    dummyTrack.Set( itrack, p4.at(itrack), charge.at(itrack), -1.);
    dummyTrack.SetPdgID( pdgID.at(itrack));
    dummyTrack.SetDz ( dz.at(itrack));

    iso = 0.;
    isLepton = abs( dummyTrack.PdgID())==11 || abs( dummyTrack.PdgID())==13;
    
    for (int jtrack=0; jtrack<(int)p4.size(); jtrack++ ) {
      
      if (itrack == jtrack) continue;

      if ( dz.at(jtrack) >= 0.1) continue;
      
      if ( isLepton) {
	if ( abs( pdgID.at(jtrack))==11 || abs( pdgID.at(jtrack))==13) continue;
      }

      deltaR= ROOT::Math::VectorUtil::DeltaR( dummyTrack.P4(), p4.at(jtrack));
      if ( deltaR >= 0.3) continue;
      
      iso += p4.at(jtrack).pt();
    }
    
    dummyTrack.SetIso( iso);

    allTracks.push_back(dummyTrack);
  }
  //=end=loop=over=tracks===============================================

  return allTracks;
}
#undef CUTFLOW
