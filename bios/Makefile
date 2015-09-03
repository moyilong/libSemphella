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

$(OUTDIR)/bios:
	$(CCARGS) -o $(OUTDIR)/bios bios.cpp -lSemphella -lgomp -L$(OUTDIR) -I$(CURDIR)/../

install:$(OUTDIR)/bios
	mkdir -p $(INSTALL_DIR)/bin
	cp $(OUTDIR)/bios $(INSTALL_DIR)/bin/bios

all:$(OUTDIR)/bios

clean:
	rm -rf $(OUTDIR)

	
