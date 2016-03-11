ifdef ROOTSYS
include $(ROOTSYS)/etc/Makefile.arch
else
	@echo "NO ROOTSYS!"
endif


MYCLASSES := SectionInfo Student
MYCLASSES += GradeCluster MyFunctions

MYHEADERS := $(addsuffix .h, $(MYCLASSES))
MYSOURCES := $(addsuffix .C, $(MYCLASSES))
MYOBJECTS := $(addsuffix .$(ObjSuf), $(MYCLASSES))

LRNANAO = $(MYOBJECTS) LrnAnaDict.$(ObjSuf)
LRNANAS = $(MYSOURCES) LrnAnaDict.C
LRNANASO = libLrnAna.$(DllSuf)
LRNANALIB = $(LRNANASO)

CPPFLAGS += -Wno-unused-parameter

$(LRNANASO): $(LRNANAO)
	$(LD) $(SOFLAGS)$@ $(LDFLAGS) $^ $(OutPutOpt) $@ $(EXPLLINKLIBS)
	@echo "$@ done"
	
dictionary: LrnAnaDict.$(DllSuf)
	
LrnAnaDict.$(DllSuf): LrnAnaDict.C $(MYSOURCES)
	$(CXX) -fPIC -shared -o $@ $(ROOTLDFLAGS) $(CPPFLAGS) $(ROOTLIBS) -I$(ROOTSYS)/include $^
	
LrnAnaDict.C: $(MYHEADERS) LinkDef.h
	@echo "Generating Dictionary $@..."
	$(ROOTCLING) -f $@ -c $^
	
default: test
	
test:
	@echo "Test makefile LA Project"
	@echo "RC            = " $(RC)
	@echo "Source suffix = " $(SrcSuf)
	@echo "Object suffix = " $(ObjSuf)
	@echo "Dll suffix    = " $(DllSuf)
	@echo "c++ compiler  = " $(CXX)
	@echo "ROOTLDFLAGS   = " $(ROOTLDFLAGS)
	@echo "CPPFLAGS      = " $(CPPFLAGS)
	@echo "ROOTLIBS      = " $(ROOTLIBS)
	@echo "LD            = " $(LD)
	@echo "PLATFORM      = " $(PLATFORM)
	@echo "MACOSX_MINOR  = " $(MACOSX_MINOR)
	@echo "MINOR test    = " $(subst $(MACOSX_MINOR),,1234)
	@echo "ROOTCLING     = " $(ROOTCLING)
	@echo "MYOBJECTS     = " $(MYOBJECTS)
	@echo "SOFLAGS       = " $(SOFLAGS)
	@echo "LDFLAGS       = " $(LDFLAGS)
	@echo "OutPutOpt     = " $(OutPutOpt)
	@echo "EXPLLINKLIBS  = " $(EXPLLINKLIBS)
	@echo "CFLAGS        = " $(CFLAGS)
	
.$(SrcSuf).$(ObjSuf):
	@echo "Compiling $@"
	$(CXX) $(CFLAGS) -c $<
	@echo "-----------------------------------------------------"

clean::
	rm -f *.o *~ \#*
	@rm -f $(MYOBJECTS) core

