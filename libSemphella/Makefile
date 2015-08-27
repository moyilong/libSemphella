#
#
#	ELONE inside - libSemphella
#
#

GCC?=$(TARGET_CROSS)g++
LD?=$(TARGET_CROSS)ld
AR?=$(TARGET_CROSS)ar
OUTDIR?=$(CURDIR)/out
INSTALLDIR?=/usr

$(OUTDIR)/libSemphella.so:
	mkdir -p $(OUTDIR)
	$(GCC) -fpic -shared $(CXXFLAGS) -O3  -fpermissive -Wall -Wno-sign-compare  -lgomp -fopenmp -std=c++11 -fpermissive  -Wall -D__LINUX__ -D__DLL_EXPORT -o $(OUTDIR)/libSemphella.so $$(ls | grep .cpp | grep -v \~)

all:$(OUTDIR)/libSemphella.so
install:$(OUTDIR)/libSemphella.so
	mkdir -p $(INSTALL_DIR)/lib
	cp $(OUTDIR)/libSemphella.so $(INSTALL_DIR)/lib/libSemphella.so
	mkdir -p $(INSTALL_DIR)/include/libSemphella
	cp $(CURDIR)/*.h $(INSTALL_DIR)/include/libSemphella
	

clean:
	rm -rf $(OUTDIR)
