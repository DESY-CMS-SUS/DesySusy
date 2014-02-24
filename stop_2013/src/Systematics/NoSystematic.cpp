#include "NoSystematic.h"

Systematics::NoSystematic::NoSystematic(){
  InEvent = 0;
}

void Systematics::NoSystematic::Reset(){
  InEvent = 0;
  OutEvent = Event();
}

void Systematics::NoSystematic::Eval( Event& event_){
  InEvent=&event_;  
  OutEvent = Event(event_);
}

Event* Systematics::NoSystematic::SysEvent(){
  return &OutEvent;
}

