#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"

#include <Math/Random.h>
#include <Math/GSLRndmEngines.h>
using namespace ROOT::Math;

#include <iostream>

int limitSetting(){

  int NToys = 1000000;
  double pExclusion = 0.05;

  ROOT::Math::Random<ROOT::Math::GSLRngMT> rand;

  TFile* outFile = TFile::Open("outFile.root","RECREATE");

  TH1D* bkgPDF = new TH1D( "bkg", "bkg", 10000, 0., 100.);
  TH1D* bkgAndSigPDF = new TH1D( "bkgAndSig", "bkgAndSig", 10000, 0., 100.);
  
  double bkg = 1.;
  double bkgStat = 0.3 * sqrt(bkg);
  double meanSPDbkg = bkg * bkg / ( bkgStat * bkgStat);
  double scaleSPDbkg = bkgStat  * bkgStat / bkg;  
  double bkgSys = 0.3;



  double sig = 3.;
  double sigStat = sqrt(sig);
  double meanSPDsig = sig * sig / ( sigStat * sigStat);
  double scaleSPDsig = sigStat  * sigStat / sig;
  double sigSys = 0.3;
  
  double data = 0.;
  double dataStat = sqrt(data);


  double bkg0, sig0;
  
  for ( int i = 0; i < NToys; i++){
    bkg0 = (double) rand.Poisson( meanSPDbkg);
    bkg0 *= scaleSPDbkg;
    bkg0 *= rand.LogNormal( 0, bkgSys);
    
    sig0 = rand.Poisson( meanSPDsig);
    sig0 *= scaleSPDsig;
    sig0 *= rand.LogNormal( 0, sigSys);
    
    bkgPDF->Fill( bkg0);
    bkgAndSigPDF->Fill( bkg0+sig0);
  }


  cout<<bkgPDF->Integral( 0, bkgPDF->GetNbinsX()+1, "width")<<endl;
  cout<<bkgAndSigPDF->Integral( 0, bkgAndSigPDF->GetNbinsX()+1, "width")<<endl;

  bkgPDF->Scale( 1./(float) bkgPDF->Integral(0, bkgPDF->GetNbinsX()+1, "width"));
  bkgAndSigPDF->Scale( 1./(float) bkgAndSigPDF->Integral(0, bkgAndSigPDF->GetNbinsX()+1, "width"));
  

  double pvalue_bkgOnly_bkg = bkgPDF->Integral( 0, bkgPDF->FindBin(bkg), "width");
  double pvalue_bkgOnly_data = bkgPDF->Integral( 0, bkgPDF->FindBin(data), "width");

  double pvalue_bkgAndSig_bkg = bkgAndSigPDF->Integral( 0, bkgAndSigPDF->FindBin(bkg), "width");
  double pvalue_bkgAndSig_data = bkgAndSigPDF->Integral( 0, bkgAndSigPDF->FindBin(data), "width");
  
  double CLs_expected = pvalue_bkgAndSig_bkg / pvalue_bkgOnly_bkg;
  double CLs_observed = pvalue_bkgAndSig_data / pvalue_bkgOnly_data;


  cout<<"p-value expected: "<<pvalue_bkgAndSig_bkg;
  if ( pvalue_bkgAndSig_bkg < pExclusion)
    cout<<", expected signal exclusion at 95CL";
  else
    cout<<", expected no signal exclusion at 95CL";
  cout<<endl;
  cout<<"p-value observed: "<<pvalue_bkgAndSig_data;
  if ( pvalue_bkgAndSig_data < pExclusion)
    cout<<", expected signal exclusion at 95CL";
  else
    cout<<", expected no signal exclusion at 95CL";
  cout<<endl;


  cout<<"Cls expected: "<<CLs_expected;
  if ( CLs_expected < pExclusion)
    cout<<", expected signal exclusion at 95CLs";
  else
    cout<<", expected no signal exclusion at 95CLs";
  cout<<endl;
  cout<<"CLs observed: "<<CLs_observed;
  if ( CLs_observed < pExclusion)
    cout<<", expected signal exclusion at 95CLs";
  else
    cout<<", expected no signal exclusion at 95CLs";
  cout<<endl;
  
  bkgPDF->Write();
  bkgAndSigPDF->Write();

  return 0;
}
