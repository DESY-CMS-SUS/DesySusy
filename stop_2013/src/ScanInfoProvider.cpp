#include "ScanInfoProvider.h"

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
  if (sample.find("T2tt") != std::string::npos) FE *= 0.25;

};
