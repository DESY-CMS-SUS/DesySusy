#ifndef Tools_h
#define Tools_h

#include <vector>
#include <string>
#include "NtupleTools2_h.h"
#include "AnalysisObject.h"
#include "Particle.h"
#include "Muon.h"
#include "Electron.h"
#include "Jet.h"
#include "Tau.h"
#include "TString.h"
#include "TFile.h"
#include "TLorentzVector.h"

using namespace std;

namespace gen_tools{
  std::vector<int> GetDaughters(int ParticleIndex, std::vector<int>* MotherIndex);
  int Decay(int ParticleIndex, std::vector<int>* MotherIndex, std::vector<int>* PdgId, std::vector<int>* Status, TString space, bool verbose = 0);
  TString IdToString(int id);

  double ISRWeight( const string& sample, EasyChain* chain);
  double ISRWeight( const string& sample, const vector<LorentzM>& p4, const vector<int>& pdgId);
  double TopPtWeight( const string& sample, EasyChain* chain);
  double TopPtWeight( const TString& sample, double topPt);
  
  ////////////////////////////////////////////////////////////
  // Code for the stop polariziation reweighting by Juan
  ////////////////////////////////////////////////////////////

  class SUSYGenParticle { // To be filled with status-3 genParticles
   public:
    int pdgId; // PDG identifier (with sign, please)
    int firstMother; // first mother, set to <0 if no mothers
    double energy; // energy [GeV]
    double pt; // pt [GeV]
    double eta; // eta
    double phi; // phi
  };
  double Reweight_Stop_to_TopChi0 ( vector<gen_tools::SUSYGenParticle> genParticles, double referenceTopPolarization, double requestedTopPolarization, TString prefix);
  double Norm(double M1, double M2, double MV, double CL, double CR);
  void Boost_To_Stop_Rest_Frame(TLorentzVector& stop4, TLorentzVector& chargino4, TLorentzVector& b4, TLorentzVector& neutralino4, TLorentzVector& W4, TLorentzVector& up4, TLorentzVector& down4, TLorentzVector& s4);
  double Reweight_T2bW (double thetaChi_eff, double thetaW_eff, std::vector<gen_tools::SUSYGenParticle> genParticles);
}

namespace desy_tools {

  float Consistency( LorentzM refP4, vector<LorentzM>& compP4);
  float Consistency( LorentzM refP4, EasyChain* tree, const char* compLabelP4);
  float Consistency( LorentzM refP4, int refCh, vector<LorentzM>& tmpCompP4, vector<int>& tmpCompCh);
  float Consistency( LorentzM refP4, int refCh, EasyChain* tree, const char* compLabelP4, const char* compLabelCh);

  template <class Comp> bool CleaningByDR( LorentzM const & ref, vector<Comp*>& comp, float DR = 0.3);
  bool CleaningByDR( LorentzM const & ref, vector<LorentzM*>& comp, float DR = 0.3);

  LorentzM Rescale( const LorentzM& vec, const double& c);
  
  template <class Obj1, class Obj2> bool compare_Object_Pt( const Obj1& obj1, const Obj2& obj2);
  template <class Obj1, class Obj2> bool compare_Pointer_Pt( const Obj1* obj1, const Obj2* obj2);
  float gettrigweight(int id1, float pt, float eta);

  double getJerSF(double eta, double err_factor);
  double getJetResolution(double pT, double eta);

  double GetBJetSF( double pt, double eta, TString tagger, TString wp, int match);

  bool BDisc_SortFunction ( const Jet* jet1, const Jet* jet2, const string& key);

  class  compare_Jet_BDisc{
  private:
    string key_;
    
  public:
    compare_Jet_BDisc( string key) : key_(key) {}
      bool operator()( const Jet* jet1, const Jet* jet2) const {
	return BDisc_SortFunction( jet1, jet2, key_);
      }
  };

  template <class J> double HT( const vector< J*>& jets);
  template <class J> double HT( const vector< J*>* jets);

  template <class J, class M> double HTratio( const vector< J*>& jets, const M& met);
  template <class J, class M> double HTratio( const vector< J*>* jets, const M* met);
  
  template <class J> double MHT( const vector< J*>& jets);
  template <class J> double MHT( const vector< J*>* jets);
  template <class J> double MHTx( const vector< J*>& jets);
  template <class J> double MHTx( const vector< J*>* jets);
  template <class J> double MHTy( const vector< J*>& jets);
  template <class J> double MHTy( const vector< J*>* jets);

  template <class L, class M> double MT ( const L* lep, const M* met); 

  double Meff ( double HT, double lpt, double met);

  double Y ( double met, double HT);
  
  template <class J> double Mlb( const LorentzM& lepton, const vector< const J*>* jets){
    double mlb = -999.;
    if ( jets->size() == 0)
      return mlb;
    
    double dR = 10.;
    double dRmin = 10.;    
    const J* jetClosest = 0;
    
    for (unsigned i = 0; i < jets->size(); i++) {
      dR = DeltaR( lepton, jets->at(i)->P4());
      if ( dR < dRmin){
	dRmin = dR;
	jetClosest = jets->at(i);
      }
    }

    if (jetClosest == 0)
      mlb = -1.;
    else
      mlb = ( lepton + jetClosest->P4()).M();
    
    return mlb;
  };
  template <class J> double Mlb( const LorentzM& lepton, const vector< const J*>& jets){
    return Mlb( lepton, &jets);
  }

  template <class J> double M3b( const LorentzM& lepton, const vector< const J*>* jets){
    int njets = jets->size();
    // need at least 3 jets
    if(njets < 3){
      cout << "M3b cannot be calculated for events with less than three jets!!" << endl;
      return -1;
    }

    LorentzM sum;
    if(njets == 3){
      sum = jets->at(0)->P4() + jets->at(1)->P4() + jets->at(2)->P4();
    } else { //check which jet is closest to lepton, then take other 3
      double dphimin = 10.;
      int index_closest_jet = -1;
      for(int i=0; i<4; i++){
        double dphi = fabs(DeltaPhi( jets->at(i)->P4(), lepton));
        if(dphi < dphimin){
          dphimin = dphi;
          index_closest_jet = i;
        }
      }
      for(int i=0; i<4; i++){
        if(i!=index_closest_jet)
          sum = sum + jets->at(i)->P4();
      }
    }    
    return sum.M();    
  }
  template <class J> double M3b( const LorentzM& lepton, const vector< const J*>& jets){
    return M3b( lepton, &jets);
  }

  template <class M, class J> double DeltaPhiMinj12m( const M* met, const vector< const J*>* jets){
    double dphimin = 999.;
    
    LorentzM tmpV = LorentzM( met->Pt(), met->Eta(), met->Phi(), met->M());

    for ( int ijet = 0; ijet < jets->size() && ijet < 2; ijet++){
      double dphi = fabs(DeltaPhi( jets->at(ijet)->P4(), tmpV));
      if(dphi < dphimin)
	dphimin = dphi;
    }

    return dphimin;
  }
  template <class M, class J> double DeltaPhiMinj12m( const M& met, const vector< const J*>& jets){
    return DeltaPhiMinj12m( &met, &jets);
  }
}

template <class Comp> 
  bool desy_tools::CleaningByDR( LorentzM const & ref, vector<Comp*>& comp, float DR){

  for(int icomp=0; icomp<(int)comp.size(); ++icomp){
    if(DeltaR( ref, comp.at(icomp)->P4()) < DR) return true;  
  }

  return false;
};

template <class Obj1, class Obj2> 
  bool desy_tools::compare_Object_Pt( const Obj1& obj1, const Obj2& obj2){
  return compare_Pointer_Pt( &obj1, &obj2);
};

template <class Obj1, class Obj2> 
  bool desy_tools::compare_Pointer_Pt( const Obj1* obj1, const Obj2* obj2){

  //Ordering mechanism for Particles. Used for stl containers with sort.
  //If obj1 has a higher Pt than obj2, it should return true. This means that obj1
  //should be placed before obj2 in the list.
  
  if ( obj1->Pt() > obj2->Pt() ) {
    return true;
  }
  else return false;
};

//////////////////////////
//  HT
/////////////////////////

template <class J> double desy_tools::HT( const vector< J*>& jets){
  double ht=0;
  for (int ijet = 0; ijet<jets.size(); ijet++){
    ht += jets.at(ijet)->Pt();
  }
  return ht;
};
template <class J> double desy_tools::HT( const vector< J*>* jets){
  return HT( *jets);
};

template <class J, class M> double desy_tools::HTratio( const vector< J*>& jets, const M& met){
  double htssm_ = 0.;
  double htosm_ = 0.;
  
  for (int ijet = 0; ijet<jets.size(); ijet++){
    if ( cos( jets.at(ijet)->phi() - met.phi() ) > 0.)
      htssm_ += jets.at(ijet)->pt();
    else
      htosm_ += jets.at(ijet)->pt();
  }
    
  return htssm_ / (htssm_ + htosm_);
};
template <class J, class M> double desy_tools::HTratio( const vector< J*>* jets, const M* met){
  return desy_tools::HTratio( *jets, *met);
};


//////////////////////////
//  MHT
/////////////////////////

template <class J> double desy_tools::MHTx( const vector< J*>& jets){
  double mhtx=0;
  for (int ijet = 0; ijet<jets.size(); ijet++){
    mhtx += jets.at(ijet)->Px();
  }
  return mhtx;
};
template <class J> double desy_tools::MHTx( const vector< J*>* jets){
  return MHTx( *jets);
};


template <class J> double desy_tools::MHTy( const vector< J*>& jets){
  double mhty=0;
  for (int ijet = 0; ijet<jets.size(); ijet++){
    mhty += jets.at(ijet)->Py();
  }
  return mhty;
};
template <class J> double desy_tools::MHTy( const vector< J*>* jets){
  return MHTy( *jets);
};

template <class J> double desy_tools::MHT( const vector< J*>& jets){
  return sqrt( pow( MHTx( jets), 2.) + pow( MHTy( jets), 2.));
};
template <class J> double desy_tools::MHT( const vector< J*>* jets){
  return sqrt( pow( MHTx( *jets), 2.) + pow( MHTy( *jets), 2.));
};

//////////////////////////
//  MT
/////////////////////////

template <class L, class M> double desy_tools::MT ( const L* lep, const M* met){  
  return sqrt(2 * lep->pt() * met->pt() * (1 - cos( lep->phi() - met->phi() ) ) );
}
#endif
