#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "TDirectory.h"
#include "THTools.h"
#include "eventselection.h"
#include "Electron.h"
#include "Muon.h"
#include "Jet.h"
#include "makeJets.h"
#include "myBTagReshaping.h"
#include "BTagReshaping.h"

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
extern myBTagReshaping    myReshaping;
extern BTagShapeInterface sh;
extern BTagShapeInterface sh2;

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

  vector<Jet>        jets;
  Jet                dummyJet;
  vector<LorentzM>&  jets_p4         = tree->Get( &jets_p4,         "ak5JetPFCorrectedP4Pat");
  vector<float>&     jets_corrFactor = tree->Get( &jets_corrFactor, "ak5JetPFCorrFactorPat");
  vector<float>&     jets_CSV        = tree->Get( &jets_CSV,        "ak5JetPFCombinedSecondaryVertexBJetTagsPat");
  vector<int>&       jets_IDLoose    = tree->Get( &jets_IDLoose,    "ak5JetPFPFJetIDloosePat");
  vector<int>&       jets_IDTight    = tree->Get( &jets_IDTight,    "ak5JetPFPFJetIDtightPat");
  vector<int>&       jets_PULoose    = tree->Get( &jets_PULoose,    "DESYak5JetPFPileUpIdfull5xLoosePat");
  vector<int>&       jets_PUMedium   = tree->Get( &jets_PUMedium,   "DESYak5JetPFPileUpIdfull5xMediumPat");
  vector<int>&       jets_PUTight    = tree->Get( &jets_PUTight,    "DESYak5JetPFPileUpIdfull5xTightPat");
  //=end=variables=initialization=======================================

  //=============================================
  // LOOP OVER THE JETS
  //=============================================
  for(int ijet = 0; ijet<jets_p4.size(); ijet++){

    if(pcp){
      cout<<"Jet "<<ijet<<": ";
      cout<<"Pt = "<<jets_p4.at(ijet).Pt()<<";";
      cout<<"Eta = "<<jets_p4.at(ijet).Eta()<<";";
      cout<<"Phi = "<<jets_p4.at(ijet).Phi()<<".";
    }

    dummyJet.Set( ijet, jets_p4.at(ijet), jets_corrFactor.at(ijet), "AK5"); 
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
    
    for(int ijet = 0; ijet<jets.size(); ijet++){
      int indx = jets.at(ijet).IndexInTree();
      jets.at(ijet).SetMatchedGenJetIndex( jets_genIndx.at(indx)); 
      jets.at(ijet).SetPartonFlavor( jets_genFlavor.at(indx));
      
      int iflavor = jets_genFlavor.at(indx);
      double pt =  jets.at(ijet).Pt();
      double eta = jets.at(ijet).Eta();
      double bdisc = jets.at(ijet).BJetDisc("CSV");

      //double bdisc_myReshape = myReshaping.Reshape( iflavor, pt, eta, bdisc);     
      //jets.at(ijet).SetBJetDisc("CSV_myReshape", bdisc_myReshape);

      //double bdisc_fnalReshape = sh.reshape(eta, pt, bdisc, iflavor);
      //jets.at(ijet).SetBJetDisc("CSV_fnalReshape", bdisc_fnalReshape);
      
      //double bdisc_fnalReshapeOriginal = sh2.reshape(eta, pt, bdisc, iflavor);
      //jets.at(ijet).SetBJetDisc("CSV_fnalReshapeOriginal", bdisc_fnalReshapeOriginal);

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


//======================================================
// match Jets and GenJets
//======================================================

void matchGenJets(EasyChain* tree, vector<GenJet>& genJets, vector<Jet>& jets){
  
  for (int ijet=0; ijet<jets.size(); ++ijet){

    int jetIndx = jets.at(ijet).IndexInTree();
    int matchedGenJetIndx = jets.at(ijet).MatchedGenJetIndex();

    for ( int igenjet=0; igenjet < genJets.size(); ++igenjet){
      int genJetIndx = genJets.at( igenjet).IndexInTree();

      if ( genJetIndx == matchedGenJetIndx ){

	jets.at(ijet).SetIsMatched( true);
	jets.at(ijet).SetMatchedGenJet( &genJets.at( igenjet));

	genJets.at( igenjet).SetIsMatched( true);
	genJets.at( igenjet).SetMatchedJet( &jets.at(ijet));

	break;
      }
    }
  }
}

//====================================
// PREVIOUS FUNCTIONS
//===================================
/*
void rescaleJER(EasyChain* tree, vector<Jet*>& AllJets, LorentzM & metCorr, float jerSF_err, JetMonitor * pJetM) {

  vector<LorentzM>&  genJets_p4 = tree->Get(&genJets_p4, "genak5GenJetsP4");
  //vector<int>&  genJets_matched = tree->Get(&genJets_matched, "ak5JetPFGenJetMatchIndexPat");

  vector<int> recoToGen(AllJets.size(), -1);
  vector<int> genToReco(genJets_p4.size(), -1);
  matchJets(AllJets, genJets_p4, recoToGen, genToReco);

  typedef vector<Jet*>::iterator jetIt;
  for (jetIt jet = AllJets.begin(); jet != AllJets.end(); jet++) {
    if (  (*jet)->Pt() > 10. && fabs((*jet)->Eta()) < 5.) {
      //Get core resolution factor.
      //Numbers taken from the Twiki CMS/JetResolution
      float jerSF = getJerSF((*jet)->Eta(), jerSF_err);
      if (pcp) cout << "jerSF: " << jerSF << endl;

      //Get the matched gen level jet.
      //int indexOfMatchedGenJet = genJets_matched.at( (*jet)->GetIndexInTree() );
      int indexOfMatchedGenJet = recoToGen[jet-AllJets.begin()];
      if (pcp) cout << "Matched to gen jet: " << indexOfMatchedGenJet << endl;
      
      //Check if matched index is ok.
      if (indexOfMatchedGenJet >= 0 && indexOfMatchedGenJet < genJets_p4.size() ) {
      
	//Get the P4 of the matched jet.
	LorentzM genP4 = genJets_p4.at(indexOfMatchedGenJet);
	if (pcp) {
	  cout << "Jet P4=(" << (*jet)->Px() << ","  << (*jet)->Py() << ","  << (*jet)->Pz() << ","  << (*jet)->E() << ")" << endl;
	  cout << "Matched gen jet P4=(" << genP4.Px() << ","  << genP4.Py() << ","  << genP4.Pz() << ","  << genP4.E() << ")" << endl;
	}

	//Get the full factor by which to rescale the jet p4.
	float jetRescale = genP4.Pt() + jerSF * ( (//======================================================
// make Selected Jets
//======================================================*jet)->Pt() - genP4.Pt() );
	jetRescale /= (*jet)->Pt();
	if (jetRescale < 0.) jetRescale = 0.;
	if (pcp) cout << "Jet rescale factor: " << jetRescale << endl;
	
	//Rescale the Jet's P4.
	LorentzM oldP4 = (*jet)->P4();
	metCorr += oldP4 * (1. - jetRescale);
	(*jet)->SetP4(oldP4 * jetRescale);

	//Store the information in the jet monitor
	if (pJetM) {
	  pJetM->hMatched->Fill(oldP4.Pt(), oldP4.Eta());
	  float deltaR = ROOT::Math::VectorUtil::DeltaR(oldP4, genP4);;
	  pJetM->hMatchedDeltaR->Fill(oldP4.Pt(), oldP4.Eta(), deltaR);
	  float deltaPt = oldP4.Pt() - genP4.Pt();
	  pJetM->hMatchedDeltaPt->Fill(oldP4.Pt(), oldP4.Eta(), deltaPt);
	  pJetM->hMatchedScaleFactor->Fill(oldP4.Pt(), oldP4.Eta(), jetRescale);
	}
	
      }
      else {

	LorentzM oldP4 = (*jet)->P4();
	float oldE = oldP4.E();
	
	float jetRes = getJetRes(oldP4.Pt(), oldP4.Eta());
	
	float newE = oldE;
	float gSigma = jerSF*jerSF - 1.;
	if (gSigma > 0.) {
	  gSigma = jetRes * sqrt(gSigma);
	  TRandom3 rand(0); //Random num gen with seed set by sys time
	  newE += rand.Gaus(0., gSigma);
	}
	float jetRescale = newE / oldE;
	if (jetRescale < 0.) jetRescale = 0.;
	if (pcp) cout << "Jet rescale factor: " << jetRescale << endl;

	metCorr += oldP4 * (1. - jetRescale);
	(*jet)->SetP4(oldP4 * jetRescale);

	//Store the jet info in the jet monitor
	if (pJetM) {
	  pJetM->hNotMatched->Fill(oldP4.Pt(), oldP4.Eta());
	  pJetM->hNotMatchedDeltaE->Fill(oldP4.Pt(), oldP4.Eta(), newE - oldE);
	  pJetM->hNotMatchedScaleFactor->Fill(oldP4.Pt(), oldP4.Eta(), jetRescale);
	}

      }
    }
  }


  return;
}
*/
float getJerSF(float eta, float err_factor) {

  float sf = 1.;

  //Values taken from AN-2011-330
  if ( fabs(eta) < 0.5 ) {
    sf = 1.052 + err_factor * 0.065;
  }
  else if (fabs(eta) < 1.1) {
    sf = 1.057 + err_factor * 0.059;
  }
  else if (fabs(eta) < 1.7) {
    sf = 1.096 + err_factor * 0.070;
  }
  else if (fabs(eta) < 2.3 ){
    sf = 1.134 + err_factor * 0.102;
  }
  else {
    sf = 1.288 + err_factor * 0.222;
  }
  
  if (sf < 0.) {
    cout << "getJerSF >> ERROR sf is negative. Set to zero." << endl;
    sf = 0.;
  }

  return sf;
}

void rescaleJES(EasyChain* tree, vector<Jet*>& AllJets, LorentzM & metCorr, float jesSF_err) {

  vector<float>&  jetJecUnc = tree->Get(&jetJecUnc, "ak5JetPFJecUncPat");

  for (int iJet = 0 ; iJet < AllJets.size(); iJet++) {
    Jet* cJet = AllJets.at(iJet);

    if (  (cJet)->Pt() > 10. && fabs((cJet)->Eta()) < 5.) {
      //Get the full factor by which to rescale the jet p4.
      float jetRescale = 1 + jesSF_err * jetJecUnc.at(iJet);
      if (jetRescale < 0.) jetRescale = 0.;
      if (pcp) cout << "Jet rescale factor: " << jetRescale << endl;
      
      //Rescale the Jet's P4.
      LorentzM oldP4 = cJet->P4();
      metCorr += oldP4 * (1. - jetRescale);
      cJet->SetP4(oldP4 * jetRescale);
    }
  }

  return;
}



float getJetRes(double pT, double eta) {

  double N=0., S=0., C=0., m=0.;

  if     ( fabs(eta) < 0.5) {N = -0.34921; S = 0.29783; C = 0.; m = 0.47112;}
  else if (fabs(eta) < 1.0) {N = -0.49974; S = 0.33639; C = 0.; m = 0.43069;}
  else if (fabs(eta) < 1.5) {N = -0.56165; S = 0.42029; C = 0.; m = 0.39240;}
  else if (fabs(eta) < 2.0) {N = -1.12329; S = 0.65789; C = 0.; m = 0.13960;}
  else if (fabs(eta) < 2.5) {N =  1.04792; S = 0.46676; C = 0.; m = 0.19314;}
  else if (fabs(eta) < 3.0) {N =  1.89961; S = 0.33432; C = 0.; m = 0.36541;}
  else if (fabs(eta) < 3.5) {N =  1.66267; S = 0.33780; C = 0.; m = 0.43943;}
  else if (fabs(eta) < 4.0) {N =  1.50961; S = 0.22757; C = 0.; m = 0.60094;}
  else if (fabs(eta) < 4.5) {N =  0.99052; S = 0.27026; C = 0.; m = 0.46273;}
  else                      {N =  1.37916; S = 0.28933; C = 0.; m = 0.61234;}

  double sgnN = N > 0. ? 1. : -1.;
  double sigma = sgnN * pow( N/pT , 2. ) + pow(S,2.)*pow(pT, m-1.) + pow(C,2.);
  if (sigma < 0.) sigma = 0.;

  sigma = pT * sqrt(sigma);

  return sigma;    
}

#undef CUTFLOW
