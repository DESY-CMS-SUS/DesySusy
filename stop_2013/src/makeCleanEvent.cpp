#include "makeCleanEvent.h"

#include "ConfigReader.h"
#include "CleaningFilters.h"
#include "EventFilterFromListStandAlone.h"

using namespace std;

extern bool pcp;
extern EventFilterFromListStandAlone badLaserFilter;
bool makeCleanEvent(EasyChain* tree, CutSet* globalFlow){
  bool OK=false;

  ConfigReader config;
  bool isData=config.getBool("isData");
  bool isFSIM=config.getBool("isFSIM");

  unsigned int event; tree->Get( event, "event");
  unsigned int lumi;  tree->Get(  lumi, "lumiSection");
  unsigned int run;   tree->Get(   run, "run");

  static CutSet newFlow( "EventCleaning");
  newFlow.autodump=true;
  CutSet* flow = &newFlow;
  if (globalFlow) {
    newFlow.autodump = false;
    flow = globalFlow;
  }

  //====================================================================
  // Scraping Veto
  //====================================================================
  if(pcp)cout<<"Scraping veto!"<<endl;
  OK = cleaningFilters::scrapingVeto( tree);
  if( !flow->keepIf("Scraping_Veto", OK ) ) return false;
  //====================================================================

  //====================================================================    
  // check CSCHALO
  //==================================================================== 
  if(pcp)cout<<"CSC halo filter!"<<endl;
  OK = true;
  if (!isFSIM) OK = cleaningFilters::cscHalo( tree); 
  if( !flow->keepIf("CSC_HALO", OK) ) return false;
  //====================================================================

  //====================================================================    
  // check HBHE Noise Filter
  //====================================================================
  if(pcp)cout<<"HBHE Noise filter!"<<endl;
  OK = cleaningFilters::hbheNoise( tree);
  if( !flow->keepIf("HBHE", OK) ) return false;
  //====================================================================

  //====================================================================    
  // check HCal Laser Filter
  //====================================================================
  if(pcp)cout<<"HCal laser filter!"<<endl;
  OK = true;
  if (isData) OK = cleaningFilters::hcalLaser( tree);
  if( !flow->keepIf( "hcalLaserFilter", OK ) ) return false;
  //====================================================================

  //====================================================================    
  // check bad Laser Filer
  //====================================================================
  if(pcp)cout<<"Bad laser filter!"<<endl;
  OK = true;
  if (isData) OK = !badLaserFilter.filter( (int)run, (int)lumi, (int)event);
  if( !flow->keepIf( "badLaserFilter", OK ) ) return false;
  //==================================================================== 

  //====================================================================    
  // check ECal Dead Cell Filter
  //====================================================================  
  if(pcp)cout<<"ECal dead cell filter!"<<endl;
  OK = cleaningFilters::ecalDeadCellTP( tree);
  if( !flow->keepIf("ECAL_TP", OK) ) return false;
  //====================================================================

  //====================================================================    
  // check Tracking Failure Filter
  //====================================================================
  if(pcp)cout<<"Tracking failure filter!"<<endl;
  OK = cleaningFilters::trackingFailure( tree);
  if( !flow->keepIf( "trackingFailure", OK) ) return false;
  //====================================================================

  //====================================================================    
  // check Bad EE SuperCluster  Filter
  //==================================================================== 
  if(pcp)cout<<"Bad EE supercluster filter!"<<endl;
  OK = cleaningFilters::badEESuperCluster( tree);
  if( !flow->keepIf( "eeBadSCFilter", OK) ) return false;
  //====================================================================

  //====================================================================    
  // check ECal Xtal Laser Corr Filter FIX ME!!
  //====================================================================
  if(pcp)cout<<"ECal xtal laser corr filter!"<<endl;
  OK = cleaningFilters::ecalXtalLargeCorr( tree);
  if( !flow->keepIf( "ecalXtalLaserCorrFilter", OK) ) return false;  
  //====================================================================

  //====================================================================    
  // check Tracking Odd Event Filter FIX ME!!
  //====================================================================
  if(pcp)cout<<"Tracking odd event filter!"<<endl;
  OK = true;
  if (!isFSIM) OK = cleaningFilters::trackingOddEvent( tree);
  if( !flow->keepIf( "trackingOddEvent", OK) ) return false;  
  //====================================================================

  return true;
}
