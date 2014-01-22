//========================================================================//
//                                                                        //
//                         Single Lepton Stop                             //
//                                                                        //
//========================================================================//
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include "TSystem.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "Math/VectorUtil.h"
#include "NtupleTools2.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "EventFilterFromListStandAlone.h"

#include "eventselection.h"
#include "SampleInformation.h"

#include "triggers_RA4b.h"
#include "SetTriggers_RA4b.h"
#include "THTools.h"
#include "TStopwatch.h"

#include "HistoMaker.h"
#include "TriggerEfficiencyProvider.h"

#include "makePileUp.h"

#include "Event.h"
#include "EventInfo.h"
#include "Particle.h"
#include "Muon.h"
#include "Electron.h"
#include "Tau.h"
#include "Jet.h"
#include "GenJet.h"

#include "makeEventInfo.h"
#include "makeTracks.h"
#include "makeMuons.h"
#include "makeElectrons.h"
#include "makeTaus.h"
#include "makeJets.h"
#include "makeMET.h"

#include "CleaningFilters.h"
#include "makeCleanEvent.h"
#include "IsoTrackVeto.h"

#include "TriggerStudyTree.h"
#include "SkimmingTree.h"
#include "Bonsai.h"

#include "myBTagReshaping.h"
#include "BTagReshaping.h"

#include "Systematics/Systematic.h"
#include "Systematics/SystematicFactory.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

//===================================================================
//                   GLOBAL VARIABLES
//===================================================================

bool pcp = false; //Set to true for debugging.

//myBTagReshaping    myReshaping( "myCsvdiscr.root");
//BTagShapeInterface sh( "myCsvdiscr.root",0,0);
BTagShapeInterface reshape( "csvdiscr.root",0,0);
EventFilterFromListStandAlone badLaserFilter;
//===================================================================

int main(int argc, char** argv){
  TH1::SetDefaultSumw2(true);  //keep track of the weights
  string MainDir="./";

  Event event;

  //===========================================
  // Config Reader
  //===========================================
  ConfigReader config(MainDir+"config.txt",argc,argv);
  config.Add(MainDir+"para_config.txt");
  config.Add(MainDir+"pu_config.txt");
  //===========================================
  if(pcp)cout<<"inputs set!"<<endl;


  if(pcp)cout<<"going to set inputs"<<endl;
  //===========================================
  // Set Input Files and Tree
  //===========================================
  TString filename = config.getTString("filename");

  EasyChain* tree = new EasyChain("/susyTree/tree");
  int f = tree->AddSmart(filename);
  if(pcp) cout<<tree->GetNtrees()<<" Files read"<<endl;

  if(pcp) cout<<"check point before tree->GetEntries"<<endl;
  int N = tree->GetEntries();  cout<<"THERE ARE "<<N<<" EVENTS IN "<<filename<<endl;
  //===========================================
  if(pcp)cout<<"inputs set!"<<endl;


  if(pcp)cout<<"going to set outputs"<<endl;
  //=====================================================
  // Set Output Files, Control Plots, Cut flow  and Tree
  //=====================================================
  TString outname = config.getTString("outname",tree->GetUniqeName());
  TFile *outfile = TFile::Open(outname,"RECREATE");
  outfile->cd();

  cout<<endl;
  cout<<"-----------------------------------------------"<<endl;
  cout<<"The output file name is "<<outname<<endl;
  cout<<"-----------------------------------------------"<<endl;
  cout<<endl;

  CutSet globalFlow("global flow");
  CutSet::setTFile(outfile);

  bool DoControlPlots=config.getBool("DoControlPlots",true);
  HistoMaker ControlPlots("AnalyzeSUSY"," ");
  ControlPlots.setTFile(outfile);

  SkimmingTree *skTree = new SkimmingTree( outfile, "");
  TriggerStudyTree *trigTree = new TriggerStudyTree( outfile, "");
  //===========================================
  if(pcp)cout<<"outputs set!"<<endl;


  if(pcp)cout<<"going to load sample info"<<endl;
  //===========================================
  // Sample Info
  //===========================================
  bool isData=config.getBool("isData");

  SampleInformation mySampleInfo;
  EventInfo info;
  info.Sample = config.getString("SampleName","undefined");
  info.SubSample = config.getString("SubSampleName","undefined");
  info.Estimation = config.getString("Estimation","undefined");
  info.Tail = config.getString("Tail","undefined");
  mySampleInfo.SetInformation( info.Sample, info.SubSample, info.Estimation, info.Tail);
  mySampleInfo.DumpInformation();

  info.xs = config.getFloat( "xs_"+info.Sample+"_"+info.SubSample, 1.); cout<<"xs = "<<info.xs<<endl;
  info.NEvents = config.getInt( "TNoE_"+info.Sample+"_"+info.SubSample, 1); cout<<"TNoE = "<<info.NEvents<<endl;
  info.FE = config.getFloat( "FE_"+info.Sample+"_"+info.SubSample, 1.); cout<<"FE = "<<info.NEvents<<endl;
  info.GlobalWeight = 1.;
  if ( !isData) info.GlobalWeight =  info.xs * info.FE / (float) info.NEvents;
  info.isData = isData;

  TH1I* isdata = new TH1I("isdata","data =1 means Data",1,0,1);
  if(isData){isdata->SetBinContent(1,1);}
  else{isdata->SetBinContent(1,0);}
  isdata->Write();
  delete isdata;
  
  TH1I* num_entries = new TH1I("num_entries","number of entries",1,0,1);
  num_entries->SetBinContent(1,N);
  num_entries->Write();
  delete num_entries;
  //===========================================
  if(pcp)cout<<"sample info loaded!"<<endl;


  if(pcp)cout<<"going to set triggers"<<endl;
  //===========================================
  // Set Triggers
  //===========================================
  vector<const char*> triggernames;
  SetTriggers_RA4b( mySampleInfo, triggernames);

  bool turntriggersoff = config.getBool( "turntriggersoff", false);
  if(turntriggersoff){
    cout<<"-----------TURNTRIGGERSOFF IS true!!-----------"<<endl;
    if(isData){
      cout<<endl;
      cout<<"--------NO TRIGGERS, NO DATA------------------"<<endl;
      cout<<endl;
      exit(1);
    }
  }
  //===========================================
  if(pcp)cout<<"triggers set!"<<endl;


  if(pcp)cout<<"going to pileUp initialization"<<endl;
  //======================================================
  // pileUp Initialization
  //======================================================
  pileUpInfo pileUp;

  if (!isData){ pileUp.Initialize(mySampleInfo);}
  //==========================================
  if(pcp)cout<<"out of pileUp initialization"<<endl;

  //================================
  //  Miscellaneous
  //================================
  Jet::SetWP("8TeV"); 
  bool OK=false;
  badLaserFilter = EventFilterFromListStandAlone( "badlaser_events_StdFormat.txt.gz");
  //=============================================


  //======================================================
  // Systematics
  //======================================================

  std::vector<Bonsai*> bonsai;
  std::vector<std::string> sysName;
  sysName.push_back("NoSystematic");
  
  /*sysName.push_back("PUReweight_Up");
  sysName.push_back("PUReweight_Down");
  
  sysName.push_back("BTagReweight_UpBC");
  sysName.push_back("BTagReweight_DownBC");
  sysName.push_back("BTagReweight_UpLight");
  sysName.push_back("BTagReweight_DownLight");

  sysName.push_back("JES_Up");
  sysName.push_back("JES_Down");
  */
  sysName.push_back("JER_GenUp");
  sysName.push_back("JER_GenCentral");
  sysName.push_back("JER_GenDown");
  sysName.push_back("JER_RecoUp");
  sysName.push_back("JER_RecoCentral");
  sysName.push_back("JER_RecoDown");


  std::vector< Systematics::Systematic*> sys;
  Systematics::SystematicFactory sysFactory;
  for ( unsigned int isys = 0; isys < sysName.size(); isys++){
    bonsai.push_back( new Bonsai( outfile, sysName.at(isys)));
    sys.push_back( sysFactory.NewSystematic( sysName.at(isys)));
  }
  
  //=============================================================================
  //=============================================================================
  //LOOP OVER EVENTS
  //=============================================================================
  //=============================================================================
  N=1000;
  for(int i=0;i<N;++i){
    timer();

    globalFlow.keepIf("allEvents", true);

    if(pcp)cout<<"check point about to get entry "<< i<<endl;      
    //====================================================
    tree->GetEntry(i);
    //====================================================

    tree->Get( info.Event,"event");
    tree->Get( info.Lumi, "lumiSection");
    tree->Get( info.Run,  "run");

    if(pcp){
      cout<<endl;
      cout<<"**************************************"<<endl;
      cout<<"EVENT = "<<info.Run<<":"<<info.Lumi<<":"<<info.Event<<endl;
      cout<<"**************************************"<<endl;
      cout<<endl;
    }

    //====================================================================
    // Apply Triggers
    //====================================================================
    if( isData){
      double EventWeight = 1.;
      OK = triggers_RA4b(tree, triggernames, EventWeight);
      if(pcp)cout<<"check point triggers called"<<endl;
      if( !globalFlow.keepIf("triggers", OK )) continue;    
    }
    //====================================================================

    //====================================================================    
    // check vertices
    //====================================================================
    vector<int> goodVert;
    if(pcp)cout<<"check point calling vertex"<<endl;     
    OK = cleaningFilters::goodVertices(tree,goodVert);
    if(  !globalFlow.keepIf("PV", OK) ) continue;
    if(pcp)cout<<"check point  vertex called"<<endl;
    //====================================================================

    //====================================================================    
    // MET Filters
    //====================================================================
    if(pcp)cout<<"check point calling Event cleaning"<<endl;     
    OK = makeCleanEvent(tree, &globalFlow);
    if(  !globalFlow.keepIf("EventCleaning", OK) ) continue;
    //====================================================================

    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    

    //           C R E A T E    O B J E C T S                            //


    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    
    //============================================
    // Make Tracks
    //============================================
    vector<Particle> tracks;
    //============================================
    
    //============================================
    // Make Muons
    //============================================
    vector<Muon>  muons;
    vector<Muon*> goodMuons;
    vector<Muon*> selectedMuons;

    muons = makeAllMuons( tree);
    makeGoodMuons( tree, muons, goodMuons);
    makeSelectedMuons( tree, muons, selectedMuons);
    //============================================

    //============================================
    // Make Electrons
    //============================================
    vector<Electron>  electrons;
    vector<Electron*> cleanElectrons;
    vector<Electron*> goodElectrons;
    vector<Electron*> selectedElectrons;
    
    electrons = makeAllElectrons( tree);
    makeCleanElectrons( electrons, cleanElectrons, goodMuons);
    makeGoodElectrons( tree, electrons, goodElectrons);
    makeSelectedElectrons( tree, electrons, selectedElectrons);
    //============================================

    //============================================
    // Make Taus
    //============================================
    vector<Tau>  taus;
    vector<Tau*> cleanTaus;
    
    taus = makeAllTaus( tree);
    makeCleanTaus( taus, cleanTaus, goodMuons, goodElectrons);
    //============================================
    
    //============================================    
    // Make Jets
    //============================================ 
    vector<Jet>  jets;
    vector<Jet*> cleanJets;
    vector<Jet*> goodJets;
    vector<Jet*> selectedJets;

    jets = makeAllJets( tree);
    makeCleanJets( jets, cleanJets, goodMuons, goodElectrons);
    makeGoodJets( tree, jets, goodJets);
    makeSelectedJets( tree, jets, selectedJets);

    LorentzM& unclusteredEnergy =tree->Get( &unclusteredEnergy, "ak5JetPFDroppedSumP4Pat");
    //============================================    
    
    //============================================    
    // Make GenJets
    //============================================
    vector<GenJet>  genJets;
    
    if(!isData) genJets = makeAllGenJets( tree);
    //============================================ 

    //============================================    
    // Make MET
    //============================================
    LorentzM rawMET = makeMET<LorentzV>( tree, "DESYmetP4PF");
    LorentzM typeIMET = makeType1MET( tree, rawMET, goodJets);
    LorentzM typeIPhiCorrMET = makePhiCorrectedMET( tree, typeIMET);

    LorentzM typeIMETPat = makeMET<LorentzM>( tree, "metP4TypeIPF");
    LorentzM tupeIPhiCorrMETPat = makePhiCorrectedMET( tree, typeIMETPat);

    LorentzM caloMET = typeIPhiCorrMET;
    if ( (info.Sample.compare("SingleMu") == 0 && info.SubSample.compare("Run2012D-PromptReco-v1") != 0) || 
	 info.Sample.compare("TTJetsPOWHEG") == 0 || 
	 info.Sample.compare("WJetsToLNu") == 0) 
      caloMET = makeMET<LorentzV>( tree, "corMetGlobalMuonsP4Calo");
    
    LorentzM mvaMET = typeIPhiCorrMET;
    if ( (info.Sample.compare("SingleMu") == 0 && info.SubSample.compare("Run2012D-PromptReco-v1") != 0) || 
	 info.Sample.compare("TTJetsPOWHEG") == 0 
	 || info.Sample.compare("WJetsToLNu") == 0) 
      mvaMET = makeMET<LorentzV>( tree, "DESYmetP4MVA");
    //============================================ 


    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    

    //           S K I M M I N G                                         //


    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   

    //============================================    
    // Lepton cuts.
    //============================================

    OK = selectedMuons.size() + selectedElectrons.size() > 0;
    if ( !globalFlow.keepIf( "atLeast1Lepton", OK) ) continue;

    Particle selectedLepton;
    if ( selectedElectrons.size() > 0) {
      selectedLepton = (Particle) *selectedElectrons.at( 0);
      if ( selectedMuons.size() > 0)
	if (  selectedMuons.at(0)->Pt() > selectedLepton.Pt())
	  selectedLepton = (Particle) *selectedMuons.at( 0);
    }
    else
      selectedLepton = (Particle) *selectedMuons.at( 0);
    //============================================

    //====================================================================
    // EventInfo
    //====================================================================
    info.PUWeight = 1.;
    info.TriggerWeight = 1.;
    if(!isData){
      pileUp.RescaleWeight( tree, info.PUWeight, "central");
      pileUp.RescaleWeight( tree, info.PUWeight_up, "up");
      pileUp.RescaleWeight( tree, info.PUWeight_down, "down");

      info.TriggerWeight = gettrigweight( selectedLepton.PdgID(), selectedLepton.Pt(), selectedLepton.Eta());
      if(pcp) {
	cout<<"id = "<<selectedLepton.PdgID()<<"; ";
	cout<<"pt = "<<selectedLepton.Pt()<<"; ";
	cout<<"eta = "<<selectedLepton.Eta()<<"; ";
	cout<<"triggerWeight = "<<info.TriggerWeight<<"."<<endl;
      }
    }

    CutSet::global_event_weight  = info.GlobalWeight * info.PUWeight * info.TriggerWeight;
    HistoMaker::global_event_weight  = CutSet::global_event_weight;

    info.EventWeight = info.GlobalWeight * info.PUWeight * info.TriggerWeight;

    float PUnumInter = -1.;
    if (!isData) PUnumInter = tree->Get( PUnumInter, "pileupTrueNumInteractionsBX0");
    info.PUInter = (int) PUnumInter;
    vector<float>&     vertex_ndof    = tree->Get( &vertex_ndof, "vertexNdof");
    info.NPV = vertex_ndof.size();
    info.NgoodPV = goodVert.size();

    makeEventInfo( tree, info);
    //============================================



    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    

    //           T R I G G E R   S T U D Y   T R E E                     //


    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //===================================================================== 

    event.SetInfo( info);
    event.SetMuons( muons);
    event.SetElectrons( electrons);
    event.SetJets( jets);
    
    trigTree->Fill( &event, tree);
    
    //sys.at(0)->Eval( event);

    //============================================    
    // Additional Filters
    //============================================

    // Anomalous Rho Filter
    if(pcp)cout<<"Anomalous rho filter!"<<endl;
    OK= cleaningFilters::anomalousRho( tree);
    //if( !globalFlow.keepIf( "anomalousRho", OK) ) continue;

    // MET consistency
    if(pcp)cout<<"MET consistency filter!"<<endl;
    OK= cleaningFilters::METconsistency( caloMET, typeIPhiCorrMET);
    //if( !globalFlow.keepIf( "METconsistency", OK) ) continue;
    //============================================ 
    
    //============================================    
    // Jet cuts.
    //============================================
    int njets = (int) selectedJets.size();
    OK = njets >= 3;
    if ( !globalFlow.keepIf( "3+Jets", OK) ) continue;

    //============================================    
    // Iso Track and tau vetos.
    //============================================
    tracks = makeAllTracks( tree);
    OK = !isoTrackVeto::IsoTrackVetoV4( selectedLepton, tracks);
    //if ( !globalFlow.keepIf( "IsoTrackVeto", OK)) continue;

    vector<Tau*> vetoTaus;
    makeVetoTaus( tree, taus, vetoTaus, selectedLepton);
    OK = vetoTaus.size() == 0;
    //if ( !globalFlow.keepIf( "TauVeto", OK)) continue;    
    //============================================ 


    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    

    //           F I L L I N G   T H E   S U B T R E E                   //


    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================  

    skTree->info            = &info;
    skTree->tracks          = &tracks;
    skTree->muons           = &muons;
    skTree->electrons       = &electrons;
    skTree->taus            = &taus;
    skTree->jets            = &jets;
    skTree->genJets         = &genJets;
    skTree->rawMET          = &rawMET;
    skTree->typeIMET        = &typeIMET;
    skTree->typeIPhiCorrMET = &typeIPhiCorrMET;
    skTree->caloMET         = &caloMET;
    skTree->mvaMET          = &mvaMET;

    skTree->Fill();

    event.SetInfo( info);
    event.SetTracks( tracks);
    event.SetMuons( muons);
    event.SetElectrons( electrons);
    event.SetTaus( taus);
    event.SetJets( jets);
    event.SetUnclusteredEnergy( unclusteredEnergy);
    event.SetGenJets( genJets);
    event.SetCaloMET( caloMET);
    event.SetRawMET( rawMET);
    event.SetTypeIMET( typeIMET);
    event.SetTypeIPhiCorrMET( typeIPhiCorrMET);

    for ( unsigned int isys = 0; isys < sysName.size(); isys++){
      sys.at(isys)->Eval( event);
      bonsai.at(isys)->Fill( sys.at(isys)->SysEvent());  
    }

    ControlPlots.MakePlots( "PreSelection", selectedMuons, selectedElectrons, selectedJets, typeIPhiCorrMET);
  }  

  globalFlow.dumpToHist();
  skTree->Write();
  trigTree->Write();
  for ( unsigned int isys = 0; isys < sysName.size(); isys++)
    bonsai.at(isys)->Write();
}
