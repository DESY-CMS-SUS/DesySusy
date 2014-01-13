#include "SubTree.h"

#include "TObjArray.h"
#include "TObjString.h"

SubTree::SubTree( TDirectory* indir, TString newdir){
  TDirectory* keep = gDirectory->GetDirectory("");

  if(indir == 0)
    home=0;
  else{
    if(newdir == "")    
      this->SetHome( indir);
    else
      this->SetHome( indir, newdir);
  }
  keep->cd();
}


void SubTree::SetHome( TDirectory* indir){
  home=indir;
  
  this->UpdatePath();
}

void SubTree::SetHome( TDirectory* indir, TString newdir){
  TDirectory* keep = gDirectory->GetDirectory("");

  home=indir;

  if(newdir!=(TString)"./"){
    TObjArray* arr=newdir.Tokenize("/");
    Int_t numsep=arr->GetEntries();
    TObjString* tok;

    TString newpath="";
    for(Int_t i=0; i<numsep;++i){
      tok=(TObjString*)(*arr)[i];    
      TString newsubdir=tok->GetName();
      newpath+=newsubdir; newpath+=(TString)+"/";
      home->mkdir(newsubdir);
      home=home->GetDirectory(newsubdir);
    }
  }
  keep->cd();

  this->UpdatePath();
}

void SubTree::Write(){  
  TDirectory* keep = gDirectory->GetDirectory("");

  cout<<tree->GetName()<<" writing in the output directory: "<<  home->GetPath()<<endl;
  home->cd();
  tree->Write();

  keep->cd();
};

const TDirectory* SubTree::GetDir(){
  return home;
};

TString SubTree::GetFullPath(){
  return fullpath;
};

TString SubTree::GetPath(){
  return path;
};

void SubTree::UpdatePath(){
  fullpath = home->GetPath();
  path = fullpath.ReplaceAll( home->GetFile()->GetName(), "");
  path = fullpath.ReplaceAll( ":", "");
}

const TTree* SubTree::GetTree(){
  return tree;
};
