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

$(OUTDIR)/mpshell:
	$(CCARGS) -o $(OUTDIR)/mpshell mpshell.cpp -lSemphella -L$(OUTDIR) -I$(CURDIR)/../

install:$(OUTDIR)/mpshell
	mkdir -p $(INSTALL_DIR)/bin
	cp $(OUTDIR)/mpshell $(INSTALL_DIR)/bin/mpshell

all:$(OUTDIR)/mpshell

clean:
	rm -rf $(OUTDIR)

	
