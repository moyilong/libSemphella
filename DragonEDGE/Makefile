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

$(OUTDIR)/DragonEDGE:
	mkdir -p $(OUTDIR)
	$(CCARGS) -o $(OUTDIR)/DragonEDGE $$(ls | grep .cpp | grep -v \~ | grep -v windows.cpp) -lSemphella -lgomp -L$(OUTDIR) -I$(CURDIR)/../

install:$(OUTDIR)/DragonEDGE
	mkdir -p $(INSTALL_DIR)/bin
	cp $(OUTDIR)/DragonEDGE $(INSTALL_DIR)/bin/DragonEDGE

all:$(OUTDIR)/DragonEDGE

clean:
	rm -rf $(OUTDIR)

	
