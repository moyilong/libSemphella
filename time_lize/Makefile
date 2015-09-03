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

$(OUTDIR)/time_lize:
	$(CCARGS) -o $(OUTDIR)/time_lize time_lize.cpp stdafx.cpp -lSemphella -lgomp -L$(OUTDIR) -I$(CURDIR)/../

install:$(OUTDIR)/time_lize
	mkdir -p $(INSTALL_DIR)/bin
	cp $(OUTDIR)/time_lize $(INSTALL_DIR)/bin/time_lize

all:$(OUTDIR)/time_lize

clean:
	rm -rf $(OUTDIR)

	
