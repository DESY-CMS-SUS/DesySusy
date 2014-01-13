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
void matchGenJets(EasyChain* tree,vector<GenJet>& genJets, vector<Jet*>& Jets);
void matchJets( const vector<Jet*> & recoJets, const vector<LorentzM> & genJets, vector<int> & recoToGen, vector<int> & genToReco);

void  rescaleJES(EasyChain* tree, vector<Jet*>& AllJets, LorentzM & metCorr, float jesSF_err);
//void  rescaleJER(EasyChain* tree, vector<Jet*>& AllJets, LorentzM & metCorr, float jerSF_err, JetMonitor * pJetM);
float getJerSF(float eta, float err_factor);
float getJetRes(double pT, double eta);
#endif
