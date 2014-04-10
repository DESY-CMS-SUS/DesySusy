#include "EventInfo.h"

/*
EventInfo::EventInfo(const EventInfo& copy){
  Sample     = copy.Sample;      
  SubSample  = copy.SubSample; 
  Estimation = copy.Estimation;
  Tail	     = copy.Tail;      
  isData     = copy.isData;    
  
  xs           = copy.xs;
  NEvents      = copy.NEvents;
  FE  	       = copy.FE;    

  Event = copy.Event;
  Lumi  = copy.Lumi;
  Run   = copy.Run; 

  PUInter       = copy.PUInter;                      
  PUWeight	= copy.PUWeight;                     
  PUWeight_up 	= copy.PUWeight_up;                  
  PUWeight_down = copy.PUWeight_down;                
  NPV 		= copy.NPV;                          
  NgoodPV 	= copy.NgoodPV;     
		   
  mStop = copy.mStop;                           
  mLsp  = copy.mLsp;                         
}

/*
EventInfo& EventInfo::operator= (const EventInfo& copy){
  std::cout<<"EventInfo_operator= "<<xs<<" "<<copy.xs<<std::endl;
  xs           = copy.xs;
  std::cout<<"EventInfo_operator= "<<xs<<" "<<copy.xs<<std::endl;
}

*/
