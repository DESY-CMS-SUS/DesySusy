#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "TDirectory.h"
#include "THTools.h"
#include "Electron.h"
#include "Muon.h"
#include "Jet.h"
#include "makeJets.h"
#include "BTagReshaping/BTagReshaping.h"

#include "Math/VectorUtil.h"

#ifndef CUTFLOW
#define CUTFLOW( cut, obj, flow_in)                 \
  static CutSet newFlow( cut+"_"+obj+"_Selection"); \
  newFlow.autodump=true;                            \
  CutSet* flow = &newFlow;                          \
  if (flow_in) {                                    \
    newFlow.autodump = false;                       \
    flow = flow_in;                                 \
  }
#endif

using namespace std;

extern bool pcp;
extern BTagShapeInterface reshape;
//======================================================
// make All Jets
//======================================================

vector<Jet> makeAllJets(EasyChain* tree, CutSet* flow_in){

  if(pcp) cout<<endl<<"inside makeAllJets "<<endl<<endl;

  //====================================================================
  // DEFINE CUT FLOW
  //====================================================================
  CUTFLOW( TString("All"), TString("Jets"), flow_in)
  //=end=initialize=cut=flow============================================

  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  ConfigReader config;
  bool isData=config.getBool("isData");

  vector<Jet>        jets; jets.clear();
  Jet                dummyJet;
  vector<LorentzM>&  jets_p4         = tree->Get( &jets_p4,         "ak5JetPFCorrectedP4Pat");
  vector<float>&     jets_corrFactor = tree->Get( &jets_corrFactor, "ak5JetPFCorrFactorPat");
  vector<float>&     jets_corrUnc    = tree->Get( &jets_corrUnc,    "DESYak5JetPFJECuncertaintyPat");
  vector<float>&     jets_CSV        = tree->Get( &jets_CSV,        "ak5JetPFCombinedSecondaryVertexBJetTagsPat");
  vector<int>&       jets_IDLoose    = tree->Get( &jets_IDLoose,    "ak5JetPFPFJetIDloosePat");
  vector<int>&       jets_IDTight    = tree->Get( &jets_IDTight,    "ak5JetPFPFJetIDtightPat");
  vector<int>&       jets_PULoose    = tree->Get( &jets_PULoose,    "DESYak5JetPFPileUpIdfull5xLoosePat");
  vector<int>&       jets_PUMedium   = tree->Get( &jets_PUMedium,   "DESYak5JetPFPileUpIdfull5xMediumPat");
  vector<int>&       jets_PUTight    = tree->Get( &jets_PUTight,    "DESYak5JetPFPileUpIdfull5xTightPat");
  //=end=variables=initialization=======================================

  if( jets_p4.size() != jets_corrUnc.size()){
    cout<<"makeAlJets: jets_p4.size() != jets_corrUnc.size(), "<<jets_p4.size()<<" != "<<jets_corrUnc.size()<<endl;
    return jets;
  }

  //=============================================
  // LOOP OVER THE JETS
  //=============================================
  for(int ijet = 0; ijet<jets_p4.size(); ijet++){

    if(pcp){
      cout<<"Jet "<<ijet<<": ";
      cout<<"Pt = "<<jets_p4.at(ijet).Pt()<<";";
      cout<<"Eta = "<<jets_p4.at(ijet).Eta()<<";";
      cout<<"Phi = "<<jets_p4.at(ijet).Phi()<<"."<<endl;;
    }
    dummyJet.Set( ijet, jets_p4.at(ijet), jets_corrFactor.at(ijet), "AK5");
    dummyJet.SetJECorrUncertainty( "JES", jets_corrUnc.at(ijet));
    dummyJet.SetBJetDisc("CSV", jets_CSV.at(ijet));

    dummyJet.SetID("Loose", jets_IDLoose.at(ijet));
    dummyJet.SetID("Tight", jets_IDTight.at(ijet));

    dummyJet.SetID("PULoose", jets_PULoose.at(ijet));
    dummyJet.SetID("PUMedium",jets_PUMedium.at(ijet));
    dummyJet.SetID("PUTight", jets_PUTight.at(ijet));

    jets.push_back( dummyJet);
  }
  //=end=loop=over=jets============================================
  
  if (!isData) {
    vector<int>& jets_genIndx    = tree->Get( &jets_genIndx,    "ak5JetPFGenJetMatchIndexPat");
    vector<int>& jets_genFlavor  = tree->Get( &jets_genFlavor,  "ak5JetPFgenJetFlavourPat");
  
    double bdisc_old, bdisc_new;
    for(int ijet = 0; ijet<jets.size(); ijet++){
      int indx = jets.at(ijet).IndexInTree();
      jets.at(ijet).SetMatchedGenJetIndex( jets_genIndx.at(indx)); 
      jets.at(ijet).SetPartonFlavor( jets_genFlavor.at(indx));

      bdisc_old = jets.at(ijet).BJetDisc("CSV");
      bdisc_new = reshape.reshape( jets.at(ijet).Eta(),
				   jets.at(ijet).Pt(),
				   bdisc_old,
				   jets.at(ijet).PartonFlavor());
      jets.at(ijet).SetBJetDisc("CSV", bdisc_new);
      jets.at(ijet).SetBJetDisc("CSV_Reshape", bdisc_new);
      jets.at(ijet).SetBJetDisc("CSV_NoReshape", bdisc_old);
    }
  }
  
  for(int ijet = 0; ijet<jets_p4.size(); ijet++)
    if(pcp) cout<<"pt,eta, phi and id -->"<<jets.at(ijet).pt()<<" "<<jets.at(ijet).eta()<<" "<<jets.at(ijet).Phi()<<" "<<jets.at(ijet).Is("Loose")<<endl;

  return jets;
}

//======================================================
// make Clean Jets
//======================================================

void makeCleanJets(vector<Jet>& allJets, vector<Jet*>& cleanJets, vector<Muon*>& muons, vector<Electron*>& electrons, CutSet* flow_in){
  
  if(pcp) cout<<endl<<"inside makeCleanJets"<<endl<<endl;
  cleanJets.clear();

  //====================================================================
  // DEFINE CUT FLOW
  //====================================================================
  CUTFLOW( TString("Clean"), TString("Jets"), flow_in)
  //=end=initialize=cut=flow============================================

  //====================================================================
  // DEFINE THE CUTS
  //====================================================================
  ConfigReader config;

  static float  DRmu  =  config.getFloat("CleanJets_DRmu",  0.4 );
  static float  DRel  =  config.getFloat("CleanJets_DRel",  0.4 );
  //=end=define=cuts====================================================

  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  bool OK = false;
  //=end=variables=initialization=======================================

  //=============================================
  // LOOP OVER THE JETS
  //=============================================
  for(int ijet=0; ijet < (int)allJets.size(); ++ijet){
    
    if(pcp)cout<<"inside the jet loop at iteration "<<ijet<<endl;

    allJets.at(ijet).SetID( "Clean", false);
    
    OK=!desy_tools::CleaningByDR( allJets.at(ijet).P4(), muons, DRmu);
    if(!flow->keepIf("jets_Clean_wrtMuons",OK)) continue;
    //
    OK=!desy_tools::CleaningByDR( allJets.at(ijet).P4(), electrons, DRel);
    if(!flow->keepIf("jets_Clean_wrtElectrons",OK)) continue;

    allJets.at(ijet).SetID( "Clean", true);
    cleanJets.push_back( &allJets.at(ijet));
  }//=end=loop=over=jets============================================

  return;
}

//======================================================
// make Good Jets
//======================================================

void makeGoodJets(EasyChain* tree, vector<Jet>& allJets, vector<Jet*>& goodJets, CutSet* flow_in){

  if(pcp) cout<<"indide makeGoodJets "<<endl;
  goodJets.clear();

  //====================================================================
  // DEFINE CUT FLOW
  //====================================================================
  CUTFLOW( TString("Good"), TString("Jets"), flow_in)
  //=end=initialize=cut=flow============================================

  //====================================================================
  // DEFINE CUTS
  //====================================================================
  ConfigReader config;

  static float  PTMIN  =  config.getFloat("GoodJets_PTMIN",  10. );
  static float  ETAMAX  = config.getFloat("GoodJets_ETAMAX", 4.7 );
   //=end=define=cuts===================================================

  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  bool OK = false;
  //=end=variables=initialization=======================================

  //=============================================
  // LOOP OVER THE JETS
  //=============================================
  for(int ijet = 0; (int)ijet<allJets.size(); ijet++){
    
    OK = false;
    int indx=allJets.at(ijet).IndexInTree();
 
    allJets.at(ijet).SetID("Good",0);

    OK=allJets.at(ijet).pt() > PTMIN;
    if( !flow->keepIf("GoodJets_PTMIN", OK)) continue;
    // 
    OK=fabs(allJets.at(ijet).eta()) < ETAMAX;
    if( !flow->keepIf("GoodJets_ETAMAX",OK)) continue;
    //
    OK=allJets.at(ijet).Is("Loose");
    if( !flow->keepIf("ak5JetPFPFJetIDloosePat", OK )) continue;
    //
    OK=allJets.at(ijet).Is("Clean");
    if( !flow->keepIf("GoodJets_Clean", OK)) continue;

    allJets.at(ijet).SetID("Good",1);
    goodJets.push_back( &allJets.at(ijet));
  }//=end=loop=over=jets============================================

  return;
};


//======================================================
// make Selected Jets
//======================================================

void makeSelectedJets(EasyChain* tree, vector<Jet>& allJets, vector<Jet*>& selectedJets, CutSet* flow_in){

  if(pcp) cout<<"inside makeSelectedJets "<<endl;
  selectedJets.clear();

  //====================================================================
  // DEFINE CUT FLOW
  //====================================================================
  CUTFLOW( TString("Selected"), TString("Jets"), flow_in)
  //=end=initialize=cut=flow============================================

  //====================================================================
  // DEFINE CUTS
  //====================================================================
  ConfigReader config;

  static float  PTMIN  =  config.getFloat("SelectedJets_PTMIN",  30. );
  static float  ETAMAX  = config.getFloat("SelectedJets_ETAMAX", 2.4 );
  //=end=define=cuts====================================================

  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  bool OK = false;
  //=end=variables=initialization=======================================

  //=============================================
  // LOOP OVER THE JETS
  //=============================================
  for(int ijet = 0; (int)ijet<allJets.size(); ijet++){

    OK=false;
    allJets.at(ijet).SetID("Selected",false);

    OK=allJets.at(ijet).Is("Good");
    if (!flow->keepIf("is Good",OK) ) continue;
    //
    OK=allJets.at(ijet).Is("Clean");
    if( !flow->keepIf("Jets_Clean", OK)) continue;
    //
    OK=allJets.at(ijet).Is("PUTight");
    if( !flow->keepIf("Jets_Pileup", OK)) continue;
    //
    OK=allJets.at(ijet).pt() > PTMIN;
    if( !flow->keepIf("Jets_PTMIN", OK)) continue;
    // 
    OK=fabs(allJets.at(ijet).eta()) < ETAMAX;
    if( !flow->keepIf("jets_ETAMAX",OK)) continue;

    allJets.at(ijet).SetID("Selected",true);
    selectedJets.push_back( &allJets.at(ijet));
  }//=end=loop=over=jets============================================

  return;
};

//======================================================
// make All Gen Jets
//======================================================

vector<GenJet> makeAllGenJets( EasyChain* tree, CutSet* flow_in){

  if(pcp) cout<<endl<<"inside makeAllGenJets "<<endl<<endl;

  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  ConfigReader config;
  bool isData=config.getBool("isData");

  vector<GenJet>     genJets;
  GenJet             dummyGenJet;
  vector<LorentzM>&  p4     = tree->Get( &p4,     "genak5GenJetsP4");
  //=end=variables=initialization=======================================

  if ( isData) return genJets;

  //=============================================
  // LOOP OVER THE GEN JETS
  //=============================================
  for(int ijet = 0; ijet<p4.size(); ijet++){
  
    if(pcp){
      cout<<"GenJet "<<ijet<<": ";
      cout<<"Pt = "<<p4.at(ijet).Pt()<<";";
      cout<<"Eta = "<<p4.at(ijet).Eta()<<";";
      cout<<"Phi = "<<p4.at(ijet).Phi()<<".";
    }

    dummyGenJet.Set( ijet, p4.at(ijet), "AK5"); 
    genJets.push_back(dummyGenJet);
  }//=end=loop=over=gen=jets========================================

  return genJets;
}
#undef CUTFLOW
