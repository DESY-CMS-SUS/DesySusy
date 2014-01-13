#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

#pragma link C++ defined_in "Systematic.h";
#pragma link C++ defined_in "NoSystematic.h";
#pragma link C++ defined_in "SystematicFactory.h";

#pragma link C++ namespace Systematics+;
#pragma link C++ class Systematics::Systematic+;
#pragma link C++ class std::vector<Systematics::Systematic*>+;
#pragma link C++ class Systematics::NoSystematic+;
#pragma link C++ class std::vector<Systematics::NoSystematic*>+;
#pragma link C++ class Systematics::SystematicFactory+;
#endif
