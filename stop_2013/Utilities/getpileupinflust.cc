//version 0.99(Beta)
//a fancy PU inf reader (and writer :))
//usage:
//./getpileupinflust <sample_name> <subsample_name> <PATH of the sample> <path of the pu config>
//If you forgot to add these arguments it will ask for them... 
#include <cstdlib>
#include <algorithm>
#include <functional>
#include "TH1.h"
#include "TObjArray.h" 
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <vector> 
#include <TFile.h>
#include <TSystem.h>
#include "TTree.h"
#include "TChain.h"
#include "TChainElement.h"
#include "TSystem.h"
#include "TROOT.h"
#include <TString.h>
#include "TObjArray.h"
#include <TObjString.h>
#include "TStopwatch.h"
#include "Math/LorentzVector.h"
#include "Math/DisplacementVector3D.h"
#include "Math/GenVector/PositionVector3D.h"
#include "Math/GenVector/PxPyPzE4D.h"
#include "Math/GenVector/PtEtaPhiE4D.h"
#include "Math/GenVector/PxPyPzM4D.h"
#include "Math/GenVector/PtEtaPhiM4D.h"

#ifdef __GNUC__
// only visible for the gnu pre-compiler
//#warning __GNUC__
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif


using namespace std;

// unfortunately by the nature of the reponsibility of this method, it includes some file and TString manipulations. therefore, please treat gently...
// it takes pileup distribution and sample + subsample name, modify pu_config.txt (change the existing values or write a new entity with the given sample subsample name).

 
int  writePUconfig ( TString& pudir, TH1F *pu, TString & sample, TString& subsample, bool data= true)
{
  TString mcordata;
  if(data) mcordata="data";
  else mcordata="mc";
  TString pathpu;
  if(pudir.IsNull()){
    cout << "please enter the path for the pu_config.txt file (i.e. ./path1/path2/) \n '0' for default ra4b path" << endl;
    cin >> pathpu;
  }
  pathpu = pudir;
  if(sample.IsNull()){
  if(!data){  cout << "please enter sample and sub_sample name (i.e. 'TTJets SUMMER12')" << endl;
    cin >> sample >> subsample;}
  else {sample="";subsample="";}
  }
  if(!data) {sample=sample+"_"; subsample=subsample+"_";}
  ifstream in;  
  ofstream out;
  TString FullPath; 
  if (pathpu =="0"){
    in.open("./ra4b_2012/pu_config.txt");
    out.open("./ra4b_2012/temp_config.txt");
    FullPath="./ra4b_2012/";
  }
  else{
    in.open(pathpu+"/pu_config.txt");
    
  }
  pu->Scale(1/pu->Integral());  
  char yorn='n';
  if( !in.is_open()){
    cout << "could not open the file, a new file will be generated (y/n)" << endl;
    cin >> yorn;
    if(yorn=='y') {cout << "lol";
      FullPath="./";
      out.open("./temp_config.txt");}
    else return 0;

    //return 1;
  }
  else if (pathpu!="0") { out.open(pathpu+"/temp_config.txt"); FullPath=pathpu+"/";}
  TString line;
  char line2[2500];
  TString samplename;
  Ssiz_t from = 0;    
  //  int from = 0;
  bool flag = false;
  while(in.getline(line2,2500)){
    from = 0;
    line=TString(line2);
    // cout << " token bool" << line.Tokenize(samplename,from,"=") << " token "<< tokeneq<<" line   " << line << endl;
    if(line.Tokenize(samplename,from,"=") && line[0]!=35 && line.Sizeof()>0) cout << "||" ;//{ cout <<" samplename "<< samplename << "   name generated " << "PU_"+sample+subsample+mcordata<< endl;}
    // cout <<"samplename "<< samplename << "   name generated " << "PU_"+sample+subsample+mcordata<< endl;
    //    cout << endl;
    if(!(samplename.Strip()=="PU_"+sample+subsample+mcordata)){ out<<line<<endl;}
    else { out <<"PU_"+sample+subsample+mcordata+"     = " ;
      for(unsigned bin = 1; bin<pu->GetXaxis()->GetXmax()+1; ++bin)
	out << pu->GetBinContent(bin) << " ";
      out << endl; flag = true;
    }
  }
  if(yorn=='y') {
    out <<"PU_"+sample+subsample+mcordata+"     = " ;
    for(unsigned bin = 1; bin<pu->GetXaxis()->GetXmax()+1; ++bin)
      out << pu->GetBinContent(bin) << " ";   
    out << endl; flag = true;
  }
  if (!flag){
    out <<"PU_"+sample+subsample+mcordata+"     = " ;
    for(unsigned bin = 1; bin<pu->GetXaxis()->GetXmax()+1; ++bin)
      out << pu->GetBinContent(bin) << " ";   
    out << endl;
  }
  in.close();
  out.close();
  remove (FullPath+"pu_config.txt");
  rename (FullPath+"temp_config.txt",FullPath+"pu_config.txt");
  return 0;
}
//I took this part from NTupleTools2.h
int GetResult(vector<TString>& out, const TString& command,bool nodup)
{
  TString line;
  FILE* pipe= gSystem->OpenPipe(command,"r");
  if(!pipe){
    cerr<<"Did not work: "<<command<<endl;
  } else {
    while (line.Gets(pipe)) if(line!="") {
      out.push_back(string(line));
    }
    gSystem->ClosePipe(pipe);
  }
  /*  if(nodup){
    map<string, pair<unsigned,string> > singleOut;
    map<string, pair<unsigned,string> >::iterator it;
    unsigned i;
    for(i=0;i<out.size();i++){
      //check format
      if(out[i].CountChar('_') < 2) break;
      //let's hope it fits
      unsigned pos =out[i].Last('_');
      unsigned pos2=TString(out[i].SubString(0,pos)).Last('_')+1;
      unsigned n=atoi(TString(out[i].SubString(pos2,pos-pos2)).c_str()));
      it = singleOut.first((TString)(out[i].SubString(0,pos2)));
      if(it!=singleOut.end()) {
	if(it->second.first<n) {
	  it->second.first=n;
	  it->second.second=out[i].SubString(pos2);
	}
      } else singleOut[out[i].SubString(0,pos2)]=pair<unsigned,string>(n,out[i].SubString(pos2));
  }
    if(i==out.size()){
      if (out.size()!=singleOut.size()) cout<< out.size()-singleOut.size() <<" duplicates ignored!"<<endl;;
      out.clear();
      for(it = singleOut.begin();it!=singleOut.end();it++)
	out.push_back(it->first+it->second.second);
    } else cout<<"File name format not appropriate for duplicate check!"<<endl;
    }*/
  return out.size();
}
int getpileupinfo(TString& dir,TString& pudir, TString& SAMPLE, TString& SUBSAMPLE)
{
  bool nodup = false;
  vector<TString> files;
  int n=0;
  n=GetResult(files,"ls "+dir+" | grep \"\\.root\" ",nodup);
  const TString dcache_gate="dcap://dcache-cms-dcap.desy.de:22125/";
  TH1F *histo= new TH1F ("histo","Pileup dist of "+SAMPLE+" "+SUBSAMPLE ,100,0,100);
  if (n == 0) {cout << "...in Sonas or Lust: Checking the dcache..." << endl; n=GetResult(files,"dcls "+dir+" | grep \"\\.root\" ",nodup); dir = dcache_gate+dir;}


  cout << n << endl;
  for(int i=0;i<n;++i) 
    {
      TFile * t = TFile::Open(dir+"/"+files[i]);
      t->cd("susydesypu");
      TH1F *pu2; 
      gDirectory->GetObject("pu",pu2);
      histo->Add(pu2);
      cout  << files[i+1]<<" "<<i+1 << " file \r" << flush ;
      pu2->Delete();
      t->Close();
    }
  cout << endl;
  cout<<"# of events: " <<(int) histo->GetEntries() << endl;
  TFile* pudist= new TFile ("PU_"+SAMPLE+"_"+SUBSAMPLE+".root","RECREATE");
  writePUconfig(pudir, histo, SAMPLE, SUBSAMPLE, false);
  histo->Draw();

  histo->Write();
  pudist->Write();
  return (int) histo->GetEntries();
}
//read the arguments and act clever :P
int main (int argc, char* argv[])
{

  TString path, pupath;
  TString name, name2;
  if (argc != 5){
  cout << "please enter the sample's path" << endl;
  cin >> path;
  cout << "please enter the SAMPLE SUBSAMPLE name" << endl;
  cout << "(i.e. 'TTJETS SUMMER12')" << endl; 
  cin >> name >> name2;
  }
  else { name = argv[1]; name2 = argv[2]; path = argv[3]; pupath = argv[4]; }
  int Ntries=getpileupinfo(path,pupath,name,name2);
  cout << "%" << Ntries << "%" <<endl;
  return 0;
}
