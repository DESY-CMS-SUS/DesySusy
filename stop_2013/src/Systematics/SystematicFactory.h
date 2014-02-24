#ifndef SystematicFactory_h
#define SystematicFactory_h

#include "TString.h" 
#include "Systematic.h"
#include "NoSystematic.h"
#include "PUReweight.h"
#include "BTagReweight.h"
#include "JES.h"
#include "JER.h"

#include <iostream>

using namespace std;

namespace Systematics 
{
  
  class SystematicFactory
  {
  public:
    
    static Systematics::Systematic* NewSystematic(TString sys)
    {
      sys.ToLower();

      if(sys=="nosystematic") {
	cout<<sys<<" will be evaluated."<<endl;
	return new Systematics::NoSystematic;	
      }

      //PUReweight
      else if(sys=="pureweight_up") {
	cout<<sys<<" systematic will be evaluated."<<endl;
	return new Systematics::PUReweight("Up");
      }
      else if(sys=="pureweight_down") {
	cout<<sys<<" systematic will be evaluated."<<endl;
	return new Systematics::PUReweight("Down");
      }

      //BTagReweight
      else if(sys=="btagreweight_upbc") {
	cout<<sys<<" systematic will be evaluated."<<endl;
	return new Systematics::BTagReweight("UpBC");
      }
      else if(sys=="btagreweight_downbc") {
	cout<<sys<<" systematic will be evaluated."<<endl;
	return new Systematics::BTagReweight("DownBC");
      }
      else if(sys=="btagreweight_uplight") {
	cout<<sys<<" systematic will be evaluated."<<endl;
	return new Systematics::BTagReweight("UpLight");
      }
      else if(sys=="btagreweight_downlight") {
	cout<<sys<<" systematic will be evaluated."<<endl;
	return new Systematics::BTagReweight("DownLight");
      }

      //JES
      else if(sys=="jes_up") {
	cout<<sys<<" systematic will be evaluated."<<endl;
	return new Systematics::JES("Up");
      }
      else if(sys=="jes_down") {
	cout<<sys<<" systematic will be evaluated."<<endl;
	return new Systematics::JES("Down");
      }

      //JER
      else if(sys=="jer_genup") {
	cout<<sys<<" systematic will be evaluated."<<endl;
	return new Systematics::JER("GenUp");
      }
      else if(sys=="jer_gencentral") {
	cout<<sys<<" systematic will be evaluated."<<endl;
	return new Systematics::JER("GenCentral");
      }
      else if(sys=="jer_gendown") {
	cout<<sys<<" systematic will be evaluated."<<endl;
	return new Systematics::JER("GenDown");
      }
      else if(sys=="jer_recoup") {
	cout<<sys<<" systematic will be evaluated."<<endl;
	return new Systematics::JER("RecoUp");
      }
      else if(sys=="jer_recocentral") {
	cout<<sys<<" systematic will be evaluated."<<endl;
	return new Systematics::JER("RecoCentral");
      }
      else if(sys=="jer_recodown") {
	cout<<sys<<" systematic will be evaluated."<<endl;
	return new Systematics::JER("RecoDown");
      }

      else {
	cout<<"No matching for SystematicType: NoSystematic will be evaluated."<<endl;
	return new Systematics::NoSystematic;
      }  
    }
  };
};
#endif
