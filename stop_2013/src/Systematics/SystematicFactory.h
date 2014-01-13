#ifndef SystematicFactory_h
#define SystematicFactory_h

#include "TString.h" 
#include "Systematic.h"
#include "NoSystematic.h"

#include <iostream>

using namespace std;

namespace Systematics{

  class SystematicFactory
  {
  public:
    
    static Systematics::Systematic* NewSystematic(TString sys) {
      //static Systematic* NewSystematic(TString sys) {
      
      if(sys=="NoSystematic"){
	cout<<sys<<" systematic will be evaluated."<<endl;
	return new Systematics::NoSystematic;
	//return new NoSystematic;
      }  
      else{
	cout<<"No matching for SystematicType: NoSystematic will be evaluated."<<endl;
	//return new NoSystematic;
	return new Systematics::NoSystematic;
      }  
    }
  };
};
#endif
