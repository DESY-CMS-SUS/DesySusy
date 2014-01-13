#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include "myBTagReshaping.h"
#include "TKey.h"
#include "TClass.h"

using namespace std;

//ClassImp(myBTagReshaping);

std::vector<std::string> inline StringSplit(const std::string &source, const char *delimiter = " ", bool keepEmpty = false)
{
  std::vector<std::string> results;
  
  size_t prev = 0;
  size_t next = 0;

  while ((next = source.find_first_of(delimiter, prev)) != std::string::npos) {
    if (keepEmpty || (next - prev != 0)) {
      results.push_back(source.substr(prev, next - prev));
    }
    prev = next + 1;
  }
    
  if (prev < source.size()) {
    results.push_back(source.substr(prev));
  }
  return results;
}


myBTagReshaping::myBTagReshaping( const char* filename){
  
  file = TFile::Open( filename, "READ");
  TIter nextkey( file->GetListOfKeys() );
  
  TKey *key;
  while ( (key = (TKey*)nextkey())){
    TObject *obj = key->ReadObj();
    if ( obj->IsA()->InheritsFrom( TGraphErrors::Class() ) )
      this->DefineBin( obj);

    else{
      cout << "Unknown object type, name: "
	   << obj->GetName() << " title: " << obj->GetTitle() << endl;
    }
  }
}
myBTagReshaping::~myBTagReshaping(){
  for ( int iflavor = 0; iflavor < 3; iflavor++)
    for ( unsigned int ieta = 0; ieta < etamin[iflavor].size(); ieta++)
      for ( unsigned int ipt = 0; ipt < ptmin[iflavor][ieta].size(); ipt++)
	delete reshape[iflavor][ieta][ipt];
  file->Close();
}

double myBTagReshaping::Reshape( int flavor, double pt, double eta, double disc){

  if ( disc < 0)
    return disc;
  if ( !this->GetReshape( flavor, eta, pt))
    return disc;
  else
    return (this->GetReshape( flavor, eta, pt))->Eval( disc);
}


void myBTagReshaping::DefineBin( const TObject* obj){
  string name = obj->GetName();  
  std::vector<std::string> strname = StringSplit( name, "_");

  if ( strname.size() != 4){
    cout<<"name format not supported"<<endl;
    return;
  }

  int iflavor = -1;
  float pt1 = -1.;
  float pt2 = -1.;
  float eta1 = -1.;
  float eta2 = -1.;

  if ( strname.at(1) == "l")
    iflavor = 0;
  else if ( strname.at(1) == "c")
    iflavor = 1;
  else if ( strname.at(1) == "b")
    iflavor = 2;

  strname.at(2).erase(0, 3);
  eta1 = (float) atof( StringSplit( strname.at(2), "-").at(0).c_str());
  eta2 = (float) atof( StringSplit( strname.at(2), "-").at(1).c_str());

  strname.at(3).erase( 0, 2);
  pt1 = (float) atof( StringSplit( strname.at(3), "-").at(0).c_str());
  pt2 = (float) atof( StringSplit( strname.at(3), "-").at(1).c_str());

  unsigned int ieta = 0;

  for ( ; ieta < etamin[iflavor].size(); ieta++)
    if ( fabs( eta1 - etamin[iflavor].at(ieta)) < 0.0001 && 
	 fabs( eta2 -= etamax[iflavor].at(ieta)) < 0.0001)
      break;

  if (ieta == etamin[iflavor].size()){
    ieta = etamin[iflavor].size();
    etamin[iflavor].push_back(eta1);
    etamax[iflavor].push_back(eta2);
  }
  
  ptmin[iflavor][ieta].push_back(pt1);
  ptmax[iflavor][ieta].push_back(pt2);

  reshape[iflavor][ieta][ptmin[iflavor][ieta].size()-1] = (TGraphErrors*) obj;
}

const TGraphErrors* myBTagReshaping::GetReshape( int flavor, double eta, double pt){

  int iflavor, ieta, ipt;
  iflavor = GetFlavorBin(iflavor);
  if ( iflavor < 0)
    return 0;
  ieta = GetEtaBin( iflavor, eta);
  if ( ieta < 0)
    return 0;
  ipt = GetPtBin( iflavor, ieta, pt);
  if ( ipt < 0)
    return 0;

  return reshape[iflavor][ieta][ipt];
}

int myBTagReshaping::GetFlavorBin( int flavor){
  int absf = abs(flavor);

  if ( absf == 5)
    return 2;
  else if ( absf == 4)
    return 1;
  else if ( absf == 0 || absf == 1 || absf == 2 || absf == 3 || absf == 21)
    return 0;
  
  return -1;
}

int myBTagReshaping::GetEtaBin( int iflavor, float eta){
  float abse = fabs(eta);
  for ( unsigned int ieta = 0; ieta < etamin[iflavor].size(); ieta++)
    if ( abse >= etamin[iflavor].at(ieta) && abse < etamax[iflavor].at(ieta))
      return (int) ieta;
        
  return -1;
}

int myBTagReshaping::GetPtBin( int iflavor, int ieta, float pt){
  float abspt = fabs(pt);
  for ( unsigned int ipt = 0; ipt < ptmin[iflavor][ieta].size(); ipt++)
    if ( abspt >= ptmin[iflavor][ieta].at(ipt) && abspt < ptmax[iflavor][ieta].at(ipt))
      return (int) ipt;
        
  return -1;
}
