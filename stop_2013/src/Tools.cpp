#include "Tools.h"

std::vector<int> gen_tools::GetDaughters(int ParticleIndex, std::vector<int>* MotherIndex) {
  std::vector<int> daughters;
  daughters.clear();

  for( int igen=0; igen<MotherIndex->size(); igen++) {
    if (MotherIndex->at(igen) == ParticleIndex)
      daughters.push_back(igen);
  }
  return daughters;
}


int gen_tools::Decay(int ParticleIndex, std::vector<int>* MotherIndex, std::vector<int>* PdgId, std::vector<int>* Status, TString space, bool verbose) {
  TString spacenew="\t"; spacenew+=space;

  std::vector<int> daughters;
  daughters.clear();
  daughters=GetDaughters( ParticleIndex, MotherIndex );

  int count=0;

  for( int igen=0; igen<daughters.size(); igen++) {
    int id = PdgId->at(daughters.at(igen));
    if ( Status->at(igen) != 3 ) continue;
    if ( verbose)
      std::cout<<space<< "'------>" << IdToString(id) << std::endl;
    if ( abs(id) == 1000006 || abs(id) == 6 ||  abs(id) == 24 )
      count+=Decay(daughters.at(igen), MotherIndex, PdgId, Status, spacenew, verbose);
    if ( abs(id) == 11 ||  abs(id) == 13 || abs(id) == 15)
      count++;
  }
  return count;
}

TString gen_tools::IdToString(int id){
  TString particle;
  particle="";
  if (id<0) particle+="anti-";

  id=abs(id);
  if (id==1){ particle+="d"; }
  else if (id==2) {particle+="u";}
  else if (id==3) {particle+="s";}
  else if (id==4) {particle+="c";}
  else if (id==5) {particle+="b";}
  else if (id==6) {particle+="t";}
  else if (id==11){particle+="e";}
  else if (id==12){particle+="nu_e";}
  else if (id==13){particle+="mu";}
  else if (id==14){particle+="nu_mu";}
  else if (id==15){particle+="tau";}
  else if (id==16){particle+="nu_tau";}
  else if (id==24){particle+="W+";}
  else {particle+=id;}

  return particle;

}

double gen_tools::ISRWeight( const string& sample, EasyChain* chain){
  if ( sample.find("T2") != 0 && sample.find("TTJets") != 0 )
    return 1;

  vector<LorentzM> p4;
  vector<int> pdgId;

  vector<LorentzM>& p4tmp    = chain->Get( &p4tmp,    "genP4");
  vector<int>&      pdgIdtmp = chain->Get( &pdgIdtmp, "genPdgId");
  vector<int>&      status   = chain->Get( &status,   "genStatus");

  vector<int>& MotherIndex = chain->Get( &MotherIndex, "genMotherIndex");
  vector<int>& MotherPdgId = chain->Get( &MotherPdgId, "genMotherPdgId");
  
  for ( unsigned int i = 0; i < status.size(); i++){
    if( status.at(i) == 3){
      p4.push_back( p4tmp.at(i));
      pdgId.push_back( pdgIdtmp.at(i));
    }
  }
  return gen_tools::ISRWeight( sample, p4, pdgId);
}

double gen_tools::ISRWeight( const string& sample, const vector<LorentzM>& p4, const vector<int>& pdgId){
  if ( sample.find("T2") != 0 && sample.find("TTJets") != 0 )
    return 1;

  LorentzM stop;
  LorentzM stopbar;
  LorentzM top;
  LorentzM topbar;

  double isrboost = 0;
  double isrweight = 1;
  
  for ( unsigned int i = 0; i < pdgId.size(); i++){
    if      ( pdgId.at(i) ==  6)       top     = p4.at(i);
    else if ( pdgId.at(i) == -6)       topbar  = p4.at(i);
    else if ( pdgId.at(i) ==  1000006) stop    = p4.at(i);
    else if ( pdgId.at(i) == -1000006) stopbar = p4.at(i);
  }
  
  if ( sample.find("T2") == 0) isrboost = ( stop + stopbar).pt();
  else if ( sample.find("TTJets") == 0) isrboost = ( top + topbar).pt();
    
  if     ( isrboost > 120.0 && isrboost < 150.0 ) isrweight = 0.95;
  else if( isrboost > 150.0 && isrboost < 250.0 ) isrweight = 0.90;
  else if( isrboost > 250.0                     ) isrweight = 0.80;
  
  return isrweight;
}

double gen_tools::TopPtWeight( const string& sample, EasyChain* chain){
  if ( sample.find("TTJets") != 0)
    return 1.;

  vector<LorentzM>& p4     = chain->Get( &p4,     "genP4");
  vector<int>&      pdgId  = chain->Get( &pdgId,  "genPdgId");
  vector<int>&      status = chain->Get( &status, "genStatus");

  double topPt = -1.;
  double topbarPt = -1.;
  
  for ( unsigned int i = 0; i < pdgId.size(); i++){
    if ( status.at(i) != 3) continue;
    if ( pdgId.at(i) ==  6) topPt = p4.at(i).pt();
    else if ( pdgId.at(i) == -6) topbarPt = p4.at(i).pt();
  }

  return gen_tools::TopPtWeight( sample, sqrt( topPt * topPt + topbarPt * topbarPt));
}

double gen_tools::TopPtWeight( const TString& sample, double topPt){
  if( topPt<0 ) return 1.;
  return exp(0.156 - 0.00137 * topPt);
}

double gen_tools::Reweight_Stop_to_TopChi0 (std::vector<gen_tools::SUSYGenParticle> genParticles, double referenceTopPolarization, double requestedTopPolarization, TString prefix) {
 
  if( !prefix.Contains("T2tt") ) return 1.0;
 
  double weight = 1.;
  int nFoundStops = 0;
 
  unsigned int ngen = genParticles.size();
 
  for (unsigned int ig=0; ig<ngen; ++ig) {
    const SUSYGenParticle& gen = genParticles[ig];
    if (gen.firstMother<0) continue;
    if (abs(gen.pdgId)>20) continue; // expect quarks or leptons from W decay
 
    // Navigate upwards in the stop->top->W->fermion decay chain
    const SUSYGenParticle& genW = genParticles[gen.firstMother];
    if (genW.firstMother<0) continue;
    if (abs(genW.pdgId)!=24) continue;
    const SUSYGenParticle& genTop = genParticles[genW.firstMother];
    if (abs(genTop.pdgId)!=6) continue;
 
    // We only care about the down-type fermion
    if (genTop.pdgId*gen.pdgId>0) continue;
 
    // We also need a stop
    if (genTop.firstMother<0) continue;
    const SUSYGenParticle& genStop = genParticles[genTop.firstMother];
    if (abs(genStop.pdgId)!=1000006) continue;
 
    // Move top and fermion to the stop center-of-mass frame
    TLorentzVector stop4;
    stop4.SetPtEtaPhiE(genStop.pt, genStop.eta, genStop.phi, genStop.energy);
    TVector3 betaV(-stop4.Px()/stop4.Energy(),-stop4.Py()/stop4.Energy(),-stop4.Pz()/stop4.Energy());
 
    TLorentzVector top4;
    top4.SetPtEtaPhiE(genTop.pt, genTop.eta, genTop.phi, genTop.energy);
    top4.Boost(betaV);
 
    TLorentzVector ferm4;
    ferm4.SetPtEtaPhiE(gen.pt, gen.eta, gen.phi, gen.energy);
    ferm4.Boost(betaV);
 
    // Do not reweight if by any reason top/fermion directions are undefined
    // This should be pathological if things are fine
    if (top4.P()<=0 || ferm4.P()<=0) {
      printf("Warning: particles at rest, no weight applied: ptop: %.3e, pf: %.3e\n", top4.P(), ferm4.P());
      continue; 
    }
 
    double costh = (top4.Px()*ferm4.Px()+top4.Py()*ferm4.Py()+top4.Pz()*ferm4.Pz())/top4.P()/ferm4.P();
       
    double weight_L = (top4.Energy()+top4.P())*(1-costh);
    double weight_R = (top4.Energy()-top4.P())*(1+costh);
    weight *= ((1+requestedTopPolarization)*weight_R+(1-requestedTopPolarization)*weight_L)/((1+referenceTopPolarization)*weight_R+(1-referenceTopPolarization)*weight_L);
 
    nFoundStops++;
  }
 
  if( nFoundStops!=2 ) cout << __FILE__ << " " << __LINE__ << " WARNING: found " << nFoundStops << " stops, should be 2." << endl;
 
  return weight;
 
};
 
double gen_tools::Norm(double M1, double M2, double MV, double CL, double CR) {
      double lambda = pow(M1,4) + pow(M2,4) + pow(MV,4) - 2*pow(M1*M2,2) - 2*pow(M1*MV,2) - 2*pow(M2*MV,2);
      double norm = (CL*CL+CR*CR)*(lambda + 3*MV*MV*(M1*M1+M2*M2-MV*MV)) - 12*CL*CR*M1*M2*MV*MV;
      norm /= 3.;
      return norm;
}
 
void gen_tools::Boost_To_Stop_Rest_Frame(TLorentzVector& stop4, TLorentzVector& chargino4, TLorentzVector& b4, TLorentzVector& neutralino4, TLorentzVector& W4, TLorentzVector& up4, TLorentzVector& down4, TLorentzVector& s4){
      TVector3 betaV(-stop4.Px()/stop4.Energy(),-stop4.Py()/stop4.Energy(),-stop4.Pz()/stop4.Energy());
      stop4.Boost(betaV);
      chargino4.Boost(betaV);
      b4.Boost(betaV);
      neutralino4.Boost(betaV);
      W4.Boost(betaV);
      up4.Boost(betaV);
      down4.Boost(betaV);
      s4.SetE(chargino4.P()/chargino4.M());
      s4.SetVect(chargino4.Vect().Unit()*chargino4.Gamma());
}
 
double gen_tools::Reweight_T2bW (double thetaChi_eff, double thetaW_eff, std::vector<gen_tools::SUSYGenParticle> genParticles) {
    double weight = 1.;
 
    unsigned int ngen = genParticles.size();
 
    for (unsigned int i_stop=0; i_stop<ngen; ++i_stop) {
      // Look for stops
      const SUSYGenParticle& gen = genParticles[i_stop];
      if (abs(gen.pdgId)!=1000006) continue;
 
      // Look for stop decay products
      int i_b = -1;
      int i_chargino = -1;
      for (unsigned int ig=i_stop+1; ig<ngen; ++ig) {
            const SUSYGenParticle& gen = genParticles[ig];
            if (abs(gen.firstMother)!=i_stop) continue;
            if (abs(gen.pdgId)==5) i_b = ig;
            else if (abs(gen.pdgId)==1000024) i_chargino = ig;
            if (i_b>=0 && i_chargino>=0) break;
      }
      if (i_b<0 || i_chargino<0) continue;
 
      int i_neutralino = -1;
      int i_W = -1;
      for (unsigned int ig=i_chargino+1; ig<ngen; ++ig) {
            const SUSYGenParticle& gen = genParticles[ig];
            if (abs(gen.firstMother)!=i_chargino) continue;
            if (abs(gen.pdgId)==24) i_W = ig;
            else if (abs(gen.pdgId)==1000022) i_neutralino = ig;
            if (i_W>=0 && i_neutralino>=0) break;
      }
      if (i_W<0 || i_neutralino<0) continue;
 
      int i_up = -1;
      int i_down = -1;
      for (unsigned int ig=i_W+1; ig<ngen; ++ig) {
            const SUSYGenParticle& gen = genParticles[ig];
            if (abs(gen.firstMother)!=i_W) continue;
            if (abs(gen.pdgId)%2==0) i_up = ig;
            else if (abs(gen.pdgId)%2==1) i_down = ig;
            if (i_up>=0 && i_down>=0) break;
      }
      if (i_up<0 || i_down<0) continue;
 
      const SUSYGenParticle& gen_stop = genParticles[i_stop];
      const SUSYGenParticle& gen_b = genParticles[i_b];
      const SUSYGenParticle& gen_chargino = genParticles[i_chargino];
      const SUSYGenParticle& gen_W = genParticles[i_W];
      const SUSYGenParticle& gen_neutralino = genParticles[i_neutralino];
      const SUSYGenParticle& gen_up = genParticles[i_up];
      const SUSYGenParticle& gen_down = genParticles[i_down];
 
      // Fill Lorentz four-vectors
      TLorentzVector stop4, chargino4, b4, neutralino4, W4, up4, down4;
 
      stop4.SetPtEtaPhiE(gen_stop.pt, gen_stop.eta, gen_stop.phi, gen_stop.energy);
      chargino4.SetPtEtaPhiE(gen_chargino.pt, gen_chargino.eta, gen_chargino.phi, gen_chargino.energy);
      b4.SetPtEtaPhiE(gen_b.pt, gen_b.eta, gen_b.phi, gen_b.energy);
      neutralino4.SetPtEtaPhiE(gen_neutralino.pt, gen_neutralino.eta, gen_neutralino.phi, gen_neutralino.energy);
      W4.SetPtEtaPhiE(gen_W.pt, gen_W.eta, gen_W.phi, gen_W.energy);
      up4.SetPtEtaPhiE(gen_up.pt, gen_up.eta, gen_up.phi, gen_up.energy);
      down4.SetPtEtaPhiE(gen_down.pt, gen_down.eta, gen_down.phi, gen_down.energy);
 
      // Reference spin four-vector along the chargino direction (filled in after boost)
      TLorentzVector s4;
 
      // Move everything to the stop center-of-mass frame
      Boost_To_Stop_Rest_Frame(stop4, chargino4, b4, neutralino4, W4, up4, down4, s4);
 
      double c_L = sin(thetaW_eff);
      double c_R = cos(thetaW_eff);
      double norm_target = Norm(chargino4.M(), neutralino4.M(), W4.M(), c_L, c_R);
      double target = 0;
      for (int hel = -1; hel<2; hel += 2) {
            TLorentzVector t4 = s4*hel;
            TLorentzVector chargino4_plus = chargino4 + t4*chargino4.M();
            TLorentzVector chargino4_minus = chargino4 - t4*chargino4.M();
            target += (1. - chargino4.M()*cos(2*thetaChi_eff)*(b4*t4)/((b4*chargino4) - (b4.M()*chargino4.M())*sin(2*thetaChi_eff)))/2 *
           (8*c_L*c_L*(down4*chargino4_minus)*(up4*neutralino4)
                      + 8*c_R*c_R*(up4*chargino4_plus)*(down4*neutralino4)
                      - 4*c_L*c_R*neutralino4.M()*(pow(W4.M(),2)*chargino4.M()-2*(down4*t4)*(up4*chargino4)+2*(down4*chargino4)*(up4*t4)))/norm_target; 
      }
 
      weight *= target;
 
    }
 
    return weight;
 
};

float desy_tools::Consistency( LorentzM refP4, vector<LorentzM>& compP4) {
  float dR=0.;
  float dRclosest=20.;
  float dPt = 99999.;
  
  for (int iP4=0; iP4<compP4.size(); iP4++) {
    dR=ROOT::Math::VectorUtil::DeltaR( refP4, compP4.at(iP4));
    if (dR < dRclosest) {
      dRclosest = dR;
      dPt = fabs( refP4.Pt() - compP4.at(iP4).Pt());
    }
  }
  return dPt;
}

float desy_tools::Consistency( LorentzM refP4, EasyChain* tree, const char* compLabelP4) {
  
  vector<LorentzM>& compP4 = tree->Get(&compP4, compLabelP4);
  
  return desy_tools::Consistency( refP4, compP4);  
}

float desy_tools::Consistency( LorentzM refP4, int refCh, vector<LorentzM>& tmpCompP4, vector<int>& tmpCompCh) {
  
  vector<LorentzM> compP4;
    
  if (tmpCompP4.size() != tmpCompCh.size()){
    cout << "ERROR: desy_tools::Consistency: P4.size() != Ch.size()" << endl;
    return 1;
  }

  for (int iP4 = 0; iP4 < tmpCompP4.size(); iP4++) {
    if (tmpCompP4.at(iP4).Pt() > 10. && tmpCompCh.at(iP4) == refCh) compP4.push_back(tmpCompP4.at(iP4));
  }

  return desy_tools::Consistency( refP4, compP4);
}

float desy_tools::Consistency( LorentzM refP4, int refCh, EasyChain* tree, const char* compLabelP4, const char* compLabelCh) {
  
  vector<LorentzM>& tmpCompP4 = tree->Get(&tmpCompP4, compLabelP4);
  vector<int>&      tmpCompCh = tree->Get(&tmpCompCh, compLabelCh);
  
  vector<LorentzM> compP4;
  
  if (tmpCompP4.size() != tmpCompCh.size()){
    cout << "ERROR: desy_tools::Consistency: P4.size() != Ch.size()" << endl;
    return 1;
  }

  for (int iP4 = 0; iP4 < tmpCompP4.size(); iP4++) {
    if (tmpCompP4.at(iP4).Pt() > 10. && tmpCompCh.at(iP4) == refCh) compP4.push_back(tmpCompP4.at(iP4));
  }

  return desy_tools::Consistency( refP4, compP4);
}

LorentzM desy_tools::Rescale( const LorentzM& vec, const double& c){
  LorentzM vec_out; vec_out.SetPxPyPzE( vec.Px() * c,
					vec.Py() * c,
					vec.Pz() * c,
					vec.E() * c );
  return vec_out;
}


bool desy_tools::CleaningByDR( LorentzM const & ref, vector<LorentzM*>& comp, float DR){
double GetBJetSF( double pt, double eta, string tagger, string wp, int match);
  for(int icomp=0; icomp<(int)comp.size(); ++icomp){
    if(DeltaR( ref, *comp.at(icomp)) < DR) return true;  
  }

  return false;
};


double desy_tools::GetBJetSF( double pt, double eta, TString tagger, TString wp, int match) {
  TString Atagger = tagger; Atagger+=wp;
  
  if (match == 0)
    return -1;

  if( abs(match) == 1 || abs(match) == 2 || abs(match) == 3 || abs(match) == 21) {

    if (pt < 20.) pt = 20.;
    if (pt > 800.) pt = 800.;
    
    if( Atagger == "CSVL" && fabs(eta) < 0.5)
      return (1.04901)+(0.00152181*pt)+(-3.43568e-06*(pt*pt))+(2.17219e-09*(pt*(pt*pt)));
    if( Atagger == "CSVL" && fabs (eta) >= 0.5 && fabs(eta) < 1.0)
      return (0.991915)+(0.00172552*pt)+(-3.92652e-06*(pt*pt))+(2.56816e-09*(pt*(pt*pt)));
    if( Atagger == "CSVL" && fabs (eta) >= 1.0 && fabs(eta) < 1.5)
      return (0.962127)+(0.00192796*pt)+(-4.53385e-06*(pt*pt))+(3.0605e-09*(pt*(pt*pt)));
    if( Atagger == "CSVL" && fabs (eta) >= 1.5 && fabs(eta) < 2.4) {
      if (pt > 700) pt = 700;
      return (1.06121)+(0.000332747*pt)+(-8.81201e-07*(pt*pt))+(7.43896e-10*(pt*(pt*pt)));
    }
    
    if( Atagger == "CSVM" && fabs(eta) < 0.8)
      return (1.06238)+(0.00198635*pt)+(-4.89082e-06*(pt*pt))+(3.29312e-09*(pt*(pt*pt)));
    if( Atagger == "CSVM" && fabs (eta) >= 0.8 && fabs(eta) < 1.6)
      return (1.08048)+(0.00110831*pt)+(-2.96189e-06*(pt*pt))+(2.16266e-09*(pt*(pt*pt)));
    if( Atagger == "CSVM" && fabs (eta) >= 1.6 && fabs(eta) < 2.4) {
      if (pt > 700) pt = 700;
      return (1.09145)+(0.000687171*pt)+(-2.45054e-06*(pt*pt))+(1.7844e-09*(pt*(pt*pt)));
    }

    if( Atagger == "CSVT" && fabs(eta) < 2.4)
      return (1.01739)+(0.00283619*pt)+(-7.93013e-06*(pt*pt))+(5.97491e-09*(pt*(pt*pt)));
  }

  if( abs(match) == 4 || abs(match) == 5) {

    if (pt < 20.) pt = 20.;
    if (pt > 800.) pt = 800.; 

    if( Atagger == "CSVL" && fabs(eta) < 2.4)
      return 0.981149*((1.+(-0.000713295*pt))/(1.+(-0.000703264*pt)));
    if( Atagger == "CSVM" && fabs(eta) < 2.4)
      return 0.726981*((1.+(0.253238*pt))/(1.+(0.188389*pt)));
    if( Atagger == "CSVT" && fabs(eta) < 2.4)
      return 0.869965*((1.+(0.0335062*pt))/(1.+(0.0304598*pt)));
  }

  cout<<"TagEff.cpp: error! tagger="<<Atagger<<"; pt="<<pt<<"; eta="<<eta<<endl;

  return -1.;
}

bool desy_tools::BDisc_SortFunction ( const Jet* jet1, const Jet* jet2, const string& key){

  if ( jet1->BJetDisc( key) > jet2->BJetDisc( key) ) {
    return true;
  }
  else return false;
};



double desy_tools::Meff ( double HT, double lpt, double met){
  return HT + lpt + met;
}

double desy_tools::Y ( double met, double ht){
  return met / sqrt( ht);
}

float desy_tools::gettrigweight(int id1, float pt, float eta){

  //electron efficiencies
  if ( abs(id1)==11 ) {
    if ( fabs(eta)<1.5) {      
      if ( pt>=20 && pt<22 ) return 0.00;
      if ( pt>=22 && pt<24 ) return 0.00;
      if ( pt>=24 && pt<26 ) return 0.00;
      if ( pt>=26 && pt<28 ) return 0.08;
      if ( pt>=28 && pt<30 ) return 0.61;
      if ( pt>=30 && pt<32 ) return 0.86;
      if ( pt>=32 && pt<34 ) return 0.88;
      if ( pt>=34 && pt<36 ) return 0.90;
      if ( pt>=36 && pt<38 ) return 0.91;
      if ( pt>=38 && pt<40 ) return 0.92;      
      if ( pt>=40 && pt<50 ) return 0.94;
      if ( pt>=50 && pt<60 ) return 0.95;
      if ( pt>=60 && pt<80 ) return 0.96;
      if ( pt>=80 && pt<100 ) return 0.96;
      if ( pt>=100 && pt<150 ) return 0.96;
      if ( pt>=150 && pt<200 ) return 0.97;
      if ( pt>=200 ) return 0.97;
    } else if ( fabs(eta)>=1.5 && fabs(eta)<2.5) {
      if ( pt>=20 && pt<22 ) return 0.00;
      if ( pt>=22 && pt<24 ) return 0.00;
      if ( pt>=24 && pt<26 ) return 0.02;
      if ( pt>=26 && pt<28 ) return 0.18;
      if ( pt>=28 && pt<30 ) return 0.50;
      if ( pt>=30 && pt<32 ) return 0.63;
      if ( pt>=32 && pt<34 ) return 0.68;
      if ( pt>=34 && pt<36 ) return 0.70;
      if ( pt>=36 && pt<38 ) return 0.72;
      if ( pt>=38 && pt<40 ) return 0.74;
      if ( pt>=40 && pt<50 ) return 0.76;
      if ( pt>=50 && pt<60 ) return 0.77;
      if ( pt>=60 && pt<80 ) return 0.78;
      if ( pt>=80 && pt<100 ) return 0.80;
      if ( pt>=100 && pt<150 ) return 0.79;
      if ( pt>=150 && pt<200 ) return 0.76;
      if ( pt>=200 ) return 0.81;
    }
  } else if ( abs(id1)==13 ) {//muon efficiencies

    if ( fabs(eta)<0.8 ) {
      if (pt>=20 && pt<22)  return  0.85;     
      if (pt>=22 && pt<24)  return  0.85;      
      if (pt>=24 && pt<26)  return  0.87;
      if (pt>=26 && pt<28)  return  0.90;
      if (pt>=28 && pt<30)  return  0.91;
      if (pt>=30 && pt<32)  return  0.91;
      if (pt>=32 && pt<34)  return  0.92;
      if (pt>=34 && pt<36)  return  0.93;
      if (pt>=36 && pt<38)  return  0.93;
      if (pt>=38 && pt<40)  return  0.93;
      if (pt>=40 && pt<50)  return  0.94;
      if (pt>=50 && pt<60)  return  0.95;
      if (pt>=60 && pt<80)  return  0.95;
      if (pt>=80 && pt<100) return 0.94;
      if (pt>=100 && pt<150) return 0.94;
      if (pt>=150 && pt<200) return 0.93;
      if (pt>=200) return 0.92;
    } else if ( fabs(eta)>=0.8 && fabs(eta)<1.5 ) {
      if (pt>=20 && pt<22)  return  0.80;
      if (pt>=22 && pt<24)  return  0.80;
      if (pt>=24 && pt<26)  return  0.78;
      if (pt>=26 && pt<28)  return  0.81;
      if (pt>=28 && pt<30)  return  0.81;
      if (pt>=30 && pt<32)  return  0.81;
      if (pt>=32 && pt<34)  return  0.82;
      if (pt>=34 && pt<36)  return  0.82;
      if (pt>=36 && pt<38)  return  0.83;
      if (pt>=38 && pt<40)  return  0.83;
      if (pt>=40 && pt<50)  return  0.84;
      if (pt>=50 && pt<60)  return  0.84;
      if (pt>=60 && pt<80)  return  0.84;
      if (pt>=80 && pt<100) return 0.84;
      if (pt>=100 && pt<150) return 0.84;
      if (pt>=150 && pt<200) return 0.84;
      if (pt>=200) return 0.82;
    } else if ( fabs(eta)>=1.5 && fabs(eta)<2.1 ) {
      if (pt>=20 && pt<22)  return  0.80;
      if (pt>=22 && pt<24)  return  0.80;
      if (pt>=24 && pt<26)  return  0.80;
      if (pt>=26 && pt<28)  return  0.78;
      if (pt>=28 && pt<30)  return  0.79;
      if (pt>=30 && pt<32)  return  0.80;
      if (pt>=32 && pt<34)  return  0.80;
      if (pt>=34 && pt<36)  return  0.81;
      if (pt>=36 && pt<38)  return  0.81;
      if (pt>=38 && pt<40)  return  0.82;
      if (pt>=40 && pt<50)  return  0.82;
      if (pt>=50 && pt<60)  return  0.83;
      if (pt>=60 && pt<80)  return  0.83;
      if (pt>=80 && pt<100) return 0.83;
      if (pt>=100 && pt<150) return 0.83;
      if (pt>=150 && pt<200) return 0.82;
      if (pt>=200) return 0.82;
    }
  }//end check for muons

  return 1.;
}


double desy_tools::getJerSF(double eta, double err_factor) {

  double sf = 1.;

  if ( fabs(eta) < 0.5 ) {
    sf = 1.052 + err_factor * 0.065;
  }
  else if (fabs(eta) < 1.1) {
    sf = 1.057 + err_factor * 0.059;
  }
  else if (fabs(eta) < 1.7) {
    sf = 1.096 + err_factor * 0.070;
  }
  else if (fabs(eta) < 2.3 ){
    sf = 1.134 + err_factor * 0.102;
  }
  else {
    sf = 1.288 + err_factor * 0.222;
  }
  
  if (sf < 0.) {
    cout << "getJerSF >> ERROR sf is negative. Set to zero." << endl;
    sf = 0.;
  }

  return sf;
}

double desy_tools::getJetResolution(double pT, double eta) {

  double N=0., S=0., C=0., m=0.;

  if     ( fabs(eta) < 0.5) {N = -0.34921; S = 0.29783; C = 0.; m = 0.47112;}
  else if (fabs(eta) < 1.0) {N = -0.49974; S = 0.33639; C = 0.; m = 0.43069;}
  else if (fabs(eta) < 1.5) {N = -0.56165; S = 0.42029; C = 0.; m = 0.39240;}
  else if (fabs(eta) < 2.0) {N = -1.12329; S = 0.65789; C = 0.; m = 0.13960;}
  else if (fabs(eta) < 2.5) {N =  1.04792; S = 0.46676; C = 0.; m = 0.19314;}
  else if (fabs(eta) < 3.0) {N =  1.89961; S = 0.33432; C = 0.; m = 0.36541;}
  else if (fabs(eta) < 3.5) {N =  1.66267; S = 0.33780; C = 0.; m = 0.43943;}
  else if (fabs(eta) < 4.0) {N =  1.50961; S = 0.22757; C = 0.; m = 0.60094;}
  else if (fabs(eta) < 4.5) {N =  0.99052; S = 0.27026; C = 0.; m = 0.46273;}
  else                      {N =  1.37916; S = 0.28933; C = 0.; m = 0.61234;}

  double sgnN = N > 0. ? 1. : -1.;
  double sigma = sgnN * pow( N/pT , 2. ) + pow(S,2.)*pow(pT, m-1.) + pow(C,2.);
  if (sigma < 0.) sigma = 0.;

  sigma = pT * sqrt(sigma);

  return sigma;    
}
