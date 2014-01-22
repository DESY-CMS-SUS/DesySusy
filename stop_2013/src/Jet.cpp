#include "Jet.h"
#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

//ClassImp(Jet);

map<string, map<string, double> > Jet::bJetWP;


//=======CONSTRUCTORS=============//

Jet::Jet(){
  type="";
  isMatched=false;
  matchedGenJetIndex=-1;
  partonFlavor=0;
  matchedGenJet=0;
}

Jet::Jet(const Jet& copy){
  p4 = copy.P4();
  maptotree = copy.IndexInTree();
  id = copy.IDMap();

  type=copy.Type();
  bJetDisc=copy.BJetDisc();
  jECorrFactor=copy.JECorrFactor();
  jECorrUncertainty=copy.JECorrUncertainty();
  isMatched=copy.IsMatched();
  matchedGenJetIndex=copy.MatchedGenJetIndex();
  partonFlavor=copy.PartonFlavor();
  matchedGenJet=copy.MatchedGenJet();
}

Jet::~Jet(){}

//=======GET METHODS=============//

string Jet::Type()const {return type;};

map< string, map< string, double> > Jet::GetbJetWP() const {return bJetWP;}
map< string, double>  Jet::BJetDisc() const {return bJetDisc;}
double Jet::BJetDisc(const string& key) const {return bJetDisc.at(key);}
bool Jet::IsBJet(const string& key, const string& WP) const{
  map<string, map<string, double> >::const_iterator itKey = bJetWP.find(key);
  if (itKey != bJetWP.end()) {
    map<string, double>::const_iterator itWP = itKey->second.find(WP);
    if (itWP != itKey->second.end()) return IsBJet(key, itWP->second);
  }
  std::cout<<"BTagging WP not set!"<<std::endl;
  std::cout<<"using the key "<<key<<"of size"<<sizeof(key)<<endl;
  std::cout<<"WP given was "<<WP<<endl;
  std::cout<<(key=="CSV")<<" "<<(WP=="Medium")<<std::endl;
  return false;
}
bool Jet::IsBJet(const string& key, const double& disc_cut) const {
  map<string, double>::const_iterator itKey = bJetDisc.find(key);
  if (itKey == bJetDisc.end()) {
    std::cout<<"Btagging discriminator not found!"<<std::endl;
    return false;
  }
  else {
    return (itKey->second)>disc_cut;
  }
}

map< string, double> Jet::JECorrFactor()      const {return jECorrFactor;}
map< string, double> Jet::JECorrUncertainty() const {return jECorrUncertainty;}
double Jet::JECorrFactor( const string& corr)      const {return jECorrFactor.at(corr);}
double Jet::JECorrUncertainty( const string& corr) const {return jECorrUncertainty.at(corr);}

bool     Jet::IsMatched()           const {return isMatched;}
int      Jet::MatchedGenJetIndex()  const {return matchedGenJetIndex;}
int      Jet::PartonFlavor()        const {return partonFlavor;}
LorentzM Jet::MatchedGenJetP4()     const {return matchedGenJet->P4();}
const GenJet*  Jet::MatchedGenJet() const {return matchedGenJet;}

//=======SET METHODS=============//

void Jet::SetType(const string& type_In){ type=type_In;}

void Jet::SetBJetDisc(const map<string, double>& bJetDisc_In) { bJetDisc=bJetDisc_In;}
void Jet::SetBJetDisc(const string& key, const double& value){  bJetDisc[key]=value;}
void Jet::SetWP(const string& cme){
  bJetWP.clear();

  if(cme=="8TeV"){
    cout << "simpleJet::SetWP >> Setting WP " << cme << endl;

    (bJetWP)["TCHP"]["Tight"] =3.41;

    (bJetWP)["CSV"]["Loose"] =0.244;
    (bJetWP)["CSV"]["Medium"]=0.679;
    (bJetWP)["CSV"]["Tight"] =0.898;

    (bJetWP)["JP"]["Loose"] =0.275;
    (bJetWP)["JP"]["Medium"]=0.545;
    (bJetWP)["JP"]["Tight"] =0.790;

  }
  else if(cme=="7TeV"){
    cout << "simpleJet::SetWP >> Setting WP " << cme << endl;

    (bJetWP)["TCHE"]["Loose"] = 1.7;
    (bJetWP)["TCHE"]["Medium"]= 3.3;
    (bJetWP)["TCHE"]["Tight"] =10.2;

    (bJetWP)["TCHP"]["Loose"] =1.19;
    (bJetWP)["TCHP"]["Medium"]=1.93;
    (bJetWP)["TCHP"]["Tight"] =3.41;

    (bJetWP)["CSV"]["Loose"] =0.244;
    (bJetWP)["CSV"]["Medium"]=0.679;
    (bJetWP)["CSV"]["Tight"] =0.898;

    (bJetWP)["JP"]["Loose"] =0.275;
    (bJetWP)["JP"]["Medium"]=0.545;
    (bJetWP)["JP"]["Tight"] =0.790;
  }
  else std::cout<<"Jet::SetWP >> ERROR : BTagging WP table not set!"<<std::endl;
}

void Jet::SetJECorrFactor(const map<string, double>& jECorrFactor_In) { jECorrFactor=jECorrFactor_In;}
void Jet::SetJECorrUncertainty(const map<string, double>& jECorrUncertainty_In) { jECorrUncertainty=jECorrUncertainty_In;}
void Jet::SetJECorrFactor(const string& key, const double& value){  jECorrFactor[key]=value;}
void Jet::SetJECorrUncertainty(const string& key, const double& value){  jECorrUncertainty[key]=value;}

void Jet::SetIsMatched(const bool& isMatched_In){ isMatched=isMatched_In;}
void Jet::SetMatchedGenJetIndex(const int& matchedGenJetIndex_In){ matchedGenJetIndex=matchedGenJetIndex_In;}
void Jet::SetPartonFlavor( const int& partonFlavor_In){ partonFlavor=partonFlavor_In;}
void Jet::SetMatchedGenJet( const GenJet* matchedGenJet_In){ matchedGenJet=matchedGenJet_In;}

void Jet::Set(const int maptotree_In, const LorentzM& pmomuntum_In, const double& scaleCorrFactor_In, const string& type_In){
  
  AnalysisObject::Set(maptotree_In, pmomuntum_In);

  //SET SCALE CORRECTION FACTOR
  SetJECorrFactor( "JES", scaleCorrFactor_In);

  //SET THE TYPE
  type=type_In;
}
