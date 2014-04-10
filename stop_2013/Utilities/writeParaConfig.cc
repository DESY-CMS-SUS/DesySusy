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

int  writeParaConfig ( TString& dir, TString& sample, TString& subsample, TString& Ntries, TString& xs, TString& fe)
{
  if( dir.IsNull()){
    cout << "please enter the path for the para_config.txt file (i.e. ./path1/path2/) \n '0' for default ra4b path" << endl;
    cin >> dir;
  }
  
  ifstream in;
  ofstream out;
  TString FullPath; 
  if (dir =="0"){
    in.open("./ra4b_2012/para_config.txt");
    out.open("./ra4b_2012/temp_config.txt");
    FullPath="./ra4b_2012/";
  }
  else{
    in.open(dir+"/para_config.txt");
  }

  char newfile='n';
  if( !in.is_open()){
    cout << "could not open the file, a new file will be generated (y/n)" << endl;
    cin >> newfile;
    if(newfile=='y') {
      FullPath="./";
      out.open("./temp_config.txt");
    }
    else return 1;
  }
  else if (dir!="0") { out.open(dir+"/temp_config.txt"); FullPath=dir+"/";}
  
  TString line;
  char line2[2500];
  TString samplename;
  Ssiz_t from = 0;    
  bool flag = false;

  if (newfile=='y') {
    out << "# xs_<sample name>_<sub sample name> -> Cross Section  of the" << endl;
    out << "#corresponding <sample name><sub sample name>" << endl;
    out << "xs_"+sample+"_"+subsample+"     = " << xs <<  endl;
    out << "" << endl;
    out << "#(T)otal (N)umber (o)f (E)vents" << endl;
    out << "TNoE_"+sample+"_"+subsample+"     = " << Ntries <<  endl;
    out << "" << endl;
    out <<"#(F)ilter (E)fficiency" << endl;
    out <<"FE_"+sample+"_"+subsample+"     = " << fe <<  endl;
  }

  else {
    while(in.getline(line2,2500)){
      from = 0;
      line=TString(line2);
      line.Tokenize(samplename,from,"=");
    
      if(line=="")
	break;
      
      if(!(samplename.Strip()=="xs_"+sample+"_"+subsample))
	out<<line<<endl;
      else {
	out <<"xs_"+sample+"_"+subsample +"     = " << xs << endl;
	flag = true;
      }
    }
    if (!flag)
      out <<"xs_"+sample+"_"+subsample +"     = " << xs << endl;
    out<< "" <<endl;

    flag = false;
    while(in.getline(line2,2500)){
      from = 0;
      line=TString(line2);
      line.Tokenize(samplename,from,"=");
      
      if(line=="")
	break;

      if(!(samplename.Strip()=="TNoE_"+sample+"_"+subsample))
	out << line << endl;
      else {
	out <<"TNoE_"+sample+"_"+subsample+"     = " << Ntries << endl;
	flag = true;
      }
    }
    if (!flag)
      out <<"TNoE_"+sample+"_"+subsample+"     = " << Ntries << endl;
    out<< "" <<endl;

    flag = false;
    while(in.getline(line2,2500)){
      from = 0;
      line=TString(line2);
      line.Tokenize(samplename,from,"=");

      if(!(samplename.Strip()=="FE_"+sample+"_"+subsample))
	out << line <<  endl;
      else {
	out <<"FE_"+sample+"_"+subsample+"     = " << fe << endl;
	flag =true;
      }
    }
    if (!flag)
      out <<"FE_"+sample+"_"+subsample+"     = " << fe << endl;
  }

  in.close();
  out.close();
  remove (FullPath+"para_config.txt");
  rename (FullPath+"temp_config.txt",FullPath+"para_config.txt");
  return 0;
}


//read the arguments and act clever :P
int main (int argc, char* argv[])
{

  TString parapath;
  TString name, name2;
  TString Ntries, xs, fe;
  if (argc != 7){
  cout << "please enter SAMPLE name"<< endl;
  cin >> name;
  cout << "please enter SUBSAMPLE name" << endl;
  cin >> name2;
  cout << "please enter NTRIES" << endl;
  cin >> Ntries;
  cout << "please enter XS" << endl;
  cin >> xs;
  cout << "please enter FE" << endl;
  cin >> fe;
  }
  else { name = argv[1]; name2 = argv[2]; parapath = argv[3]; Ntries = argv[4]; xs = argv[5]; fe = argv[6];}
  writeParaConfig( parapath, name, name2, Ntries, xs, fe);
  return 0;
}

