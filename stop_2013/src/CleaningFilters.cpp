#include "NtupleTools2_h.h"
#include "ConfigReader.h"

#include "CleaningFilters.h"
#include "Math/VectorUtil.h"

using namespace ROOT::Math::VectorUtil;

extern bool pcp;

bool cleaningFilters::goodVertices(EasyChain* tree, vector<int>& goodVert){

  ConfigReader config;
  bool isData=config.getBool("isData");

  float Z_Data    = config.getFloat( "VertexZ_Data",   24.);
  float Rho_Data  = config.getFloat( "VertexRho_Data",  2.);
  float NDOF_Data = config.getFloat( "VertexNDOF_Data", 4.);

  float Z_MC    = config.getFloat( "VertexZ_MC",   24.);
  float Rho_MC  = config.getFloat( "VertexRho_MC",  2.);
  float NDOF_MC = config.getFloat( "VertexNDOF_MC", 4.);

  if(pcp)cout<<"================INSIDE THE VERTICES PROGRAM===================="<<endl;
  if(pcp)cout<<"getting the Vertices from the tree"<<endl;

  vector<XYZPointF>& vertices       = tree->Get( &vertices,      "vertexPosition");
  vector<int>&       vertex_isFake  = tree->Get( &vertex_isFake, "vertexIsFake"  );
  vector<float>&     vertex_ndof    = tree->Get( &vertex_ndof,   "vertexNdof"    );
  vector<int>&       vertex_nTracks = tree->Get( &vertex_nTracks,"vertexNtrks"   );

  if( vertices.size() == 0 ) return false;

  if(pcp)cout<<"loop over the vertices"<<endl;
  for( unsigned vx=0; vx < vertices.size(); ++vx ) {

    if( vertex_isFake.at(vx) ) continue;

    if( isData ){
      // data
      if( fabs(vertices.at(vx).z()) > Z_Data )     continue;
      if( fabs(vertices.at(vx).rho()) > Rho_Data ) continue;
      if( vertex_ndof.at(vx) <= NDOF_Data )        continue;
      goodVert.push_back(vx);
    } else {
      // MC
      if( fabs(vertices.at(vx).z()) > Z_MC ) continue;
      if( vertices.at(vx).rho() > Rho_MC )   continue;
      if( vertex_ndof.at(vx) <= NDOF_MC )    continue;
      goodVert.push_back(vx);
    }
  }

  return goodVert.size()>0;
}

bool cleaningFilters::cscHalo( EasyChain* tree){
  bool Tight =  tree->Get(Tight,"beamHaloCSCTightHaloId");  
  return !Tight;
};

bool cleaningFilters::hbheNoise( EasyChain* tree){
  bool hbheNoiseFilterResult = tree->Get(hbheNoiseFilterResult, "hbheNoiseNoIsoFilterResult");
  return hbheNoiseFilterResult;
};

bool cleaningFilters::hcalLaser( EasyChain* tree){
  bool hcalLaserEventFilterFlag = tree->Get( hcalLaserEventFilterFlag, "hcalLaser2012EventFilterFlag");
  return hcalLaserEventFilterFlag; 
};

bool cleaningFilters::ecalDeadCellTP( EasyChain* tree){
  bool ECAL_TP = tree->Get(ECAL_TP, "ecalDeadCellTPFilterFlag");
  return ECAL_TP;
};

bool cleaningFilters::trackingFailure( EasyChain* tree){
  bool trackingfailurefilterflag = tree->Get( trackingfailurefilterflag, "trackingFailureFilterFlag");
  return trackingfailurefilterflag;
};

bool cleaningFilters::badEESuperCluster( EasyChain* tree){
  bool eeBadSCPassed = tree->Get( eeBadSCPassed, "eeBadScFilterFlag");
  return eeBadSCPassed;
};

bool cleaningFilters::ecalXtalLargeCorr( EasyChain* tree){
  bool  ecalLaserCorrFilterFlag = tree->Get( ecalLaserCorrFilterFlag, "ecalLaserCorrFilterFlag");
  return ecalLaserCorrFilterFlag;
};

bool cleaningFilters::scrapingVeto( EasyChain* tree){
  unsigned int nTracksAll=tree->Get( nTracksAll, "nTracksAll");
  unsigned int nTracksHighPurity=tree->Get( nTracksHighPurity, "nTracksHighPurity");
  
  bool OK=true;
  if(nTracksAll>10){
    OK=false;
    if(double(nTracksHighPurity)/nTracksAll > 0.25)OK=true;
  }
  return OK;
};

bool cleaningFilters::trackingOddEvent( EasyChain* tree){
  bool manystripclusFilterFlag = tree->Get( manystripclusFilterFlag, "manystripclus53XFilterFlag");
  bool toomanystripclusFilterFlag = tree->Get( toomanystripclusFilterFlag, "toomanystripclus53XFilterFlag");
  bool logErrorTooManyClustersFilterFlag = tree->Get( logErrorTooManyClustersFilterFlag, "logErrorTooManyClustersFilterFlag");

  return !manystripclusFilterFlag && !toomanystripclusFilterFlag && !logErrorTooManyClustersFilterFlag;
};

bool cleaningFilters::anomalousRho( EasyChain* tree){

  ConfigReader config;
  static float rho_Max = config.getFloat( "Rho_Max", 40.);

  float rho = tree->Get( rho, "rho");
  return rho <= rho_Max;
};

bool cleaningFilters::METconsistency( LorentzM& caloMET, LorentzM& pfMET){

  ConfigReader config;
  static float DPhi_Max = config.getFloat( "METconsistency_DPhiMax", 1.5);

  return fabs( DeltaPhi( caloMET, pfMET)) < DPhi_Max;
};



 
