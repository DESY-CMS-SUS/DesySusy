
# Makefile for the DesySusy ntuple analysis
#
# Author: Dirk Kruecker, 3.6.203
#
#----------------------- here you can input your files and libraries ----------
#
# we compile all *.C and *.cpp defined in SOURCES
#

  SOURCES = NoSystematic.cpp PUReweight.cpp BTagReweight.cpp JES.cpp JER.cpp

  LIB_HEADERS =  ./NoSystematic.h ./PUReweight.h ./BTagReweight.h ./JES.h ./JER.h ./Systematic.h ./SystematicFactory.h

# and create this executable
#
  EXEC=nTupler

# with additional libs and includes
# e.g. MORELIBS = -L/path -lblub
MORELIBS = -L../../ -lRA4bDict

# e.g. MOREINCS = -I/path

OBJS	  = $(notdir $(SOURCES))
OBJS     := $(OBJS:.cpp=.o)
OBJS     := $(OBJS:.C=.o)

$(info Using the following files: $(SOURCES) )
#$(info Using the following files: $(OBJS) )

#------------------------------------------------------------------------------

ifndef ROOTSYS
  $(error ROOTSYS is not defined!)
endif

ROOTLIBS    = `root-config --libs` -lGenVector -lMathMore
CXX         = g++
CXXFLAGS    = `root-config --cflags` -I./ -I/afs/desy.de/user/f/fcost/TEST/UserCode/DesySusy/stop_2013/ -I/afs/desy.de/user/f/fcost/TEST/UserCode/DesySusy/stop_2013/src/
LD          = g++
LDFLAGS     = `root-config --ldflags`

.SUFFIXES: .cpp .C .o .so

#------------------------------------------------------------------------------

all:   libsystematics.so

libsystematics.so: $(OBJS) Dict_sl.o
	$(LD) -shared $(LDFLAGS) -o $@ $(CXXFLAGS) $^ -fpic $(ROOTLIBS) $(MORELIBS)

Dict.cxx: Linkdef.h
	@echo "Generating dictionary $@..."
	@rootcint -f $@ -c $(CXXFLAGS) -p $(LIB_HEADERS) $^
Dict_sl.o: Dict.cxx
	$(CXX) -c $(CXXFLAGS) -fpic $^ -o $@

clean:
	@echo rm -f $(EXEC) *Dict.*  core $(OBJS) *.d
	@rm -f $(EXEC) *Dict.* *.o core $(OBJS) *.d *~ libsystematics.so

.cpp.o:
		$(CXX) $(CXXFLAGS) -c -fpic $<
.C.o:

		$(CXX) $(CXXFLAGS) -c -fpic $<
