#ifndef SubSampling_h
#define SubSampling_h

#include <string>
#include <vector>

#include "NtupleTools2_h.h"

using namespace std;

namespace desy_tools{
  bool ScanCheck(const string&, const string&, EasyChain*);
  bool T2tbPoints( const string& subSample_, EasyChain* tree_);
  bool TTJetsSubSampling( const string& subSample_, EasyChain* tree_);
}
#endif
