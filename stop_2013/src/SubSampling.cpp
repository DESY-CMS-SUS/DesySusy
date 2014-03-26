#include "SubSampling.h"
#include "Tools.h"

bool desy_tools::ScanCheck( const string& sample_,
			    const string& subSample_,
			    EasyChain* tree_){
  float mStop, mLSP;

  tree_->Get( mStop,"susyScanmStop");
  tree_->Get( mLSP, "susyScanmLSP");

  if (mLSP < 0.1) return false;

  if (mLSP > 351) return false;

  if (sample_.compare("T2tb") == 0 || sample_.compare("T2tbPoints") == 0){
    vector<int>& pdgId = tree_->Get( &pdgId, "genPdgId");
    vector<int>& status = tree_->Get( &status, "genStatus");
    int charginos = 0;
    for (int igen=0; igen<status.size(); igen++)
      if (abs(pdgId.at(igen))==1000024)
        charginos++;
	
    if (charginos == 0)
      return false;
    else 
      return true;
  }  

  if (sample_.compare("T2tt") == 0 || sample_.compare("T2ttPoints") == 0){
    if (subSample_.compare("mStop150To350mLSP0To250") == 0){
      if (mLSP < (mStop - 201.))
	return true;
      else 
	return false;
    }
    if (subSample_.compare("mStop150To475mLSP1") == 0){
      if (mStop > 201.)
        return true;
      else
	return false;
    }
  }

  if (sample_.compare("T2bw") == 0){
    if (subSample_.compare("mStop500To800mLSP0To700x0p25") == 0 ||
	subSample_.compare("mStop500To800mLSP0To700x0p50") == 0 ||
	subSample_.compare("mStop500To800mLSP0To700x0p75") == 0 ||
	subSample_.compare("mStop100To475mLSP0To375x0p25") == 0 ||
	subSample_.compare("mStop100To475mLSP0To375x0p50") == 0 ||
	subSample_.compare("mStop100To475mLSP0To375x0p75") == 0){
      if ( mLSP > 301.  ||
	   mStop > 651. ||
	   mLSP < (mStop - 351.) ){
	return true;
      }
      else{
	return false;
      }
    }
  }
  
  return true;
};

bool desy_tools::T2tbPoints( const string& subSample_, EasyChain* tree_){
  
  unsigned pos = subSample_.find("-");
  float mStop0 = atof(subSample_.substr(5,pos).c_str());
  float mLSP0 = atof(subSample_.substr(pos+5).c_str());

  float mStop1, mLSP1;
  tree_->Get( mStop1,"susyScanmStop");
  tree_->Get( mLSP1, "susyScanmLSP");
  
  if ( fabs(mStop0 - mStop1) < 0.001 && fabs(mLSP0 - mLSP1) < 0.001)
    return true;

  return false;
};

bool desy_tools::TTJetsSubSampling( const string& subSample_, EasyChain* tree_){
  
  vector<int>&      pdgId  = tree_->Get( &pdgId,       "genPdgId");
  vector<int>&      status = tree_->Get( &status,      "genStatus");
  vector<int>& MotherIndex = tree_->Get( &MotherIndex, "genMotherIndex");
  
  int LepFromTop = 0;
  for (int igen=0; igen<status.size(); igen++)
    if (abs(pdgId.at(igen))==6)
      LepFromTop += gen_tools::Decay(igen,&MotherIndex,&pdgId,&status,"");
  
  if (subSample_.find("FullyHad") != std::string::npos){
    if (LepFromTop != 0) return false;
  }
  else if (subSample_.find("SemiLept") != std::string::npos) {
    if (LepFromTop != 1) return false;
  }
  else if (subSample_.find("DiLept") != std::string::npos) {
    if (LepFromTop < 2) return false;
  }

  return true;
};



