void BonsaiProof(int iSample = 0){
  gSystem->Load("libsystematics.so");

  TString inDir = "/home/fcostanz/STOP/UserCode/DesySusy/stop_2013/";
  TString outDir = "/home/fcostanz/STOP/UserCode/DesySusy/stop_2013/Bonsai/";

  const int NSamples = 10;
  TString sample[NSamples];
  sample[0] = "Data";
  sample[1] = "WJets";
  sample[2] = "TTJets";
  sample[3] = "QCD";
  sample[4] = "SingleTop";
  sample[5] = "Rare";
  sample[6] = "T2tt_Stop400-LSP150";
  sample[7] = "T2tt_Stop500-LSP300";
  sample[8] = "T2tt_Stop600-LSP50";
  sample[9] = "T2tt_Stop750-LSP25";

  TString connect = gSystem->GetFromPipe("pod-info -c");
  TProof *p = TProof::Open(connect);
  gEnv->SetValue("Packetizer.MaxWorkersPerNode", 20);

  cout<<sample[iSample]<<endl;

  TChain c("skimmingTree");
  
  TString inFile = inDir; inFile += sample[iSample]; inFile += ".root";
  inFile = "/afs/desy.de/user/f/fcost/GIT2/DesySusy/stop_2013/TTJetsPowhegv1_cfg-91_out.root";
  c.Add(inFile);
  
  Long64_t nentries = c.GetEntries();
  Int_t cachesize = 200000000; //100 MB
  c.SetCacheSize(cachesize);
  
  TString outFile = outDir; outFile += sample[iSample]; outFile += ".root";
  outFile = "/afs/desy.de/user/f/fcost/GIT2/DesySusy/stop_2013/Bonsai/SimpleNtuple.root";
  p->AddInput( new TNamed( TString("OUTPUTFILE"), outFile));
  
  p->SetProgressDialog(1);
  c.SetProof(1);

  p->UploadPackage( "data" );
  p->Exec( "gSystem->Load( \"libsystematics.so\" );" );
  //p->Exec( "gSystem->Load( \"libRA4bDict.so\" );" );
    
  c.Process("BonsaiTSelector.C","");
  
  p->Close();
}
