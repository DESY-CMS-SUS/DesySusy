#ifndef makeTracks_h
#define makeTracks_h

#include "NtupleTools2_h.h"
#include "CutFlow.h"
#include "Particle.h"

vector<Particle> makeAllTracks(EasyChain* tree, CutSet* flow_in = 0);
#endif
