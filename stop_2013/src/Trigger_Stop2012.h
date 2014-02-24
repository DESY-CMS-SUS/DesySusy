#ifndef TRIGGER_STOP2012_h
#define TRIGGER_STOP2012_h

#include <map>
#include <string>
#include <iostream>

#include "NtupleTools2_h.h"
#include "ConfigReader.h"

#include "Particle.h"

class Trigger_Stop2012{
public:
  Trigger_Stop2012(){ this->triggerNames.clear();};
  
  const std::vector<const char*>* TriggerNames( const string& sample, const int& run, const Particle* lep);
  bool Fired(EasyChain* tree, const string& sample, const int& run, const Particle* lep);

private:
    vector<const char*> triggerNames;
};
#endif
