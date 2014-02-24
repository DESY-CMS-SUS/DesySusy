#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "TDirectory.h"
#include "THTools.h"
#include "Muon.h"
#include "makeMuons.h"
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
// make All Muons
//======================================================

vector<Muon> makeAllMuons(EasyChain* tree, CutSet* flow_in){
  
  if(pcp) cout<<endl<<"inside makeAllMuons "<<endl<<endl;

  //====================================================================
  // DEFINE CUT FLOW
  //====================================================================
  CUTFLOW( TString("All"), TString("Muons"), flow_in)
  //=end=initialize=cut=flow============================================

  //====================================================================
  // DEFINE CUTS
  //====================================================================
  ConfigReader config;

  static float PTMIN  = config.getFloat( "AllMuons_PTMIN",  10. );
  static float ETAMAX = config.getFloat( "AllMuons_ETAMAX",  2.4);
  //=end=define=cuts====================================================
 
  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  bool OK = false;

  vector<Muon>      allMuons;
  Muon              dummyMuon;
  vector<LorentzM>& muons     = tree->Get( &muons,     "muonP4Pat");
  vector<int>&      charge    = tree->Get( &charge,    "muonChargePat");
  vector<float>&    dz        = tree->Get( &dz,        "muonInnerTrackDzPat");
  vector<float>&    relIso    = tree->Get( &relIso,    "DESYmuonPfIsolationR03DeltaBCorrectedPat");
  vector<int>&      isPF      = tree->Get( &isPF,      "muonIsPFMuonPat");
  vector<int>&      isGlobal  = tree->Get( &isGlobal,  "muonIsGlobalMuonPat" );
  vector<int>&      isTracker = tree->Get( &isTracker, "muonIsTrackerMuonPat" );
  //=end=variables=initialization=======================================

  //=============================================
  // LOOP OVER THE MUONS                       
  //=============================================
  for (int imu=0;imu<(int)muons.size();++imu){ 

    if(pcp){
      cout<<"Muon "<<imu<<": ";
      cout<<"Pt = "<<muons.at(imu).Pt()<<";";
      cout<<"Eta = "<<muons.at(imu).Eta()<<";";
      cout<<"Phi = "<<muons.at(imu).Phi()<<".";
    }

    OK=muons.at(imu).pt() > PTMIN;
    if (!flow->keepIf("mu_All_PTMIN",OK) ) continue;
    //
    OK=fabs(muons.at(imu).Eta()) < ETAMAX;
    if (!flow->keepIf("mu_All_ETAMAX",OK) ) continue;

    dummyMuon.Set(imu, muons.at(imu), charge.at(imu), relIso.at(imu)*muons.at(imu).Pt(), dz.at(imu));
    dummyMuon.SetID( "PF", isPF.at(imu));
    dummyMuon.SetID( "Global", isGlobal.at(imu));
    dummyMuon.SetID( "Tracker", isTracker.at(imu));

    allMuons.push_back(dummyMuon);
  }
  //=end=loop=over=muons================================================

  return allMuons;
}

//======================================================
// make Good Muons
//======================================================

bool makeGoodMuons(EasyChain* tree, vector<Muon>& allMuons,vector<Muon*>& goodMuons, CutSet* flow_in){

  if(pcp)cout<<"inside makeGoodMuons"<<endl;

  //====================================================================
  // DEFINE CUT FLOW
  //====================================================================
  CUTFLOW( TString("Good"), TString("Muons"), flow_in)
  //=end=initialize=cut=flow============================================

  //====================================================================
  // DEFINE CUTS
  //====================================================================
  ConfigReader config;

  static float PTMIN                      = config.getFloat( "GoodMuons_PTMIN",                     10. );
  static float ETAMAX                     = config.getFloat( "GoodMuons_ETAMAX",                     2.4);
  static float Chi2MAX                    = config.getFloat( "GoodMuons_Chi2MAX",                   10.0);
  static int   NValidGlobalTrackerHitsMIN = config.getInt(   "GoodMuons_NValidGlobalTrackerHitsMIN", 0);
  static int   NMatchedStationsMIN        = config.getInt(   "GoodMuons_NMatchedStationsMIN",        1);
  static float dxyVertexMAX               = config.getFloat( "GoodMuons_dxyVertexMAX",               0.02);
  static float dzVertexMAX                = config.getFloat( "GoodMuons_dzVertexMAX",                0.5);
  static int   NValidPixelHitsMIN         = config.getInt(   "GoodMuons_NValidPixelHitsMIN",         0);
  static int   NTrackerLayersMIN          = config.getInt(   "GoodMuons_NTrackerLayersMIN",          5);
  static float PFRelIsoMAX                = config.getFloat( "GoodMuons_PFRelIso_MAX",               0.15);
  //=end=define=cuts====================================================

  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  bool OK=false;

  vector<float>&        normalisedchi2   = tree->Get( &normalisedchi2,   "muonGlobalTracknormalizedChi2Pat");
  vector<int>&          GTrack_nValTHits = tree->Get( &GTrack_nValTHits, "muonGlobalTracknumberOfValidHitsPat");
  vector<unsigned int>& NMatchedStations = tree->Get( &NMatchedStations, "muonNumberOfMatchedStationsPat");
  vector<float>&        ITrack_Dxy       = tree->Get( &ITrack_Dxy,       "muonInnerTrackDxyPat");
  vector<float>&        ITrack_Dz        = tree->Get( &ITrack_Dz,        "muonInnerTrackDzPat");
  vector<unsigned int>& NValidPixelHits  = tree->Get( &NValidPixelHits,  "muonNumberOfValidPixelHitsPat");
  vector<unsigned int>& NTrackerLayers   = tree->Get( &NTrackerLayers,   "muonNumberOfTrackerLayersWithMeasurementPat");
  //=end=variables=initialization=======================================

  //=============================================
  // LOOP OVER THE MUONS                       
  //=============================================
  for(int imu=0;imu<allMuons.size();++imu){

    if(pcp)cout<<"inside the muon loop at iteration "<<imu<<endl;

    allMuons.at(imu).SetID( "Good", false);
    int indx=allMuons.at(imu).IndexInTree();
    
    OK=allMuons.at(imu).pt() >= PTMIN;
    if(!flow->keepIf("pt>mu_pt_min_low GOOD",OK)) continue;
    //
    OK=fabs(allMuons.at(imu).Eta())<=ETAMAX;
    if(!flow->keepIf("abs(eta)<etamax GOOD",OK))continue;
    //
    OK=allMuons.at(imu).Is("Global");
    if(!flow->keepIf("is global",OK)) continue;    
    //
    OK=allMuons.at(imu).Is("PF");
    if(!flow->keepIf("is PF",OK)) continue;
    //
    OK=normalisedchi2.at(indx) < Chi2MAX;
    if(!flow->keepIf("normalised chi2 for GOOD muons",OK)) continue;
    //
    OK=GTrack_nValTHits.at(indx) > NValidGlobalTrackerHitsMIN;
    if(!flow->keepIf("global tracker hits for GOOD muons",OK)) continue;
    //
    OK=NMatchedStations.at(indx)>NMatchedStationsMIN;
    if(!flow->keepIf("N Matched Statios for GOOD muons",OK)) continue;
    //
    OK=fabs(ITrack_Dxy.at(indx)) < dxyVertexMAX;
    if(!flow->keepIf("dxy to vertex position",OK)) continue;
    //
    OK=fabs(ITrack_Dz.at(indx)) < dzVertexMAX;
    if(!flow->keepIf("dz to vertex position",OK)) continue;
    //
    OK=NValidPixelHits.at(indx) > NValidPixelHitsMIN;
    if(!flow->keepIf("PixelHits min",OK)) continue;
    //
    OK=NTrackerLayers.at(indx)>NTrackerLayersMIN;
    if(!flow->keepIf("NTrackerLayers min",OK)) continue;
    //
    OK=allMuons.at(imu).RelIso() < PFRelIsoMAX;
    if(!flow->keepIf("Relative PF Isolation DeltaB corrected max",OK)) continue;

    allMuons.at(imu).SetID( "Good", true);
    goodMuons.push_back(&allMuons.at(imu));

  }
  if(pcp)cout<<"out of the muon loop"<<endl;
  //=end=loop=over=muons================================================

  return true;

};

//======================================================
// make Selected Muons
//======================================================

bool makeSelectedMuons(EasyChain* tree, vector<Muon>& allMuons,vector<Muon*>& selectedMuons, CutSet* flow_in){

  if(pcp)cout<<"inside makeSelectedMuons"<<endl;

  //====================================================================
  // DEFINE CUT FLOW
  //====================================================================
  CUTFLOW( TString("Selected"), TString("Muons"), flow_in)
  //=end=initialize=cut=flow============================================

  //====================================================================
  // DEFINE CUTS
  //====================================================================
  ConfigReader config;

  static float PTMIN          = config.getFloat( "SelectedMuons_PTMIN",          20. );
  static float ETAMAX         = config.getFloat( "SelectedMuons_ETAMAX",          2.1);
  static float PFAbsIsoMAX    = config.getFloat( "SelectedMuons_PFAbsIso_MAX",    5);
  static float PFRECO_MAXDIFF = config.getFloat( "SelectedMuons_PFRECO_MAXDIFF", 10.0);
  //=end=define=cuts====================================================

  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  bool OK = false;
  //=end=variables=initialization=======================================

  //=============================================
  // LOOP OVER THE MUONS                       
  //=============================================
  for(int imu=0;imu<allMuons.size();++imu){

    if(pcp)cout<<"inside the muon loop at iteration "<<imu<<endl;

    allMuons.at(imu).SetID( "Selected", false);
    int indx=allMuons.at(imu).IndexInTree();
    
    OK=allMuons.at(imu).Is("Good");
    if (!flow->keepIf("is Good",OK) )continue;
    //
    OK=allMuons.at(imu).Pt() >= PTMIN;
    if(!flow->keepIf("pt>mu_pt_min_low SELECTED",OK)) continue;
    //
    OK=fabs(allMuons.at(imu).Eta())<=ETAMAX;
    if(!flow->keepIf("abs(eta)<etamax SELECTED",OK))continue;
    //
    OK=allMuons.at(imu).Iso() < PFAbsIsoMAX;
    if(!flow->keepIf("Absolute PF Isolation DeltaB corrected max",OK)) continue;
    //
    OK= desy_tools::Consistency( allMuons.at(imu).P4(), (int) allMuons.at(imu).Charge(), tree,"muonP4PF", "muonChargePF") < PFRECO_MAXDIFF;
    if(!flow->keepIf("RecoPt-PFPt",OK)) continue;

    allMuons.at(imu).SetID( "Selected", true);
    selectedMuons.push_back( &allMuons.at(imu));
  }
  if(pcp)cout<<"out of the muon loop"<<endl;
  //=end=loop=over=muons================================================

  return true;

};

void rescaleMUR(EasyChain* tree, vector<Muon*>&MuonsToRescale, LorentzM& metCorr, float murSF_err) {

  vector<LorentzM>& genP4 = tree->Get(&genP4, "genP4");
  vector<int>& genPdgId = tree->Get(&genPdgId, "genPdgId");

  //Loop over Muons
  for (int iMu = 0; iMu < MuonsToRescale.size(); iMu++) {

    LorentzM oldP4 = MuonsToRescale.at(iMu)->P4();

    //Match to Gen particle by minimising delta_r
    int MatchedGenParticle = -1;
    double minDeltaR = 99999999.;
    for (int iGen = 0; iGen < genP4.size() ; iGen++) {
      if (abs(genPdgId.at(iGen)) != 13 ) continue;
      float deltaR = ROOT::Math::VectorUtil::DeltaR(oldP4, genP4.at(iGen) );
      if (deltaR < minDeltaR) {
	minDeltaR = deltaR;
	MatchedGenParticle = iGen;
      }
    }

    if (minDeltaR > 0.5) {
      if (pcp) cout << "rescaleMUR >> No matched gen muon found. minDeltaR = " << minDeltaR << endl;
      continue;
    }
    else {
      if (pcp) cout << "rescaleMUR >> Matched gen muon found with minDeltaR = " << minDeltaR << endl;
    }

    if (pcp) {
      cout << "Muon P4=(" << MuonsToRescale.at(iMu)->Px() << ","  << MuonsToRescale.at(iMu)->Py() << ","  << MuonsToRescale.at(iMu)->Pz() << ","  << MuonsToRescale.at(iMu)->E() << ")" << endl;
      cout << "Matched gen muon P4=(" << genP4.at(MatchedGenParticle).Px() << ","  << genP4.at(MatchedGenParticle).Py() << ","  << genP4.at(MatchedGenParticle).Pz() << ","  << genP4.at(MatchedGenParticle).E() << ")" << endl;
    }

    //Create shift, as for jets
    float murSF = getMurSF(murSF_err);
    //float muRescale = genP4.at(MatchedGenParticle).Pt() + (murSF) * ( oldP4.Pt() - genP4.at(MatchedGenParticle).Pt() );
    //muRescale /= oldP4.Pt();
    //if (muRescale < 0.) muRescale = 0.;

    //metCorr += oldP4 * (1. - muRescale);
    //MuonsToRescale.at(iMu)->SetP4(oldP4 * muRescale);

    //Try smearing the whole P4 vector, not just the Pt.
    LorentzM newP4 = oldP4 + (murSF_err)*(oldP4 - genP4.at(MatchedGenParticle) );
    newP4.SetPxPyPzE(newP4.Px(), newP4.Py(), newP4.Pz(), newP4.P() ); //Maintain a zero mass.
    metCorr += (oldP4 - newP4);
    MuonsToRescale.at(iMu)->SetP4(newP4);

  }

  return;
}

float getMurSF(float err) {
  return (1.00 + err);
}

void rescaleMUR_simple(EasyChain* tree, vector<Muon*>&MuonsToRescale, LorentzM& metCorr, float murSF_err) {

  //Loop over Muons
  for (int iMu = 0; iMu < MuonsToRescale.size(); iMu++) {

    LorentzM oldP4 = MuonsToRescale.at(iMu)->P4();

    //Smear the energy of the muon
    float oldE = oldP4.E();
    TRandom3 rand(0);
    float newE = oldE + rand.Gaus(0., murSF_err*oldE);
    if (newE < 0) newE = 0.;

    float muRescale = newE / oldE;

    metCorr += oldP4 * (1. - muRescale);
    MuonsToRescale.at(iMu)->SetP4(oldP4 * muRescale);
  }

  return;
}

#undef CUTFLOW
