struct SUSYGenParticle { // To be filled with status-3 genParticles
  int pdgId; // PDG identifier (with sign, please)
  int firstMother; // first mother, set to <0 if no mothers
  double energy; // energy [GeV]
  double pt; // pt [GeV]
  double eta; // eta
  double phi; // phi
};
