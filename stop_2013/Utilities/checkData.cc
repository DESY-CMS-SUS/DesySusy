#include <iostream>

#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"

//#include "NtupleTools2.h"
//#include "ConfigReader.h"

using namespace std;

int main(int argc, char** argv){
  cout<<"going to set inputs"<<endl;
  //===========================================
  // Set Input Files and Tree
  //===========================================
  TString filename = argv[1];

  TChain* tree = new TChain("/susyTree/tree");
  int f = tree->Add(filename);
  cout<<tree->GetNtrees()<<" Files read"<<endl;

  cout<<"check point before tree->GetEntries"<<endl;
  int N = tree->GetEntries();  cout<<"THERE ARE "<<N<<" EVENTS IN "<<filename<<endl;
  //===========================================
  cout<<"inputs set!"<<endl;
}
