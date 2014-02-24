#include "ScanInfoProvider.h"
#include "Tools.h"

desy_tools::ScanInfoProvider::ScanInfoProvider( const string& filename){
  sample    = "NoSample";
  subSample = "NoSubSample";
  
  mStop     = -1.;
  mLSP      = -1.;
  mChargino = -1.;
  x         = -1.;
  
  xs      = -1.;
  NEvents = -1;
  FE      = -1.;
  
  infoFile = new TFile(TString(filename));
  infoFile->GetObject("T2_xs", h_xs);
};

desy_tools::ScanInfoProvider::~ScanInfoProvider(){
  delete h_xs;
  delete h_scan;
  
  infoFile->Close();
  delete infoFile;
};

void desy_tools::ScanInfoProvider::Set( const string& sample_,
					const string& subSample_,
					EasyChain* tree_){
  if ( sample.compare(sample_) != 0 || subSample.compare(subSample_) != 0){
    sample = sample_;
    subSample = subSample_;
    
    h_scan = (TH2I*) infoFile->Get( TString(sample) + "_" + TString(subSample) + "_NEvents");
  }

  tree_->Get( mStop,"susyScanmStop");
  tree_->Get( mLSP, "susyScanmLSP");
  
  if (sample_.compare("T2tb") == 0){
    if (mStop == 0){
      mLSP = 1.;
      mStop = 100.;
    }
  }

  mChargino = -1.;
  if (sample.find("T2bw") == 0){
    mChargino = tree_->Get( mChargino,"susyScanmCHI"); 
    mChargino /= 100.;
  } 
  else if (sample.find("T2tb") != std::string::npos)
    mChargino = mLSP + 5.;

  x = -1.;
  if (sample.find("T2bw") == 0){
    if (subSample.find("0p25") != std::string::npos ) x = 0.25;
    else if (subSample.find("0p50") != std::string::npos) x = 0.5;
    else if (subSample.find("0p75") != std::string::npos) x = 0.75;
  }

  xs = h_xs->GetBinContent( h_xs->FindBin(mStop));
  NEvents = (int) h_scan->GetBinContent( h_scan->FindBin( mStop, mLSP ));
  FE = 1.;
  if (subSample.find("LepFilter") != std::string::npos) FE = 0.543;
};

bool desy_tools::ScanCheck( const string& sample_,
			    const string& subSample_,
			    EasyChain* tree_){
  if (sample_.compare("T2tb") == 0)
    return true;
  
  float mStop, mLSP;
  
  tree_->Get( mStop,"susyScanmStop");
  tree_->Get( mLSP, "susyScanmLSP");

  if (mLSP < 0.1) return false;

  if (sample_.compare("T2tt") == 0){
    if (subSample_.compare("mStop150To350mLSP0To250") == 0){
      if (mLSP < (mStop - 201.))
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
  
  vector<LorentzM>& p4     = tree_->Get( &p4,          "genP4");
  vector<int>&      pdgId  = tree_->Get( &pdgId,       "genPdgId");
  vector<int>&      status = tree_->Get( &status,      "genStatus");
  vector<int>& MotherIndex = tree_->Get( &MotherIndex, "genMotherIndex");
  vector<int>& MotherPdgId = tree_->Get( &MotherPdgId, "genMotherPdgId");
  
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



