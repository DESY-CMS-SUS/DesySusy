#ifndef makePileUp_h
#define makePileUp_h

#include "ConfigReader.h"
#include "SampleInformation.h"


class mySampleInfo; //forward declaration



class pileUpInfo{
private:
  bool oldpuw;
  int nobinsmc;
  int nobinsdata;
  int nobinsdata_up;
  int nobinsdata_down;  

  double PUWeight;
  double PUWeight_up;
  double PUWeight_down;
  int relevantNumPU;
  //


public:
  vector<double> PUmc;
  vector<double> PUdata;
  vector<double> PUdata_up;
  vector<double> PUdata_down;

  pileUpInfo(SampleInformation& mySampleInfo){
    this->Constructor();
    this->Initialize(mySampleInfo);
  }
  pileUpInfo(){
    this->Constructor();
  }

  void Constructor(){
    nobinsmc=0;
    nobinsdata=0;
    nobinsdata_up=0;
    nobinsdata_down=0;
    oldpuw = false;
    PUWeight=1; 
    PUWeight_up=1;
    PUWeight_down=1;
    relevantNumPU=0;
  }

  int Initialize(SampleInformation& mySampleInfo){
    //PU
    
    ConfigReader config;
    string WhatSample=mySampleInfo.GetSample();
    string WhatSubSample=mySampleInfo.GetSubSample();
    
    if(!oldpuw){
      nobinsmc = config.getDouble("PU_"+WhatSample+"_"+WhatSubSample+"_mc",PUmc," ");
      nobinsdata = config.getDouble("PU_data",PUdata," ");
      nobinsdata = config.getDouble("PU_data_up",  PUdata_up  ," ");
      nobinsdata = config.getDouble("PU_data_down",PUdata_down," ");
    }
    else if(oldpuw){
      nobinsmc = config.getDouble("oldPU_"+WhatSample+"_"+WhatSubSample+"_mc",PUmc," ");
    } 
    
    if(nobinsmc!=nobinsdata && !oldpuw){
      cout << "problem in pu inf in para_config - number of bins in MC PU dist is different than data!" << endl;
      return -1;
    }  
    return 0;
  }
  
  
  void RescaleWeight(float, double& weight, string type);
  double GetWeight(string type);
};


#endif
