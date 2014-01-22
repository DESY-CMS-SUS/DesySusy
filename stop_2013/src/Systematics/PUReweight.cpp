#include "PUReweight.h"
#include "EventInfo.h"

Systematics::PUReweight::PUReweight(TString variation_){
  InEvent = 0;
  OutEvent = Event();

  variation = variation_;
  variation.ToLower();
}

void Systematics::PUReweight::Reset(){
  InEvent = 0;
  OutEvent = Event();
}

void Systematics::PUReweight::Eval( Event& event_){
  InEvent = &event_;
  OutEvent = Event(event_);

  if ( event_.Info()->isData)
    return;

  EventInfo info = *event_.Info();

  if(variation=="up")
    info.PUWeight = info.PUWeight_up;
  else if (variation=="down")
    info.PUWeight = info.PUWeight_down;  
  
  OutEvent.SetInfo(info);
}

void Systematics::PUReweight::Eval( Event& event_, TString variation_){
  variation = variation_;
  variation.ToLower();
  this->Eval(event_);
}

Event* Systematics::PUReweight::SysEvent(){
  return &OutEvent;
}

