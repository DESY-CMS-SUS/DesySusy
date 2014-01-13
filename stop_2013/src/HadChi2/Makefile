
# Makefile for the DesySusy ntuple analysis
#
# Author: Dirk Kruecker, 3.6.203
#
#----------------------- here you can input your files and libraries ----------
#
# we compile all *.C and *.cpp defined in SOURCES
#

  SOURCES = hadChi2.cpp

  LIB_HEADERS =  ./hadChi2.h

# and create this executable
#
  EXEC=nTupler

# with additional libs and includes
# e.g. MORELIBS = -L/path -lblub

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

ROOTLIBS    = `root-config --libs` -lGenVector -lMathMore -lMinuit2
CXX         = g++
CXXFLAGS    = `root-config --cflags` -I./
LD          = g++
LDFLAGS     = `root-config --ldflags`

.SUFFIXES: .cpp .C .o .so

#------------------------------------------------------------------------------

all:   libhadchi2.so $(EXEC)

$(EXEC):  $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^ $(ROOTLIBS) $(MORELIBS)
	@echo "$@ done"

libhadchi2.so: $(OBJS) Dict_sl.o
	$(LD) -shared $(LDFLAGS) -o $@ $(CXXFLAGS) $^ -fpic $(ROOTLIBS) $(MORELIBS)

Dict.cxx: Linkdef.h
	@echo "Generating dictionary $@..."
	@rootcint -f $@ -c  -p $(LIB_HEADERS) $^
Dict_sl.o: Dict.cxx
	$(CXX) -c $(CXXFLAGS) -fpic $^ -o $@

clean:
	@echo rm -f $(EXEC) *Dict.*  core $(OBJS) *.d
	@rm -f $(EXEC) *Dict.* *.o core $(OBJS) *.d *~ libhadchi2.so 

.cpp.o:
		$(CXX) $(CXXFLAGS) -c -fpic $<
.C.o:

		$(CXX) $(CXXFLAGS) -c -fpic $<
