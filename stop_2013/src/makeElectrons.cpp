#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "TDirectory.h"
#include "THTools.h"
#include "eventselection.h"
#include "Electron.h"
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
// make All Electrons
//======================================================

vector<Electron> makeAllElectrons(EasyChain* tree, CutSet* flow_in){

  if(pcp)cout<<"inside makeAllElectrons"<<endl;

  //====================================================================
  // DEFINE CUT FLOW
  //====================================================================
  CUTFLOW( TString("All"), TString("Electrons"), flow_in)
  //=end=initialize=cut=flow============================================

  //====================================================================
  // DEFINE CUTS
  //====================================================================
  ConfigReader config;

  static float PTMIN  = config.getFloat( "AllElectrons_PTMIN",  10. );
  static float ETAMAX = config.getFloat( "AllElectrons_ETAMAX",  2.5);
  //=end=define=cuts====================================================

  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  bool OK = false;

  vector<Electron>  allElectrons;
  Electron          dummyElectron;
  vector<LorentzM>& electrons  = tree->Get( &electrons,  "electronP4Pat");
  vector<int>&      charge     = tree->Get( &charge,     "electronChargePat");
  vector<float>&    dz         = tree->Get( &dz,         "electronGsfTrackDzPat");
  vector<float>&    relIso     = tree->Get( &relIso,     "DESYelectronCorrectedRelIsolationPat");
  vector<int>&      isLoose    = tree->Get( &isLoose,    "electronIdLoosePat");
  vector<int>&      isMedium   = tree->Get( &isMedium,   "electronIdMediumPat");
  vector<int>&      isTight    = tree->Get( &isTight,    "electronIdTightPat");  
  //=end=variables=initialization=======================================

  //=============================================
  // LOOP OVER THE ELECTRONS                       
  //=============================================
  for (int iel=0;iel<(int)electrons.size();++iel){

    if(pcp){
      cout<<"Electron "<<iel<<": ";
      cout<<"Pt = "<<electrons.at(iel).Pt()<<";";
      cout<<"Eta = "<<electrons.at(iel).Eta()<<";";
      cout<<"Phi = "<<electrons.at(iel).Phi()<<".";
    }

    OK=electrons.at(iel).Pt() > PTMIN;
    if (!flow->keepIf("el_All_PTMIN",OK) ) continue;
    //
    OK=fabs(electrons.at(iel).Eta()) < ETAMAX;
    if (!flow->keepIf("el_All_ETAMAX",OK) ) continue;

    dummyElectron.Set( iel, electrons.at(iel), charge.at(iel), relIso.at(iel)*electrons.at(iel).Pt(), dz.at(iel));
    dummyElectron.SetID( "Loose", isLoose.at(iel));
    dummyElectron.SetID( "Medium", isMedium.at(iel));   
    dummyElectron.SetID( "Tight", isTight.at(iel));   

    allElectrons.push_back(dummyElectron);
  }
  //=end=loop=over=electrons============================================

  return allElectrons;
}

//======================================================
// make Clean Electrons
//======================================================

bool makeCleanElectrons( vector<Electron>& allElectrons, vector<Electron*>& cleanElectrons, vector<Muon*>& muons, CutSet* flow_in){
  
  if(pcp)cout<<"inside makeCleanElectrons"<<endl;
  cleanElectrons.clear();
  
  //====================================================================
  // DEFINE CUT FLOW
  //====================================================================
  CUTFLOW( TString("Clean"), TString("Electrons"), flow_in)
  //=end=initialize=cut=flow============================================

  //====================================================================
  // DEFINE CUTS
  //====================================================================
  ConfigReader config;

  static float  DR  =  config.getFloat("CleanElectrons_DRmu",  0.1 );
  //=end=define=cuts====================================================

  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  bool OK = false;
  //=end=variables=initialization=======================================

  //=============================================
  // LOOP OVER THE ELECTRONS                   
  //=============================================
  for(int iel=0;iel<(int)allElectrons.size();++iel){
    
    if(pcp)cout<<"inside the electron loop at iteration "<<iel<<endl;

    allElectrons.at(iel).SetID( "Clean", false);
    
    OK=!desy_tools::CleaningByDR( allElectrons.at(iel).P4(), muons, DR);
    if( !flow->keepIf("el_Clean_wrtMuons",OK)) continue;
    
    allElectrons.at(iel).SetID( "Clean", true);
    cleanElectrons.push_back(&allElectrons.at(iel));
  }
  //=end=loop=over=electrons============================================


  return true;
}

//======================================================
// make Good Electrons
//======================================================

bool makeGoodElectrons(EasyChain* tree, vector<Electron>& allElectrons, vector<Electron*>& goodElectrons, CutSet * flow_in){

  if(pcp)cout<<"inside makeGoodElectrons"<<endl;
  goodElectrons.clear();

  //====================================================================
  // DEFINE CUT FLOW
  //====================================================================
  CUTFLOW( TString("Good"), TString("Electrons"), flow_in)
  //=end=initialize=cut=flow============================================

  //====================================================================
  // DEFINE CUTS
  //====================================================================
  ConfigReader config;

  static float  PTMIN      = config.getFloat(  "GoodElectrons_PTMIN",     10. );
  static float  ETAMAX     = config.getFloat(  "GoodElectrons_ETAMAX",     2.5 );
  static string ID         = config.getString( "GoodElectron_ID",         "Medium");
  static float trackdxyMAX = config.getFloat(  "GoodElectron_trackdxyMAX", 0.02);
  static float trackdzMAX  = config.getFloat(  "GoodElectron_trackdzMAX",  0.1);
  //=end=define=cuts====================================================

  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  if(pcp)cout<<"check point about to get the electrons"<<endl;

  bool OK=false;
  vector<float>&    superClusterPositionETA   = tree->Get( &superClusterPositionETA, "electronESuperClusterEtaPat");

  bool TTver=false;
  const char* idname="undefined";
  if(ID=="Tight"){
    idname="electronIdTightPat";
  }else if(ID=="Medium"){
    idname="electronIdMediumPat";
  }else if(ID=="TriggerTight"){
    idname="DESYelectronIdTriggerTightPat";
    TTver=true;
  }else if(ID=="Loose"){
    idname="electronIdLoosePat";
  }

  else{
    cout<<"ID NOT SET. ERROR"<<endl;
  }
  vector<int>&   id                  = tree->Get( &id,                  idname);
  vector<int>&   conversionRejection = tree->Get( &conversionRejection, "electronConversionRejectionPat");
  vector<float>& gsfTrackDxy         = tree->Get( &gsfTrackDxy,         "electronGsfTrackDxyPat");
  vector<float>& gsfTrackDz          = tree->Get( &gsfTrackDz,          "electronGsfTrackDzPat");
  vector<bool>&  isEBPat             = tree->Get( &isEBPat,             "DESYelectronIsEBPat");
  vector<bool>&  isEEPat             = tree->Get( &isEEPat,             "DESYelectronIsEEPat");
  //=end=variables=initialization=======================================

  //=============================================
  // LOOP OVER THE ELECTRONS                   
  //=============================================
  for(int iel=0;iel<(int)allElectrons.size();++iel){
    
    if(pcp)cout<<"inside the electron loop at iteration "<<iel<<endl;

    allElectrons.at(iel).SetID( "Good", false);
    int indx=allElectrons.at(iel).IndexInTree();

    OK=allElectrons.at(iel).Is("Clean");
    if (!flow->keepIf("is Clean",OK) )continue;
    //
    OK=allElectrons.at(iel).pt() >= PTMIN;
    if(!flow->keepIf("pt>el_pt_min_low GOOD",OK)) continue;
    //
    OK=fabs(allElectrons.at(iel).Eta())<=ETAMAX;
    if(!flow->keepIf("abs(eta)<etamax GOOD",OK))continue;
    //
    OK=fabs(superClusterPositionETA.at(indx))<1.4442 || fabs(superClusterPositionETA.at(indx))>1.566;
    if(!flow->keepIf("notinetagap GOOD",OK))continue;  
    //
    OK=id.at(indx);
    if(!flow->keepIf("Electron_ID GOOD",OK)) continue;
    //
    OK=!conversionRejection.at(indx);
    if(!flow->keepIf("Electron_ConversionRejection GOOD",OK)) continue;
    //
    OK=fabs(gsfTrackDxy.at(indx)) < trackdxyMAX ;
    if( !flow->keepIf("dxy to vertex position", OK)) continue;
    //
    OK=fabs(gsfTrackDz.at(indx)) < trackdzMAX;
    if( !flow->keepIf("dz to vertex position"  , OK)) continue;
    //
    OK=allElectrons.at(iel).RelIso() < 0.15;
    if( fabs(allElectrons.at(iel).eta())<1.4442 && allElectrons.at(iel).pt() < 20)
      OK=allElectrons.at(iel).RelIso() < 0.1;	
    if(!flow->keepIf("Relative PF Isolation corrected max",OK)) continue;
    //

    allElectrons.at(iel).SetID( "Good", true);
    goodElectrons.push_back(&allElectrons.at(iel));

  }
  if(pcp)cout<<"out of the electron loop"<<endl;
  //=end=loop=over=electrons============================================

  return true;

};

//======================================================
// make Selected Electrons
//======================================================

bool makeSelectedElectrons(EasyChain* tree, vector<Electron>& allElectrons,vector<Electron*>& selectedElectrons, CutSet* flow_in){

  if(pcp)cout<<"inside makeSelectedElectrons"<<endl;

  //====================================================================
  // DEFINE CUT FLOW
  //====================================================================
  CUTFLOW( TString("Selected"), TString("Electrons"), flow_in)
  //=end=initialize=cut=flow============================================

  //====================================================================
  // DEFINE CUTS
  //====================================================================
  ConfigReader config;

  static float PTMIN          = config.getFloat( "SelectedElectrons_PTMIN",          30. );
  static float ETAMAX         = config.getFloat( "SelectedElectrons_ETAMAX",          1.4442);
  static float PFAbsIsoMAX    = config.getFloat( "SelectedElectrons_PFAbsIso_MAX",    5);
  static float EOverPinMAX    = config.getFloat( "SelectedElectrons_EOverPin_MAX",    4);
  static float PFRECO_MAXDIFF = config.getFloat( "SelectedElectrons_PFRECO_MAXDIFF", 10.0);
  //=end=define=cuts====================================================

  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  bool OK=false;

  vector<float>& ESuperClusterOverP = tree->Get( &ESuperClusterOverP, "electronESuperClusterOverPPat"      );
  //=end=variables=initialization=======================================

  //=============================================
  // LOOP OVER THE ELECTRONS                   
  //=============================================
  for(int iel=0;iel<allElectrons.size();++iel){

    if(pcp)cout<<"inside the electron loop at iteration "<<iel<<endl;

    allElectrons.at(iel).SetID( "Selected", false);
    int indx=allElectrons.at(iel).IndexInTree();
    
    OK=allElectrons.at(iel).Is("Good");
    if (!flow->keepIf("is Good",OK) )continue;
    //
    OK=allElectrons.at(iel).Is("Clean");
    if (!flow->keepIf("is Clean",OK) )continue;
    //
    OK=allElectrons.at(iel).pt() >= PTMIN;
    if(!flow->keepIf("pt>el_pt_min_low SELECTED",OK)) continue;
    //
    OK=fabs(allElectrons.at(iel).Eta())<=ETAMAX;
    if(!flow->keepIf("abs(eta)<etamax SELECTED",OK))continue;
    //
    OK=allElectrons.at(iel).Iso() < PFAbsIsoMAX;
    if(!flow->keepIf("Absolute PF Isolation corrected max",OK)) continue;
    //
    OK=ESuperClusterOverP.at(indx) < EOverPinMAX;
    if(!flow->keepIf("ESuperClusterOverP",OK)) continue;
    //
    OK= desy_tools::Consistency( allElectrons.at(iel).P4(), (int) allElectrons.at(iel).Charge(), tree,"electronP4PF", "electronChargePF") < PFRECO_MAXDIFF;
    if(!flow->keepIf("RecoPt-PFPt",OK)) continue;

    allElectrons.at(iel).SetID( "Selected", true);
    selectedElectrons.push_back(&allElectrons.at(iel));
  }
  if(pcp)cout<<"out of the electron loop"<<endl;
  //=end=loop=over=electrons============================================

  return true;
};

#undef CUTFLOW
