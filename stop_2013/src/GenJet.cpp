#include "GenJet.h"
#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

//ClassImp(GenJet);

//=======CONSTRUCTORS=============//

GenJet::GenJet(){
  type="";

  isMatched=false;
  matchedJetIndex=-1;
  matchedJet=0;
}

GenJet::GenJet(const GenJet& copy){
  //AnalysisObject( (AnalysisObject) copy);

  p4 = copy.P4();
  maptotree = copy.IndexInTree();
  id = copy.IDMap();

  type=copy.Type();

  isMatched=copy.IsMatched();
  if (isMatched) matchedJetIndex=copy.MatchedJetIndex();
  matchedJet=copy.MatchedJet();
}

GenJet::~GenJet(){}

//=======GET METHODS=============//

string GenJet::Type()   const {return type;}

bool     GenJet::IsMatched()       const {return isMatched;}
int      GenJet::MatchedJetIndex() const {return matchedJet->IndexInTree();}
LorentzM GenJet::MatchedJetP4()    const {return matchedJet->P4();}
const Jet*     GenJet::MatchedJet()      const {return matchedJet;}

//=======SET METHODS=============//

void GenJet::SetType(const string& type_In) {type=type_In;}

void GenJet::SetIsMatched(const bool& isMatched_In) {isMatched=isMatched_In;}
void GenJet::SetMatchedJetIndex(const int& matchedJetIndex_In) {matchedJetIndex=matchedJetIndex_In; }
void GenJet::SetMatchedJet( const Jet* matchedJet_In) { matchedJet=matchedJet_In;}

void GenJet::Set(const int maptotree_In, const LorentzM& pmomuntum_In, const string& type_In){
  
  AnalysisObject::Set(maptotree_In, pmomuntum_In);

  SetType( type_In);
}

//======================================================
// match Jets and GenJets
//======================================================

void desy_tools::matchGenJets( vector<Jet>& jets, vector<GenJet>& genJets){
  
  vector<Jet*> jetsPointer;
  vector<GenJet*> genJetsPointer;

  for (int ijet=0; ijet<jets.size(); ++ijet)
    jetsPointer.push_back( &jets.at(ijet));
  
  for (int igenjet=0; igenjet<genJets.size(); ++igenjet)
    genJetsPointer.push_back( &genJets.at(igenjet));

  matchGenJets( jetsPointer, genJetsPointer);
}

void desy_tools::matchGenJets( vector<Jet*>& jets, vector<GenJet*>& genJets){
  for (int ijet=0; ijet<jets.size(); ++ijet){
    int matchedGenJetIndx = jets.at(ijet)->MatchedGenJetIndex();

    for ( int igenjet=0; igenjet < genJets.size(); ++igenjet){
      int genJetIndx = genJets.at( igenjet)->IndexInTree();

      if ( genJetIndx == matchedGenJetIndx ){

	jets.at(ijet)->SetIsMatched( true);
	jets.at(ijet)->SetMatchedGenJet( genJets.at( igenjet));

	genJets.at( igenjet)->SetIsMatched( true);
	genJets.at( igenjet)->SetMatchedJet( jets.at(ijet));

	break;
      }
    }
  }
}
