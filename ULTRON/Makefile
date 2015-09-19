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

$(OUTDIR)/ULTRON:
	$(CCARGS) -o $(OUTDIR)/ULTRON ULTRON.cpp -lSemphella -lTAHITI -L$(OUTDIR) -I$(CURDIR)/../

install:$(OUTDIR)/ULTRON
	mkdir -p $(INSTALL_DIR)/bin
	cp $(OUTDIR)/ULTRON $(INSTALL_DIR)/bin/ULTRON

all:$(OUTDIR)/ULTRON

clean:
	rm -rf $(OUTDIR)

	
