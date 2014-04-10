#include "makePileUp.h"

using namespace std;

void pileUpInfo::RescaleWeight( float PUNum, double& weight, string type=""){
  
  relevantNumPU = (int) PUNum;
  if( relevantNumPU >= nobinsmc ) {
    cout << "something wrong with the pile up info!!! - exceed max number of vertex:     " << nobinsmc <<endl;
    return; 
  }
  
  else if( oldpuw) {
    PUWeight= PUmc.at( relevantNumPU );
  }
  else {
    if (type=="" || type=="central"){
      PUWeight= PUdata.at( relevantNumPU )/PUmc.at( relevantNumPU );
      weight *= PUWeight;
    }else if (type=="up"){
      PUWeight_up= PUdata_up.at( relevantNumPU )/PUmc.at( relevantNumPU );
      weight *= PUWeight_up;
    }else if (type=="down"){
      PUWeight_down= PUdata_down.at( relevantNumPU )/PUmc.at( relevantNumPU );
      weight *= PUWeight_down;
    }else{
      cout<<"wrong pileup weight chice. ERROR"<<endl;
    }
  }

}

double pileUpInfo::GetWeight(string type=""){
  if (type=="" || type=="central"){
    if(PUWeight == 1.){
      PUWeight= PUdata.at( relevantNumPU )/PUmc.at( relevantNumPU );
      return PUWeight;
    }
  }else if (type=="up"){
    if(PUWeight_up==1.){
      PUWeight_up= PUdata_up.at( relevantNumPU )/PUmc.at( relevantNumPU );
      return PUWeight_up;
    }
  }else if (type=="down"){
    if(PUWeight_down==1.){
	  PUWeight_down= PUdata_down.at( relevantNumPU )/PUmc.at( relevantNumPU );
    }
	  return PUWeight_down;
  }else{
    cout<<"wrong pileup weight chice 2.ERROR"<<endl;
  }    
}
