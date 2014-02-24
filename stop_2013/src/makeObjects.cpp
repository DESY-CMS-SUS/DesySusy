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
#include "THTools.h"
#include "TProfile.h"

#include "TNtuple.h"
#include "TTree.h"
#include "TFile.h"

#include "TStopwatch.h"
#include "TSystem.h"

#include "TLorentzVector.h"
#include "Math/VectorUtil.h"

#include "NtupleTools2.h"
#include "ConfigReader.h"
#include "CutFlow.h"

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
#include "makePileUp.h"
#include "TriggerEfficiencyProvider.h"

#include "Trigger_Stop2012.h"
#include "CleaningFilters.h"
#include "makeCleanEvent.h"
#include "IsoTrackVeto.h"
#include "EventFilterFromListStandAlone.h"

#include "HistoMaker.h"
#include "TriggerStudyTree.h"
#include "SkimmingTree.h"
#include "Bonsai.h"

#include "BTagReshaping/BTagReshaping.h" // make a BTag share lib
#include "ScanInfoProvider.h"

#include "Systematics/Systematic.h"
#include "Systematics/SystematicFactory.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

//===================================================================
//                   GLOBAL VARIABLES
//===================================================================

bool pcp = false; //Set to true for debugging.

BTagShapeInterface reshape("data/csvdiscr.root",0,0);
EventFilterFromListStandAlone badLaserFilter;
desy_tools::ScanInfoProvider scanInfo("data/ScanInfo.root");
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
  int doSkimmingTree = config.getInt("doSkimmingTree",0);
  int doTriggerStudyTree = config.getInt("doTriggerStudyTree",0);
  int doBonsaiTree = config.getInt("doBonsaiTree",1);
    
  TString outname = config.getTString("outname",tree->GetUniqeName());
  TFile *outfile = TFile::Open(outname,"RECREATE");
  outfile->cd();

  cout<<endl;
  cout<<"-----------------------------------------------"<<endl;
  cout<<"The output file name is "<<outname<<endl;
  cout<<"-----------------------------------------------"<<endl;
  cout<<endl;

  CutSet globalFlow("global flow");
  CutSet muonFlow("muon flow");
  CutSet electronFlow("electron flow");
  CutSet* flow;

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
  EventInfo info;
  info.Sample = config.getString("SampleName","undefined"); cout<<"Sample = "<<info.Sample<<endl;
  info.SubSample = config.getString("SubSampleName","undefined"); cout<<"SubSample = "<<info.SubSample<<endl;
  info.Estimation = config.getString("Estimation","undefined"); cout<<"Estimation = "<<info.Estimation<<endl;
  info.Tail = config.getString("Tail","undefined"); cout<<"Tail = "<<info.Tail<<endl;

  info.xs = config.getFloat( "xs_"+info.Sample+"_"+info.SubSample, 1.); cout<<"xs = "<<info.xs<<endl;
  info.NEvents = config.getInt( "TNoE_"+info.Sample+"_"+info.SubSample, 1); cout<<"TNoE = "<<info.NEvents<<endl;
  info.FE = config.getFloat( "FE_"+info.Sample+"_"+info.SubSample, 1.); cout<<"FE = "<<info.FE<<endl;
  info.isData = config.getBool("isData");  cout<<"isData = "<<info.isData<<endl;
  info.isFSIM = config.getBool("isFSIM");  cout<<"isFSIM = "<<info.isFSIM<<endl;
  info.isScan = config.getBool("isScan");  cout<<"isScan = "<<info.isScan<<endl;

  TH1I* isdata = new TH1I("isdata","data =1 means Data",1,0,1);
  if(info.isData){isdata->SetBinContent(1,1);}
  else{isdata->SetBinContent(1,0);}
  isdata->Write();
  delete isdata;
  
  TH1I* num_entries = new TH1I("num_entries","number of entries",1,0,1);
  num_entries->SetBinContent(1,N);
  num_entries->Write();
  delete num_entries;
  //===========================================
  if(pcp)cout<<"sample info loaded!"<<endl;


  if(pcp)cout<<"going to pileUp initialization"<<endl;
  //======================================================
  // pileUp Initialization
  //======================================================
  float PUnumInter = -1.;
  pileUpInfo pileUp;

  if (!info.isData){ pileUp.Initialize( info.Sample, info.SubSample);}
  //==========================================
  if(pcp)cout<<"out of pileUp initialization"<<endl;

  if(pcp)cout<<"going to trigger initialization"<<endl;
  //======================================================
  // pileUp Initialization
  //======================================================
  Trigger_Stop2012 trigger = Trigger_Stop2012();
  //==========================================
  if(pcp)cout<<"out of trigger initialization"<<endl;

  //================================
  //  Miscellaneous
  //================================
  Jet::SetWP("8TeV"); 
  bool OK=false;
  badLaserFilter = EventFilterFromListStandAlone( "data/badlaser_events_StdFormat.txt.gz");
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
  
  sysName.push_back("JER_GenUp");
  sysName.push_back("JER_GenCentral");
  sysName.push_back("JER_GenDown");
  sysName.push_back("JER_RecoUp");
  sysName.push_back("JER_RecoCentral");
  sysName.push_back("JER_RecoDown");*/
  
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
  //N=1000;
  for(int i=0;i<N;++i){
    timer();
    event = Event();

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

    if(info.Sample.find("TTJets") != std::string::npos)
      if (!desy_tools::TTJetsSubSampling( info.SubSample, tree))
	continue;

    if(info.isScan)
      if(!desy_tools::ScanCheck(info.Sample, info.SubSample, tree))
	continue;
    
    if(info.Sample.compare("T2tbPoints") == 0)
      if (!desy_tools::T2tbPoints( info.SubSample, tree))
	continue;

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
    // Check Good Vertices
    //============================================
    vector<int> goodVert;
    cleaningFilters::goodVertices(tree,goodVert);
    //============================================

    //============================================
    // Make Tracks
    //============================================
    vector<Particle> tracks;
    tracks = makeAllTracks( tree);

    event.SetTracks( tracks);
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

    event.SetMuons( muons);
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

    event.SetElectrons( electrons);
    //============================================

    //============================================
    // Make Taus
    //============================================
    vector<Tau>  taus;
    vector<Tau*> cleanTaus;
    vector<Tau*> vetoTaus;

    taus = makeAllTaus( tree);
    makeCleanTaus( taus, cleanTaus, goodMuons, goodElectrons);
    makeVetoTaus( tree, taus, vetoTaus, event.FirstLepton());

    event.SetTaus( taus);
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

    event.SetJets( jets);
    event.SetUnclusteredEnergy( unclusteredEnergy);
    //============================================    
    
    //============================================    
    // Make GenJets
    //============================================
    vector<GenJet>  genJets;
    
    if(!info.isData) genJets = makeAllGenJets( tree);
    event.SetGenJets( genJets);
    //============================================ 

    //============================================    
    // Make MET
    //============================================
    LorentzM rawMET = makeMET<LorentzV>( tree, "DESYmetP4PF");
    LorentzM typeIMET = makeType1MET( tree, rawMET, goodJets);
    LorentzM typeIPhiCorrMET = makePhiCorrectedMET( tree, typeIMET);

    //LorentzM typeIMETPat = makeMET<LorentzM>( tree, "metP4TypeIPF");
    //LorentzM typeIPhiCorrMETPat = makePhiCorrectedMET( tree, typeIMETPat);

    LorentzM caloMET = makeMET<LorentzV>( tree, "corMetGlobalMuonsP4Calo");
    LorentzM mvaMET = typeIPhiCorrMET;//makeMET<LorentzV>( tree, "DESYmetP4MVA");

    event.SetCaloMET( caloMET);
    event.SetRawMET( rawMET);
    event.SetTypeIMET( typeIMET);
    event.SetTypeIPhiCorrMET( typeIPhiCorrMET);
    //============================================ 

    //============================================
    // EventInfo
    //============================================
    info.PUWeight = 1.;
    info.PUWeight_up = 1.;
    info.PUWeight_down = 1.;

    if(!info.isData){
      PUnumInter = tree->Get( PUnumInter, "pileupTrueNumInteractionsBX0");

      pileUp.RescaleWeight( PUnumInter, info.PUWeight, "central");
      pileUp.RescaleWeight( PUnumInter, info.PUWeight_up, "up");
      pileUp.RescaleWeight( PUnumInter, info.PUWeight_down, "down");
    }

    info.PUInter = (int) PUnumInter;
    vector<float>& vertex_ndof = tree->Get( &vertex_ndof, "vertexNdof");
    info.NPV = vertex_ndof.size();
    info.NgoodPV = goodVert.size();

    makeEventInfo( tree, info);
    event.SetInfo( info);

    if(pcp && event.FirstLepton() != 0) {
      cout<<"id = "<<event.FirstLepton()->PdgID()<<"; ";
      cout<<"pt = "<<event.FirstLepton()->Pt()<<"; ";
      cout<<"eta = "<<event.FirstLepton()->Eta()<<"; ";
      cout<<"triggerWeight = "<<event.TriggerEfficiency()<<"."<<endl;
    }

    CutSet::global_event_weight  = event.EventWeight();
    HistoMaker::global_event_weight  = CutSet::global_event_weight;

    //============================================

    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    

    //           E V E N T   C L E A N I N G                             //


    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    globalFlow.keepIf("allEvents", true);
    electronFlow.keepIf("allEvents", true);
    muonFlow.keepIf("allEvents", true);
    //====================================================================
    // Apply Triggers
    //====================================================================
    if( info.isData){
      double EventWeight = 1.;
      OK = trigger.Fired(tree, event.Info()->Sample,  event.Info()->Run, event.FirstLepton());
      if(pcp)cout<<"check point triggers called"<<endl;
      if( !globalFlow.keepIf("triggers", OK )) continue;    
    }
    //====================================================================

    //====================================================================    
    // check vertices
    //====================================================================
    OK = goodVert.size();

    electronFlow.keepIf("PV", OK);
    muonFlow.keepIf("PV", OK);
    if(  !globalFlow.keepIf("PV", OK) ) continue;
    //====================================================================

    //====================================================================    
    // MET Filters
    //====================================================================
    if(pcp)cout<<"check point calling Event cleaning"<<endl;     
    OK = makeCleanEvent(tree);

    electronFlow.keepIf("EventCleaning", OK);
    muonFlow.keepIf("EventCleaning", OK);
    if(  !globalFlow.keepIf("EventCleaning", OK) ) continue;
    //====================================================================

    //============================================    
    // Additional Filters
    //============================================

    // Anomalous Rho Filter
    if(pcp)cout<<"Anomalous rho filter!"<<endl;
    OK= cleaningFilters::anomalousRho( tree);

    electronFlow.keepIf("anomalousRho", OK);
    muonFlow.keepIf("anomalousRho", OK);
    if( !globalFlow.keepIf( "anomalousRho", OK) ) continue;

    // MET consistency
    if(pcp)cout<<"MET consistency filter!"<<endl;
    OK= cleaningFilters::METconsistency( caloMET, typeIPhiCorrMET);

    electronFlow.keepIf("METconsistency", OK);
    muonFlow.keepIf("METconsistency", OK);
    if( !globalFlow.keepIf( "METconsistency", OK) ) continue;
    //============================================

    electronFlow.keepIf("Event Cleaning", true);
    muonFlow.keepIf("Event Cleaning", true);
    if( !globalFlow.keepIf( "Event Cleaning", true) ) continue;

    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    

    //           S K I M M I G                                           //


    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================


    //============================================    
    // Lepton cuts.
    //============================================
    OK = selectedMuons.size() + selectedElectrons.size() > 0;
    if ( !globalFlow.keepIf( "at Least 1 Selected Lepton", OK) ) continue;

    OK = abs(event.FirstLepton()->PdgID()) == 11;
    if (OK){
      electronFlow.keepIf("Electron Selection", OK);
      flow = &electronFlow;
    }

    OK = abs(event.FirstLepton()->PdgID()) == 13;
    if (OK){
      muonFlow.keepIf("Muon Selection", OK);
      flow = &muonFlow;
    }

    if(doTriggerStudyTree)
      trigTree->Fill( &event, tree);
    
    //============================================    
    // Cut Flow and ControlPlots 
    //============================================
    OK = !isoTrackVeto::IsoTrackVetoV4( event.FirstLepton(), event.Tracks());
    globalFlow.keepIf( "IsoTrackVeto", OK);
    flow->keepIf( "IsoTrackVeto", OK);

    OK = OK && vetoTaus.size() == 0;
    globalFlow.keepIf( "TauVeto", OK);
    flow->keepIf( "TauVeto", OK);

    OK = OK && event.nJets()>= 4;
    globalFlow.keepIf( "4+ Jets", OK);
    flow->keepIf( "4+ Jets", OK);

    OK = OK && event.nBJets()>= 1;
    globalFlow.keepIf( "1+ b Jets", OK);
    flow->keepIf( "1+ b Jets", OK);

    OK = OK && event.TypeIPhiCorrMET()->Pt() > 80.;
    globalFlow.keepIf( "MET>80GeV", OK);
    flow->keepIf( "MET>80GeV", OK);

    if (OK) ControlPlots.MakePlots( "PreSelection", selectedMuons, selectedElectrons, selectedJets, typeIPhiCorrMET);

    //============================================    
    // Jet cuts.
    //============================================
    int njets = (int) selectedJets.size();
    OK = njets >= 2;
    if ( !OK) continue;
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

    if ( doSkimmingTree){
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
    }

    if ( doBonsaiTree){
      for ( unsigned int isys = 0; isys < sysName.size(); isys++){
	sys.at(isys)->Eval( event);

	if (sys.at(isys)->SysEvent()->nJets() < 3)
	  continue;
	
	if (sys.at(isys)->SysEvent()->TypeIPhiCorrMET()->Pt() < 80.)
	  continue;
	bonsai.at(isys)->Fill( sys.at(isys)->SysEvent());  
      }
    }
  }  

  globalFlow.dumpToHist();
  muonFlow.dumpToHist();
  electronFlow.dumpToHist();
  if(doSkimmingTree)
    skTree->Write();
  if(doTriggerStudyTree)
    trigTree->Write();
  if(doBonsaiTree)
    for ( unsigned int isys = 0; isys < sysName.size(); isys++)
      bonsai.at(isys)->Write();
}
