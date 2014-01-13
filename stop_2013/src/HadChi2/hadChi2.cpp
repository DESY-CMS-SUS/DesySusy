/*
 *
 *****compiling *****
 *
 * make
 * needs ROOT
 *
 ********************
 *
 */

//Corrigé par Thomas DESCHLER.

#include "hadChi2.h"

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>
#include<stdexcept> // needed for exception runtime_error
#include<stdio.h>
#include<stdlib.h>
#include<valarray>
#include<map>

#include "TF1.h"

//Example
int main(int argc, char *argv[]){
  
  int t0=clock();

  // initialize test event
  TLorentzVector mySamplebjet;    mySamplebjet.SetPxPyPzE(142, 0, 0, 142.);

  TLorentzVector mySampleqcdjet1; mySampleqcdjet1.SetPxPyPzE(-41., 0., 0., 40.);
  TLorentzVector mySampleqcdjet2; mySampleqcdjet2.SetPxPyPzE( 39., 0., 0., 40.);
  TLorentzVector mySampleqcdjet3; mySampleqcdjet3.SetPxPyPzE(169.,132.,-230.,467.);
  
  std::vector<TLorentzVector> jets;
  std::vector<float> bDiscs;
  jets.push_back(mySamplebjet);    bDiscs.push_back(1);
  jets.push_back(mySampleqcdjet1); bDiscs.push_back(0);
  jets.push_back(mySampleqcdjet2); bDiscs.push_back(0);
  jets.push_back(mySampleqcdjet3); bDiscs.push_back(0);

  // initialize hadChi2
  HadChi2::HadChi2 chi2;
  cout << "The hadChi2 value on this event is " << chi2.GetChi2( jets, bDiscs, 1) << endl;

  int t=clock()-t0;
  cout << "It took the program " << t << " clicks, or " << ((float) t)/CLOCKS_PER_SEC << " seconds." << endl;
  return 0;
}

HadChi2::HadChi2::HadChi2() {
  //sigma = TF1("sigma","sqrt(((TMath::Sign(1,[0])*sq([0]/x))+(sq([1])*(x^([3]-1))))+sq([2]))",0,10);
}

HadChi2::HadChi2::~HadChi2() {}

#define BTAG_MED     0.679
#define PDG_TOP_MASS 173.5
#define PDG_W_MASS   80.385


// ----------------------------------------------------------------------------
// Global functions
// ----------------------------------------------------------------------------

// Compute sigma for a given jet

double HadChi2::HadChi2::GetSigmaJet(const TLorentzVector& jet)
{
  TF1 sigma("sigma","sqrt((([4]*sq([0]/x))+(sq([1])*(x^([3]-1))))+sq([2]))",0,10);
  
  if (abs(jet.Eta()) <= 0.5)
  {
    sigma.SetParameter(0,-0.349206);
    sigma.SetParameter(1,0.297831);
    sigma.SetParameter(3,0.471121);
  }
  else if (abs(jet.Eta()) <= 1.0)
  {
    sigma.SetParameter(0,-0.499735);
    sigma.SetParameter(1,0.336391);
    sigma.SetParameter(3,0.430689);
  }
  else if (abs(jet.Eta()) <= 1.5)
  {
    sigma.SetParameter(0,-0.561649);
    sigma.SetParameter(1,0.420293);
    sigma.SetParameter(3,0.392398);
  }
  else if (abs(jet.Eta()) <= 2.0)
  {
    sigma.SetParameter(0,-1.12329);
    sigma.SetParameter(1,0.657891);
    sigma.SetParameter(3,0.139595);
  }
  else if (abs(jet.Eta()) <= 2.5)
  {
    sigma.SetParameter(0,1.04792);
    sigma.SetParameter(1,0.466763);
    sigma.SetParameter(3,0.193137);
  }
  else if (abs(jet.Eta()) <= 3.0)
  {
    sigma.SetParameter(0,2.56933);
    sigma.SetParameter(1,0.305802);
    sigma.SetParameter(3,0.398929);
  }
  else if (abs(jet.Eta()) <= 3.5)
  {
    sigma.SetParameter(0,2.81978);
    sigma.SetParameter(1,0.272373);
    sigma.SetParameter(3,0.579396);
  }
  else if (abs(jet.Eta()) <= 4.0)
  {
    sigma.SetParameter(0,1.65966);
    sigma.SetParameter(1,0.223683);
    sigma.SetParameter(3,0.60873);
  }
  else
  {
    sigma.SetParameter(0,1.41584);
    sigma.SetParameter(1,0.209477);
    sigma.SetParameter(3,0.588872);
  }
    
  sigma.SetParameter(2,0);
  sigma.SetParameter(4, TMath::Sign(1., sigma.GetParameter(0)));
  
  return sigma.Eval(jet.Pt());
}

// This is a Data/MC Ratio, so it has to be applied on MC
double HadChi2::HadChi2::GetSigmaScaleFactor(double eta)
{

    // Taken from JetResolution TWiki page on 2 sep. 2013
         if (abs(eta) <= 0.5) return 1.052;
    else if (abs(eta) <= 1.1) return 1.057;
    else if (abs(eta) <= 1.7) return 1.096;
    else if (abs(eta) <= 2.3) return 1.134;
    else if (abs(eta) <= 5.0) return 1.288;

    // No SF given after 5.0 
    //    => returning 1.0 as default value.
    // Shouldn't be a problem as it's unlikely 
    // that a jet with |eta|>5.0 get selected anyway.
    else return 1.0;

}

double HadChi2::HadChi2::GetChi2( const std::vector<TLorentzVector>& jets,
			 const std::vector<float>& bDiscs,
			 bool isData)
{
  vector<int> v_i, v_j;
  vector<double> v_k1, v_k2;
  
  // Get the sigmas
  vector<float> sigmas;

  for(unsigned int i=0; i<jets.size(); i++)
    {
      // Apply scale factors only for MC
      if (!isData)
	sigmas.push_back( GetSigmaJet(jets.at(i)) * GetSigmaScaleFactor(jets[i].Eta()));
      else
	sigmas.push_back(GetSigmaJet(jets.at(i)));
    }

  assert(jets.size() == sigmas.size());
  assert(jets.size() == bDiscs.size());

  //check at most first 6 jets
  unsigned int n_jets = jets.size();
  if (n_jets>6) n_jets = 6;
  //consider at least 3 jets
  if (n_jets<3) return 99999.;

  for ( unsigned int i=0; i<n_jets; ++i )
    for ( unsigned int j=i+1; j<n_jets; ++j )
      {

	//  W
	TLorentzVector hadW = jets.at(i) + jets.at(j);

	//  W Mass Constraint.
	double p1 = -1;
	
	TFitterMinuit minimizer;
	minimizer.ExecuteCommand("SET PRINTOUT", &p1, 1);
	minimizer.SetFCN(&(HadChi2::minuitFunction));
	minimizer.SetParameter(0 , "c1"     , 1.1             , 1 , 0 , 0);
	minimizer.SetParameter(1 , "pt1"    , 1.0             , 1 , 0 , 0);
	minimizer.SetParameter(2 , "sigma1" , sigmas.at(i)    , 1 , 0 , 0);
	minimizer.SetParameter(3 , "pt2"    , 1.0             , 1 , 0 , 0);
	minimizer.SetParameter(4 , "sigma2" , sigmas.at(j)    , 1 , 0 , 0);
	minimizer.SetParameter(5 , "m12"    , jets.at(i).M2() , 1 , 0 , 0);
	minimizer.SetParameter(6 , "m22"    , jets.at(j).M2() , 1 , 0 , 0);
	minimizer.SetParameter(7 , "m02"    , hadW.M2()       , 1 , 0 , 0);

	for (unsigned int k = 1; k < 8; k++)
	  minimizer.FixParameter(k);
	
	minimizer.ExecuteCommand("SIMPLEX", 0, 0);
	minimizer.ExecuteCommand("MIGRAD", 0, 0);
	
	double c1 = minimizer.GetParameter(0);
	if (c1!=c1) 
	  {
	    std::cout <<"[HadChi2::GetChi2]"
		      << "ERROR: c1 parameter is NAN! Skipping this parton combination"
		      << std::endl;
	    continue;
	  }

	double c2 = HadChi2::fc2(c1, jets.at(i).M2(), jets.at(j).M2(), hadW.M2());
	
	//     * W Mass check :)
	//     *  Never trust a computer you can't throw out of a window.
	//     *  - Steve Wozniak
	//    
	//     *  Never trust somebody you can't throw out of a window.
	//     *  - Me
	
	v_i.push_back(i);
	v_j.push_back(j);
	v_k1.push_back(c1);
	v_k2.push_back(c2);
      }
  

  //Apply b-consistency requirement
  int n_btag = 0;
  for(unsigned int i=0;i<n_jets;i++)
    if(bDiscs.at(i)>BTAG_MED) n_btag++;
  
  double chi2min = 99999.;
  
  //consider b-jet in leading 3 jets
  for(unsigned int b=0;b<n_jets;++b)
    {    
      //if not tagged, consider only 3 leading jets
      if( bDiscs.at(b)<BTAG_MED && b>2 ) continue;
      
      //require b-tagging if have more than 1 b-tag
      if( n_btag>1 && bDiscs.at(b) < BTAG_MED ) continue;
      
      double pt_b = jets.at(b).Pt();
      
      for (unsigned int w=0;w<v_i.size();++w)
	{
	  unsigned int k = v_i[w];
	  unsigned int l = v_j[w];

	  if ( k==b || l==b ) continue;
	  
	  //count number of b-tagged Ws
	  int nwb = 0;
	  if(k<bDiscs.size())
	    if (bDiscs.at(k) > BTAG_MED) nwb++;

	  if(l<bDiscs.size())
	    if (bDiscs.at(l) > BTAG_MED) nwb++;

	  //no btagged jets in W if have few btags
	  if ( n_btag<3  && nwb>0 ) continue;
	  
	  //In 3 b-tag case, allow for 1 W jet to be tagged
	  // If have more b-tags then btagging information not useful
	  if ( n_btag==3 && nwb>1 ) continue;
	  
	  double pt_w1 = jets.at(k).Pt();
	  double pt_w2 = jets.at(l).Pt();
	  
	  //  W Mass.
	  TLorentzVector hadW = jets.at(k) + jets.at(l);
	  double massW = hadW.M();
	  
	  double c1 = v_k1[w];
	  double c2 = v_k2[w];
      
	  // Top Mass.
	  TLorentzVector hadT = (jets.at(k) * c1) + (jets.at(l) * c2) + jets.at(b);
	  double massT = hadT.M();
	  double pt_w = hadW.Pt();
	  double sigma_w2 = pow(pt_w1*sigmas.at(k), 2)+ pow(pt_w2*sigmas.at(l), 2);
	  double smw2 = (1. + 2.*pow(pt_w,2)/pow(massW,2))*sigma_w2;
	  double pt_t = hadT.Pt();
	  double sigma_t2 = pow(c1*pt_w1*sigmas.at(k),2)+pow(c2*pt_w2*sigmas.at(l),2)+ pow(pt_b*sigmas.at(b),2);
	  double smtop2 = (1. + 2.*pow(pt_t,2)/pow(massT,2))*sigma_t2;    
	  double c_chi2 = pow(massT-PDG_TOP_MASS, 2)/smtop2+ pow(massW-PDG_W_MASS, 2)/smw2;

	  if (c_chi2<chi2min)
	    {
	      chi2min = c_chi2;
	      hadT_ = hadT;
	      smtop_ = sqrt(smtop2);
	      mtop_ = massT;
	    }
	}
    }
  
  return chi2min;
}

double HadChi2::HadChi2::fc2 (double c1, double m12, double m22, double m02)
{
  double a = m22;
  double b = (m02 - m12 - m22) * c1;
  double c = m12 * c1 * c1 - PDG_W_MASS * PDG_W_MASS;
  double num = -1. * b + sqrt(b * b - 4 * a * c);
  double den = 2 * a;

  return (num/den);
}

double HadChi2::HadChi2::fchi2 (double c1, double pt1, double sigma1, double pt2, 
		       double sigma2,double m12, double m22, double m02)
{
  double rat1 = pt1 * (1 - c1) / sigma1;
  double rat2 = pt2 * (1 - HadChi2::fc2(c1, m12, m22, m02)) / sigma2;
  
  return ( rat1 * rat1 + rat2 * rat2);
}

//--------------------------------------------------------------------
void HadChi2::HadChi2::minuitFunction(int& titi, double* tata, double &result, double* par, int toto)
{
   result=HadChi2::fchi2(par[0], par[1], par[2], par[3], par[4], par[5], par[6],par[7]);
}
