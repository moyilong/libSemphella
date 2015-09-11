#
#
#	ELONE inside - ULTRAON Basic Protoco (TAHITI Project)
#
#

GCC?=$(TARGET_CROSS)g++
LD?=$(TARGET_CROSS)ld
AR?=$(TARGET_CROSS)ar
OUTDIR?=$(CURDIR)/out
INSTALLDIR?=/usr
EXTERNEL_MODULES_DIR?=$(CURDIR)/mod

$(OUTDIR)/TAHITI:
	if [ ! -e $(EXTERNEL_MODULES_DIR) ] ; then mkdir -p $(EXTERNEL_MODULES_DIR); fi
	mkdir -p $(OUTDIR)
	$(GCC) -fpic -shared $(CXXFLAGS) -O3  -I$(CURDIR)/../ -lSemphella -fpermissive -Wall -Wno-sign-compare  -lgomp -fopenmp -std=c++11 -fpermissive  -Wall -D__LINUX__ -D__DLL_EXPORT -L$(OUTDIR) -o $(OUTDIR)/libDragonEDGE.so $$(ls | grep .cpp | grep -v \~) $$(ls $(EXTERNEL_MODULES_DIR)| grep .cpp | grep -v \~)

all:$(OUTDIR)/TAHITI
install:$(OUTDIR)/TAHITI
	mkdir -p $(INSTALL_DIR)/bin
	cp $(OUTDIR)/TAHITI $(INSTALL_DIR)/bin/TAHITI
	mkdir -p $(INSTALL_DIR)/include/ULTRON
	cp $(CURDIR)/feature_define.h $(INSTALL_DIR)/include/ULTRON/TAHITI.h
	

clean:
	rm -rf $(OUTDIR)
