#ifndef makeJets_h
#define makeJets_h

#include "NtupleTools2_h.h"
#include "Jet.h"
#include "Muon.h"
#include "Electron.h"
#include "TRandom3.h"
#include "JetMonitor.h"
#include "GenJet.h"
#include "Tools.h"

using namespace std;

vector<Jet> makeAllJets(EasyChain* tree, CutSet* flow_in = 0);
void makeCleanJets(vector<Jet>& allJets, vector<Jet*>& cleanJets, vector<Muon*>& muons, vector<Electron*>& electrons, CutSet* flow_in = 0);
void makeGoodJets(EasyChain* tree, vector<Jet>& allJets, vector<Jet*>& goodJets, CutSet* flow_in = 0);
void makeSelectedJets(EasyChain* tree, vector<Jet>& allJets, vector<Jet*>& selectedJets, CutSet* flow_in = 0);

vector<GenJet> makeAllGenJets( EasyChain* tree, CutSet* flow_in = 0);
#endif
