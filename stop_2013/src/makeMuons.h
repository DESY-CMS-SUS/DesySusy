#ifndef makeMuons_h
#define makeMuons_h

#include "NtupleTools2_h.h"
#include "Muon.h"
#include "TRandom3.h"

vector<Muon> makeAllMuons(EasyChain* tree, CutSet* flow_in = 0 );
bool makeGoodMuons(EasyChain* tree, vector<Muon>& allMuons,vector<Muon*>& goodMuons, CutSet* flow_in = 0 );
bool makeSelectedMuons(EasyChain* tree, vector<Muon>& allMuons,vector<Muon*>& selectedMuons, CutSet* flow_in = 0 );

void rescaleMUR(EasyChain* tree, vector<Muon*>&tightMuons, LorentzM& metCorr, float murErr);
float getMurSF(float err);
void rescaleMUR_simple(EasyChain* tree, vector<Muon*>&tightMuons, LorentzM& metCorr, float murErr);
#endif
