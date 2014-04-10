#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2I.h"

#include <iostream>

#ifdef __GNUC__
// only visible for the gnu pre-compiler
//#warning __GNUC__
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif


using namespace std;

int main(int argc, char* argv[])
{
  TString sample, subsample, filename;
  TString answer;

  if (argc != 4){
    cout << "please enter the SAMPLE name" << endl;
    cin >> sample;
    cout << "please enter the SUBSAMPLE name" << endl;
    cin >> subsample;
    cout << "please enter the FILE PATH" << endl;
    cin >> filename;
  }
  else { sample = argv[1]; subsample = argv[2]; filename = argv[3];}

  TFile* infile = new TFile(filename);
  if (infile->IsZombie()){
    cout<<"Input File is Zombie"<<endl;
    return 1;
  }

  TFile* outfile = new TFile("ScanInfo.root", "UPDATE");

  TString histoname = sample; histoname += "_"; histoname += subsample; histoname += "_NEvents";

  TH2I* histo;
  outfile->GetObject( histoname, histo);
  
  if(histo){
    cout<<"Output File contains a TH2D for this sample."<<endl;
    cout<<"Overwrite it? (y/n)"<<endl;
    cin >> answer;
    answer.ToLower();

    if (answer.First("n") == 0) return 2;
  }  

  infile->GetObject( "susydesyscan/Scan", histo);

  outfile->cd();
  if (histo){
    histo->Write( histoname, TObject::kOverwrite);
    outfile->Write();
  }

  infile->Close();
  outfile->Close();
  
  delete infile;
  delete outfile;

  return 0;
}
