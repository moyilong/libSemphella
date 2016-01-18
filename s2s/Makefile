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

$(OUTDIR)/s2s:
	$(CCARGS) -o $(OUTDIR)/s2s s2s.cpp -lSemphella -L$(OUTDIR) -I$(CURDIR)/../

install:$(OUTDIR)/s2s
	mkdir -p $(INSTALL_DIR)/bin
	cp $(OUTDIR)/s2s $(INSTALL_DIR)/bin/s2s

all:$(OUTDIR)/s2s

clean:
	rm -rf $(OUTDIR)

	
