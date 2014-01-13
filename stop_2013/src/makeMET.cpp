#include "makeMET.h"

LorentzM makeType1MET( EasyChain* tree, const LorentzM& met_in, const vector<Jet*>& jets){
  double metx = (double)met_in.Px();
  double mety = (double)met_in.Py();
  
  // Type I corrections
  vector<float>& L1fact = tree->Get(&L1fact, "DESYak5JetPFL1FastJetCorrFactorPat");
  vector<float>& L3fact = tree->Get(&L3fact, "DESYak5JetPFL3AbsoluteCorrFactorPat");
  
  for (unsigned int ijet = 0 ; ijet < jets.size() ; ijet++) {
    int indx = jets.at(ijet)->IndexInTree();
    double L1corr_factor = L1fact.at(indx);
    double FullCorr_factor = L3fact.at(indx);
    
    metx += jets.at(ijet)->Px() * ( 1. / L1corr_factor - 1. / FullCorr_factor );
    mety += jets.at(ijet)->Py() * ( 1. / L1corr_factor - 1. / FullCorr_factor );
  }

  double   met     = sqrt(metx*metx + mety*mety);
  LorentzM met_out = LorentzM(); met_out.SetPxPyPzE( metx, mety, 0., met);
						
  if(pcp) cout<<"TyteI MET: pt="<<met_out.pt()<<"; phi="<<met_out.phi()<<"; eta="<<met_out.eta()<<"; "<<met_in.pt()-met_out.pt()<<endl;

  return met_out;
}
    
LorentzM makePhiCorrectedMET( EasyChain* tree, const LorentzM& met_in){
  double metx = (double)met_in.Px();
  double mety = (double)met_in.Py();

  vector<float>&     vertex_ndof    = tree->Get( &vertex_ndof, "vertexNdof");
  int nvrtx = vertex_ndof.size();

  metx -= (+8.72683e-02 - 1.66671e-02*nvrtx);
  mety -= (+1.86650e-01 - 1.21946e-01*nvrtx);

  double   met     = sqrt(metx*metx + mety*mety);
  LorentzM met_out = LorentzM(); met_out.SetPxPyPzE( metx, mety, 0., met);

  if(pcp) cout<<"Phi Corr MET: pt="<<met_out.pt()<<"; phi="<<met_out.phi()<<"; eta="<<met_out.eta()<<"; "<<met_in.pt()-met_out.pt()<<endl;

  return met_out;
}


  
