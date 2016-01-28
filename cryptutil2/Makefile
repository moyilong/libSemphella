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

$(OUTDIR)/cryptutil2:
	$(CCARGS) -o $(OUTDIR)/cryptutil2 cryptutil2.cpp -lSemphella -L$(OUTDIR) -I$(CURDIR)/../

install:$(OUTDIR)/cryptutil2
	mkdir -p $(INSTALL_DIR)/bin
	cp $(OUTDIR)/cryptutil2 $(INSTALL_DIR)/bin/cryptutil2

all:$(OUTDIR)/cryptutil2

clean:
	rm -rf $(OUTDIR)

	
