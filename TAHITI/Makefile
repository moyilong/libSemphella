#
#
#	ELONE inside - ULTRAON Basic Protoco (libTAHITI.so Project)
#
#

GCC?=$(TARGET_CROSS)g++
LD?=$(TARGET_CROSS)ld
AR?=$(TARGET_CROSS)ar
OUTDIR?=$(CURDIR)/out
INSTALLDIR?=/usr
EXTERNEL_MODULES_DIR?=$(CURDIR)/mod
CXXFLAGS?=-O3

$(OUTDIR)/libTAHITI.so:
	if [ ! -e $(EXTERNEL_MODULES_DIR) ] ; then mkdir -p $(EXTERNEL_MODULES_DIR); fi
	mkdir -p $(OUTDIR)
	$(GCC) $(CXXFLAGS) -share -fpic -lSemphella -fpermissive -Wall -Wno-sign-compare  -lgomp -fopenmp -std=c++11 -fpermissive  -Wall -D__LINUX__ -I$(CURDIR)/../  -L$(OUTDIR) -o $(OUTDIR)/libTAHITI.so $$(ls | grep .cpp | grep -v \~)

all:$(OUTDIR)/libTAHITI.so
install:$(OUTDIR)/libTAHITI.so
	mkdir -p $(INSTALL_DIR)/bin
	cp $(OUTDIR)/libTAHITI.so $(INSTALL_DIR)/bin/libTAHITI.so
	mkdir -p $(INSTALL_DIR)/include/ULTRON
	cp $(CURDIR)/feature_define.h $(INSTALL_DIR)/include/ULTRON/TAHITI.h
	

clean:
	rm -rf $(OUTDIR)
