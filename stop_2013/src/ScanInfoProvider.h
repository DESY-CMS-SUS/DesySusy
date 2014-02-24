#ifndef ScanInfoProvider_h
#define ScanInfoProvider_h

#include <string>

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

#include "NtupleTools2_h.h"
#include "Event.h"

using namespace std;

namespace desy_tools{
  class ScanInfoProvider{
  public:
    ScanInfoProvider( const string& filename = "data/ScanInfo.root");
    ~ScanInfoProvider();

    void Set(const string&, const string&, EasyChain*);

    string sample;
    string subSample;

    float mStop;
    float mLSP;
    float mChargino;
    float x;
    
    float xs;
    int   NEvents;
    float FE;

  private:
    TFile* infoFile;

    TH1D* h_xs;
    TH2I* h_scan;    
  };

  bool ScanCheck(const string&, const string&, EasyChain*);
  bool T2tbPoints( const string& subSample_, EasyChain* tree_);
  bool TTJetsSubSampling( const string& subSample_, EasyChain* tree_);
}
#endif
