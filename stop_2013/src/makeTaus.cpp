#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "TDirectory.h"
#include "THTools.h"
#include "makeTaus.h"
#include "Tools.h"

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

//======================================================
// make All Taus
//======================================================

vector<Tau> makeAllTaus(EasyChain* tree, CutSet* flow_in){
  
  if(pcp) cout<<endl<<"inside makeAllTaus "<<endl<<endl;
 
  //====================================================================
  // DEFINE CUT FLOW
  //====================================================================
  CUTFLOW( TString("All"), TString("Taus"), flow_in)
  //=end=initialize=cut=flow============================================

  //====================================================================
  // DEFINE CUTS
  //====================================================================
  ConfigReader config;

  static float PTMIN  = config.getFloat( "AllTaus_PTMIN",  10. );
  //=end=define=cuts====================================================

  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  bool OK = false;

  vector<Tau>       allTaus;
  Tau               dummyTau;
  vector<LorentzM>& taus                  = tree->Get( &taus, "DESYtauP4Pat");
  vector<int>&      charge                = tree->Get( &charge,"DESYtauChargePat");
  vector<float>&    idDecayModeFinding    = tree->Get( &idDecayModeFinding,"DESYtauTauIddecayModeFindingPat");
  vector<float>&    idMediumIsolationMVA2 = tree->Get( &idMediumIsolationMVA2,"DESYtauTauIdbyMediumIsolationMVA2Pat");
  //=end=variables=initialization=======================================

  //=============================================
  // LOOP OVER THE TAUS
  //=============================================
  for (int itau=0;itau<(int)taus.size();++itau){

    if(pcp){
      cout<<"Tau "<<itau<<": ";
      cout<<"Pt = "<<taus.at(itau).Pt()<<";";
      cout<<"Eta = "<<taus.at(itau).Eta()<<";";
      cout<<"Phi = "<<taus.at(itau).Phi()<<".";
    }
    
    OK=taus.at(itau).pt() > PTMIN;
    if (!flow->keepIf("tau_All_PTMIN",OK) ) continue;
    //
    OK=charge.at(itau)!=0;
    if (!flow->keepIf("tau_All_CHARGE",OK) ) continue;

    dummyTau.Set( itau, taus.at(itau), charge.at(itau), -1.);
    dummyTau.SetID( "DecayModeFinding", idDecayModeFinding.at(itau) > 0.5);
    dummyTau.SetID( "MediumIsolationMVA2", idMediumIsolationMVA2.at(itau) > 0.5);  

    allTaus.push_back(dummyTau);
  }
  //=end=loop=over=taus=================================================

  return allTaus;
}

//======================================================
// make Clean Taus
//======================================================

void makeCleanTaus(vector<Tau>& allTaus, vector<Tau*>& cleanTaus, vector<Muon*>& muons, vector<Electron*>& electrons, CutSet* flow_in){
  
  if(pcp) cout<<endl<<"inside makeCleanTaus"<<endl<<endl;
  cleanTaus.clear();

  //====================================================================
  // DEFINE CUT FLOW
  //====================================================================
  CUTFLOW( TString("Clean"), TString("Taus"), flow_in)
  //=end=initialize=cut=flow============================================

  //====================================================================
  // DEFINE CUTS
  //====================================================================
  ConfigReader config;

  static float  DRmu  =  config.getFloat("CleanTaus_DRmu",  0.4 );
  static float  DRel  =  config.getFloat("CleanTaus_DRel",  0.4 );
  //=end=define=cuts====================================================

  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  bool OK = false;
  //=end=variables=initialization=======================================

  //=============================================
  // LOOP OVER THE TAUS
  //=============================================
  for(int itau=0; itau < (int)allTaus.size(); ++itau){
    
    if(pcp)cout<<"inside the tau loop at iteration "<<itau<<endl;

    allTaus.at(itau).SetID( "Clean", false);
    
    OK=!desy_tools::CleaningByDR( allTaus.at(itau).P4(), muons, DRmu);
    if(!flow->keepIf("tau_Clean_wrtMuons",OK)) continue;
    //
    OK=!desy_tools::CleaningByDR( allTaus.at(itau).P4(), electrons, DRel);
    if(!flow->keepIf("tau_Clean_wrtElectrons",OK)) continue;

    allTaus.at(itau).SetID( "Clean", true);
    cleanTaus.push_back( &allTaus.at(itau));
  }//=end=loop=over=taus============================================

  return;
}

//======================================================
// make Veto Taus
//======================================================

bool makeVetoTaus( EasyChain* tree, vector<Tau>& allTaus,vector<Tau*>& vetoTaus, const Particle* selectedLepton, CutSet* flow_in){

  if(pcp)cout<<"inside makeVetoTaus"<<endl;

  //====================================================================
  // DEFINE CUT FLOW
  //====================================================================
  CUTFLOW( TString("Veto"), TString("Taus"), flow_in)
  //=end=initialize=cut=flow============================================

  //====================================================================
  // DEFINE CUTS
  //====================================================================
  ConfigReader config;

  static float PTMIN  = config.getFloat(  "VetoTaus_PTMIN", 20. );
  //=end=define=cuts====================================================

  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  bool OK = false;
  //=end=variables=initialization=======================================

  //=============================================
  // LOOP OVER THE TAUS
  //=============================================
  for(int itau=0;itau<allTaus.size();++itau){

    if(pcp)cout<<"inside the tau loop at iteration "<<itau<<endl;

    allTaus.at(itau).SetID( "Veto", false);
    int indx=allTaus.at(itau).IndexInTree();
    
    OK=allTaus.at(itau).Is("Clean");
    if (!flow->keepIf("tau_Veto_isClean",OK)) continue;
    //
    OK=allTaus.at(itau).pt() >= PTMIN;
    if (!flow->keepIf("tau_Veto_PTMIN",OK)) continue;
    //
    OK=allTaus.at(itau).Is("DecayModeFinding");
    if (!flow->keepIf("tau_Veto_DeacayModeFinding",OK)) continue;
    //
    OK=allTaus.at(itau).Is("MediumIsolationMVA2");
    if (!flow->keepIf("tau_Veto_MediumIsolationMVA2",OK)) continue;
    //
    if ( selectedLepton != 0){
      OK=allTaus.at(itau).Charge() != selectedLepton->Charge();
      if(!flow->keepIf("tau_Veto_OppositeChargeWrtSelectedLepton",OK)) continue;
    }

    allTaus.at(itau).SetID( "Veto", true);
    vetoTaus.push_back(&allTaus.at(itau));
  }//=end=loop=over=taus============================================

  return true;
}

#undef CUTFLOW
