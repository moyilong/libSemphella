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

$(OUTDIR)/cryptutil:
	$(CCARGS) -o $(OUTDIR)/cryptutil cryptutil.cpp -lSemphella -L$(OUTDIR) -I$(CURDIR)/../

install:$(OUTDIR)/cryptutil
	mkdir -p $(INSTALL_DIR)/bin
	cp $(OUTDIR)/cryptutil $(INSTALL_DIR)/bin/cryptutil

all:$(OUTDIR)/cryptutil

clean:
	rm -rf $(OUTDIR)

	
