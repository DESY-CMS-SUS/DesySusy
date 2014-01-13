#include "makeCleanEvent.h"

#include "ConfigReader.h"
#include "CleaningFilters.h"

using namespace std;

extern bool pcp;

bool makeCleanEvent(EasyChain* tree, CutSet* globalFlow){

  bool OK=false;

  ConfigReader config;
  bool isData=config.getBool("isData");
  
  //====================================================================
  // Scraping Veto
  //====================================================================
  if(pcp)cout<<"Scraping veto!"<<endl;
  OK = cleaningFilters::scrapingVeto( tree);
  if( !globalFlow->keepIf("Scraping_Veto", OK ) ) return false;
  //====================================================================

  //====================================================================    
  // check CSCHALO
  //==================================================================== 
  if(pcp)cout<<"CSC halo filter!"<<endl;
  OK = cleaningFilters::cscHalo( tree); 
  if( !globalFlow->keepIf("CSC_HALO", OK) ) return false;
  //====================================================================

  //====================================================================    
  // check HBHE Noise Filter
  //====================================================================
  if(pcp)cout<<"HBHE Noise filter!"<<endl;
  OK = cleaningFilters::hbheNoise( tree);
  if( !globalFlow->keepIf("HBHE", OK) ) return false;
  //====================================================================

  //====================================================================    
  // check HCal Laser Filter
  //====================================================================
  if(pcp)cout<<"HCal laser filter!"<<endl;
  if ( isData) {
    OK = cleaningFilters::hcalLaser( tree);
  }
  else {
    OK = true;
  }
  if( !globalFlow->keepIf( "hcalLaserFilter", OK ) ) return false;
  //====================================================================

  //====================================================================    
  // check ECal Dead Cell Filter
  //====================================================================  
  if(pcp)cout<<"ECal dead cell filter!"<<endl;
  OK = cleaningFilters::ecalDeadCellTP( tree);
  if( !globalFlow->keepIf("ECAL_TP", OK) ) return false;
  //====================================================================

  //====================================================================    
  // check Tracking Failure Filter
  //====================================================================
  if(pcp)cout<<"Tracking failure filter!"<<endl;
  OK = cleaningFilters::trackingFailure( tree);
  if( !globalFlow->keepIf( "trackingFailure", OK) ) return false;
  //====================================================================

  //====================================================================    
  // check Bad EE SuperCluster  Filter
  //==================================================================== 
  if(pcp)cout<<"Bad EE supercluster filter!"<<endl;
  OK = cleaningFilters::badEESuperCluster( tree);
  if( !globalFlow->keepIf( "eeBadSCFilter", OK) ) return false;
  //====================================================================

  //====================================================================    
  // check ECal Xtal Laser Corr Filter FIX ME!!
  //====================================================================
  if(pcp)cout<<"ECal xtal laser corr filter!"<<endl;
  OK= cleaningFilters::ecalXtalLargeCorr( tree);
  //if( !globalFlow->keepIf( "ecalXtalLaserCorrFilter", OK) ) return false;  
  //====================================================================

  //====================================================================    
  // check Tracking Odd Event Filter FIX ME!!
  //====================================================================
  if(pcp)cout<<"Tracking odd event filter!"<<endl;
  if ( isData) 
    OK = cleaningFilters::trackingOddEvent( tree);
  else
    OK = true;
  //if( !globalFlow->keepIf( "trackingOddEvent", OK) ) return false;  
  //====================================================================

  return true;

}
