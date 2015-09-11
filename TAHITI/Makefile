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
CXXFLAGS?=-O3
CXXFLAGS+=-lSemphella -fpermissive -Wall -Wno-sign-compare  -lgomp -fopenmp -std=c++11 -fpermissive  -Wall -D__LINUX__

$(OUTDIR)/TAHITI:
	if [ ! -e $(EXTERNEL_MODULES_DIR) ] ; then mkdir -p $(EXTERNEL_MODULES_DIR); fi
	mkdir -p $(OUTDIR)
	$(GCC) $(CXXFLAGS) -I$(CURDIR)/../  -L$(OUTDIR) -o $(OUTDIR)/TAHITI $$(ls | grep .cpp | grep -v \~) $$(tree $(EXTERNEL_MODULES_DIR)| grep .cpp.o | grep -v \~)

all:$(OUTDIR)/TAHITI
install:$(OUTDIR)/TAHITI
	mkdir -p $(INSTALL_DIR)/bin
	cp $(OUTDIR)/TAHITI $(INSTALL_DIR)/bin/TAHITI
	mkdir -p $(INSTALL_DIR)/include/ULTRON
	cp $(CURDIR)/feature_define.h $(INSTALL_DIR)/include/ULTRON/TAHITI.h
	

clean:
	rm -rf $(OUTDIR)
