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
   
   TString dirname = gSystem->pwd();
   dirname += "/../../packages/data/";   
   dirname = "";

   TString reshapeFile = dirname;
   reshapeFile += "csvdiscr.root";
   BTagShapeInterface reshape( reshapeFile.Data(),0,0);

   /*
   TSystemDirectory dir(dirname,dirname);
   TList *files = dir.GetListOfFiles();
   if (files) {
      TSystemFile *file;
      TString fname;
      TIter next(files);
      while ((file=(TSystemFile*)next())) {
         fname = file->GetName();
         if (!file->IsDirectory()) {
            cout << fname.Data() << endl;
         }
      }
   }
   */

   Jet::SetWP("8TeV");    
   TH1::SetDefaultSumw2(true);

   Systematics::SystematicFactory sysFactory;
   /*
   int a = 0;
   int b = 1;

   bonsai.push_back( new Bonsai( outFile, sysName.at(a)));
   bonsai.push_back( new Bonsai( outFile, sysName.at(b)));
   sys.push_back( sysFactory.NewSystematic( sysName.at(a)));
   sys.push_back( sysFactory.NewSystematic( sysName.at(b)));
   
   /*
   Int_t isys = 0;

   for( isys = 0; isys < 2; isys++)
       bonsai.push_back( new Bonsai( outFile, sysName.at(isys)));   
   for( isys = 0; isys < 2; isys++)
   sys.push_back( sysFactory.NewSystematic( sysName.at(isys)));*/
   for( int isys = 0; isys < 2; isys++)
     {
       cout<<"sisname "<<sysName.size()<<endl;
       cout<<"bonsai "<<bonsai.size()<<endl;
       int a = 0;
       if (isys == 0)
	 a = 0;
       if  (isys == 1)
	 a = 1;
     
       bonsai.push_back( new Bonsai( outFile, sysName.at(a)));
       sys.push_back( sysFactory.NewSystematic( sysName.at(a)));
     } 
   cout<<"sisname "<<sysName.size()<<endl;
   cout<<"bonsai "<<bonsai.size()<<endl;
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
  std::cout<<entry<<std::endl;
  GetEntry(entry);
  std::cout<<bonsai.at(0)<<" "<<bonsai.at(1)<<std::endl;

  event.SetInfo( *info);
  event.SetTracks( *tracks);
  event.SetMuons( *muons);
  event.SetElectrons( *electrons);
  event.SetTaus( *taus);
  event.SetJets( *jets);
  event.SetGenJets( *genJets);
  event.SetCaloMET( *caloMET);
  event.SetTypeIPhiCorrMET( *typeIPhiCorrMET);
  Int_t isys = 0;
  for( isys = 0; isys < sysName.size(); isys++)
    {
      Printf("Here1");
      sys.at(isys)->Eval( event);
      Printf("Here2");
      sys.at(isys)->SysEvent();
      Printf("Here3");
      if ( entry%10 == 0)
      
	//Event* mmm = sys.at(isys)->SysEvent();
	//Printf("Here4");

   cout<<"sisname "<<sysName.size()<<endl;
   cout<<"bonsai "<<bonsai.size()<<endl;

	bonsai.at(isys)->Fill( sys.at(isys)->SysEvent());
      Printf("Here5");
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

	  Bool_t cleanup = kFALSE;
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
	      outFile->cd();
	      outFile->cd(sysName.at(isys).c_str());
	      bonsaiTree = (TTree *) gDirectory->Get("bonsai");

	      Printf("Coping Bonsai: %s", sysName.at(isys).c_str());
	      fFile->mkdir(sysName.at(isys).c_str());
	      fFile->cd(sysName.at(isys).c_str());
	      	      
	      bonsaiTree->CloneTree()->Write();
	    }
	}
      else 
	{
	  Error("Terminate", "could not open file: %s", outputFile.Data());	
	  return;
	}
    }
  else {
    Error("Terminate", "TProofOutputFile not found");
    return;
  }
}
