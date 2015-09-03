#
#
#	ELONE inside
#
#
 GCC?=$(TARGET_CROSS)g++
 CONFIG_VERSION_DIST?="unknow"
CCARGS:=$(GCC) $(CXXFLAGS) -D__LINUX__ -std=c++11 -fpermissive -D_LINUX -O3 -fopenmp
OUTDIR?=.
INSTALL_DIR?=/usr


$(OUTDIR)/PT:
	$(CCARGS) -o $(OUTDIR)/PT -lSemphella -L$(OUTDIR) -I$(CURDIR)/../ runner.cpp libPT.cpp

install:$(OUTDIR)/PT
	mkdir -p $(INSTALL_DIR)/bin
	cp $(OUTDIR)/PT $(INSTALL_DIR)/bin/PT

all: $(OUTDIR)/PT

clean:
	rm -rf $(OUTDIR)
	

