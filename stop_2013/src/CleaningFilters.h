#ifndef CleaningFilters_h
#define CleaningFilters_h

#include "NtupleTools2_h.h"
#include "ConfigReader.h"

namespace cleaningFilters{

  bool goodVertices( EasyChain* tree, vector<int>& goodVertices);
  
  bool cscHalo( EasyChain* tree);
  
  bool hbheNoise( EasyChain* tree);
  
  bool hcalLaser( EasyChain* tree);
  
  bool ecalDeadCellTP( EasyChain* tree);
  
  bool trackingFailure( EasyChain* tree);
  
  bool badEESuperCluster( EasyChain* tree);
  
  bool ecalXtalLargeCorr( EasyChain* tree);
 
  bool scrapingVeto( EasyChain* tree);

  bool trackingOddEvent( EasyChain* tree);

  bool anomalousRho( EasyChain* tree);

  bool METconsistency( LorentzM& caloMET, LorentzM& pfMET);
}
#endif
