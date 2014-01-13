#ifndef isoTrackVeto_h
#define isoTrackVeto_h

#include "NtupleTools2_h.h"
#include "CutFlow.h"
#include "Particle.h"

namespace isoTrackVeto {
  void IsoTracksV4 ( vector<Particle>& allTracks);
  bool IsoTrackVetoV4 ( Particle lepton, vector<Particle>& allTracks);
  bool IsoTrackVetoV4 ( const Particle* lepton, const vector< const Particle*>* allTracks);
}
#endif
