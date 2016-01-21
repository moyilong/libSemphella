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

$(OUTDIR)/mconf:
	$(CCARGS) -o $(OUTDIR)/mconf -luci -lSemphella -L$(OUTDIR) -I$(CURDIR)/../ $$(ls | grep .cpp | grep -v \~)

install:$(OUTDIR)/mconf
	mkdir -p $(INSTALL_DIR)/bin
	cp $(OUTDIR)/mconf $(INSTALL_DIR)/bin/mconf

all:$(OUTDIR)/mconf

clean:
	rm -rf $(OUTDIR)

	
