#ifndef myBTAGRESHAPHING_H
#define myBTAGRESHAPHING_H

#include "TObject.h"
#include "TFile.h"
#include "TGraphErrors.h"

#include <vector>
#include <stdio.h>
#include <string>

using namespace std;

class myBTagReshaping{
private:
  TFile* file;
  
  vector< float> etamin[3];
  vector< float> etamax[3];
  vector< float> ptmin[3][5];
  vector< float> ptmax[3][5];
  
  TGraphErrors* reshape[3][5][16];
  void DefineBin( const TObject* obj);
public:
  myBTagReshaping( const char* filename = "bdisc_reshaping.root");
  ~myBTagReshaping();
  int GetFlavorBin( int flavor);
  int GetEtaBin   ( int iflavor, float  eta);
  int GetPtBin    ( int iflavor,   int ieta, float pt);
  const TGraphErrors* GetReshape( int flavor, double eta, double pt);
  //void Reshape( int flavor, double pt, double eta){return;};
  double Reshape ( int flavor, double pt, double eta, double disc);

  //ClassDef (myBTagReshaping,1);
};


//#if !defined(__CINT__)
//ClassImp(myBTagReshaping);
#endif

