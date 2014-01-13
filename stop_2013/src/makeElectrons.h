#ifndef makeElectrons_h
#define makeElectrons_h

#include "NtupleTools2_h.h"
#include "Electron.h"
#include "Muon.h"
#include "Tools.h"

vector<Electron> makeAllElectrons(EasyChain* tree, CutSet* flow_in = 0);
bool makeGoodElectrons(EasyChain* tree, vector<Electron>& allElectrons, vector<Electron*>&  goodElectrons, CutSet* flow_in = 0);
bool makeSelectedElectrons(EasyChain* tree, vector<Electron>& allElectrons, vector<Electron*>&  selectedElectrons, CutSet* flow_in = 0);
bool makeCleanElectrons( vector<Electron>& allElectrons, vector<Electron*>& cleanElectrons, vector<Muon*>& muons, CutSet* flow_in = 0);
#endif
