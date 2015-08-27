#
#
#	ELONE inside - libDragonEDGE
#
#

GCC?=$(TARGET_CROSS)g++
LD?=$(TARGET_CROSS)ld
AR?=$(TARGET_CROSS)ar
OUTDIR?=$(CURDIR)/out
INSTALLDIR?=/usr

$(OUTDIR)/libDragonEDGE.so:
	mkdir -p $(OUTDIR)
	$(GCC) -fpic -shared $(CXXFLAGS) -O3  -I$(CURDIR)/../ -lSemphella -fpermissive -Wall -Wno-sign-compare  -lgomp -fopenmp -std=c++11 -fpermissive  -Wall -D__LINUX__ -D__DLL_EXPORT -L$(OUTDIR) -o $(OUTDIR)/libDragonEDGE.so $$(ls | grep .cpp | grep -v \~)

all:$(OUTDIR)/libDragonEDGE.so
install:$(OUTDIR)/libDragonEDGE.so
	mkdir -p $(INSTALL_DIR)/lib
	cp $(OUTDIR)/libDragonEDGE.so $(INSTALL_DIR)/lib/libDragonEDGE.so
	mkdir -p $(INSTALL_DIR)/include/libDragonEDGE
	cp $(CURDIR)/*.h $(INSTALL_DIR)/include/libDragonEDGE
	

clean:
	rm -rf $(OUTDIR)
