#define BonsaiTSelector_cxx
// The class definition in BonsaiTSelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("BonsaiTSelector.C")
// Root > T->Process("BonsaiTSelector.C","")
// Root > T->Process("BonsaiTSelector.C+")
//

#include "BonsaiTSelector.h"
#include <TH2.h>
#include <TStyle.h>

void BonsaiTSelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   sysName.push_back("NoSystematic");
   //sysName.push_back("..");
}

void BonsaiTSelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   fProofFile = new TProofOutputFile( "SimpleNtuple.root", TProofOutputFile::kMerge, TProofOutputFile::kRemote);
   outFile = fProofFile->OpenFile("RECREATE");
   if (outFile && outFile->IsZombie()) SafeDelete(outFile);

   // Cannot continue
   if (!outFile) {
      Info("SlaveBegin", "could not create '%s': instance is invalid!", fProofFile->GetName());
      return;
   }
   
   Jet::SetWP("8TeV");    
   TH1::SetDefaultSumw2(true);

   Systematics::SystematicFactory sysFactory;
   for(unsigned int isys = 0; isys < sysName.size(); isys++)
     {
       bonsai.push_back( new Bonsai( outFile, sysName.at(isys)));
       sys.push_back( sysFactory.NewSystematic( sysName.at(isys)));
     }
}

Bool_t BonsaiTSelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either BonsaiTSelector::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

  GetEntry(entry);
  std::cout<<entry<<std::endl;

  event.SetInfo( *info);
  event.SetTracks( *tracks);
  event.SetMuons( *muons);
  event.SetElectrons( *electrons);
  event.SetTaus( *taus);
  event.SetJets( *jets);
  event.SetGenJets( *genJets);
  event.SetCaloMET( *caloMET);
  event.SetTypeIPhiCorrMET( *typeIPhiCorrMET);

  for(unsigned int isys = 0; isys < sysName.size(); isys++)
    {
      sys.at(isys)->Eval( event);
      bonsai.at(isys)->Fill( sys.at(isys)->SysEvent());
    }
  
  return kTRUE;
}

void BonsaiTSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
  
  if (outFile)
    {
      for( unsigned int isys = 0; isys < sysName.size(); isys++)
	{
	  const TTree* bonsaiTree = bonsai.at(isys)->GetTree();
	  
	  if (!bonsaiTree) {
	    Error("SlaveTerminate", "'ntuple' is undefined!");
	    return;
	  }
      
	  TDirectory *savedir = gDirectory;

	  if (bonsaiTree->GetEntries() > 0) {
	    //outFile->cd();
	    //bonsaiTree->Write(0, TObject::kOverwrite);
	    bonsai.at(isys)->Write();
	    fProofFile->Print();
	    fOutput->Add(fProofFile);
	  } else {
	    cleanup = kTRUE;
	  }
	  //bonsaiTree->SetDirectory(0);
	  gDirectory = savedir;
	  outFile->Close();
	}
    }
}

void BonsaiTSelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
  
  const TTree* bonsaiTree;

  
  if ((fProofFile =
       dynamic_cast<TProofOutputFile*>(fOutput->FindObject("SimpleNtuple.root"))))
    {
      
      TString outputFile(fProofFile->GetOutputFileName());
      TString outputName(fProofFile->GetName());
      outputName += ".root";
      Printf("outputFile: %s", outputFile.Data());
      
      TNamed *nm = dynamic_cast<TNamed *>(fInput->FindObject("OUTPUTFILE"));
      
      cout<<nm->GetTitle()<<endl;
      cout<<outputFile.Data()<<endl;
      
      TFile* fFile = new TFile( nm->GetTitle(), "RECREATE");
      
      outFile = TFile::Open(outputFile.Data());
      if (outFile) 
	{
	  outFile->cd();
	  Printf("Managed to open file: %s", outputFile.Data());
	  
	  for( unsigned int isys = 0; isys < sysName.size(); isys++)
	    {
	      Printf("Coping Bonsai: %s", bonsai.at(isys)->GetPath());
	      fFile->mkdir( bonsai.at(isys)->GetPath());
	      fFile->cd( bonsai.at(isys)->GetPath());
	      //Printf("Coping Bonsai: NoSystematic");
	      //fFile->mkdir( "NoSystematic");
	      //fFile->cd( "NoSystematic");
	      
	      bonsaiTree = (TTree *) outFile->Get("bonsai");
	      bonsaiTree->CloneTree()->Write();
	    }
	}
    }
  else 
    {
      Error("Terminate", "could not open file: %s", outputFile.Data());	
      return;
    }
}
