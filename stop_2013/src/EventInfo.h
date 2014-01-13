#ifndef EventInfo_h
#define EventInfo_h

#include <map>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class EventInfo{

public:

  EventInfo() {};
  EventInfo(const EventInfo&);
  //EventInfo& operator= (const EventInfo&);

  // move constructor
  //Example6 (Example6&& x) : ptr(x.ptr) {x.ptr=nullptr;}
  // move assignment
  //Example6& operator= (Example6&& x) {
    
  ~EventInfo() {};
  
  string Sample;
  string SubSample;
  string Estimation;
  string Tail;
  bool   isData;
  int    LepFromTop;

  double xs;
  int    NEvents;
  double FE;
  double GlobalWeight;

  unsigned int Event;
  unsigned int Lumi;
  unsigned int Run;

  double TriggerWeight;

  int    PUInter;
  double PUWeight;
  double PUWeight_up;
  double PUWeight_down;
  int    NPV;
  int    NgoodPV;

  double EventWeight;
  
  double rho;

  double isrWeight;
  double topPtWeight;

  double mY;
  double mLsp;
  double x;

  double T2ttL;
  double T2ttR;
  
  double T2bWLL;
  double T2bWLS;
  double T2bWLR;

  double T2bWSL;
  double T2bWSS;
  double T2bWSR;

  double T2bWRL;
  double T2bWRS;
  double T2bWRR;

  bool badLaserFilter;
  bool badXTalLaserCorrectionFilter;
  bool trackingPOGFilter;
};	  
#endif
