#include "EventInfo.h"


EventInfo::EventInfo(const EventInfo& copy){

  //std::cout<<"EventInfo_Copy"<<std::endl;

  Sample     = copy.Sample;      
  SubSample  = copy.SubSample; 
  Estimation = copy.Estimation;
  Tail	     = copy.Tail;      
  isData     = copy.isData;    
  
  xs           = copy.xs;
  NEvents      = copy.NEvents;
  FE  	       = copy.FE;    
  GlobalWeight = copy.GlobalWeight;

  Event = copy.Event;
  Lumi  = copy.Lumi;
  Run   = copy.Run; 

  TriggerWeight = copy.TriggerWeight;

  PUInter       = copy.PUInter;                      
  PUWeight	= copy.PUWeight;                     
  PUWeight_up 	= copy.PUWeight_up;                  
  PUWeight_down = copy.PUWeight_down;                
  NPV 		= copy.NPV;                          
  NgoodPV 	= copy.NgoodPV;     
		   
  EventWeight = copy.EventWeight;                  
				                                 
  mY   = copy.mY;                           
  mLsp = copy.mLsp;                         
  x    = copy.x;                            

  badLaserFilter         	= copy.badLaserFilter;               
  badXTalLaserCorrectionFilter  = copy.badXTalLaserCorrectionFilter; 
  trackingPOGFilter		= copy.trackingPOGFilter;
}

/*
EventInfo& EventInfo::operator= (const EventInfo& copy){
  std::cout<<"EventInfo_operator= "<<xs<<" "<<copy.xs<<std::endl;
  xs           = copy.xs;
  std::cout<<"EventInfo_operator= "<<xs<<" "<<copy.xs<<std::endl;
}

*/
