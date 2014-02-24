#ifndef TriggerEfficiencyProvider_h
#define TriggerEfficiencyProvider_h

#include <vector>
#include <string>

#include "NtupleTools2_h.h"

#include "Event.h"

using namespace std;

namespace trigger{
  double GetTriggerEfficiency( Event& event);

  double Ele27wp80( double pt, double eta);
  double IsoMu24( double pt, double eta);
  double XTrigger_MuonLeg( double pt, double eta);
  double XTrigger_HadLeg( double pt);
  double PFNoPUJet30_TurnOn(double pt);
  double PFNoPUJet20_TurnOn(double pt);
  double PFJet30_TurnOn(double pt);
}
#endif
