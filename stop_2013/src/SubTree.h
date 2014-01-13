#ifndef SubTree_h
#define SubTree_h

//#include "NtupleTools2.h"

#include "Event.h"

#include "TString.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>

class EasyChain;

class SubTree
 {
 public:
   SubTree( TDirectory* indir = 0, TString newdir = "");

   virtual void Fill( Event* event, EasyChain* chain) = 0;
   void Write();

   void SetHome( TDirectory*);
   void SetHome( TDirectory*, TString);
   
   const TDirectory* GetDir();
   TString GetPath();
   TString GetFullPath();
   const TTree* GetTree();

 protected:
   TString path;
   TString fullpath;
   void UpdatePath();
   
   TDirectory* home;
   TTree* tree;

   virtual void Reset() = 0;
   virtual void SetBranches() = 0;
 };
#endif
