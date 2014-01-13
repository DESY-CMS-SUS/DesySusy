#include "NoSystematic.h"

Systematics::NoSystematic::NoSystematic(){
  //NoSystematic::NoSystematic(){
  InEvent = 0;
  //OutEvent = Event();
}

void Systematics::NoSystematic::Reset(){
  //void NoSystematic::Reset(){
  InEvent = 0;
  OutEvent = Event();
}

void Systematics::NoSystematic::Eval( Event& in){
  //void NoSystematic::Eval( Event& in){
  InEvent=&in;  
  OutEvent = Event(in);
}

Event* Systematics::NoSystematic::SysEvent(){
  //Event* NoSystematic::SysEvent(){
  return &OutEvent;
}

