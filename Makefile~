#
#
#	ELONE inside - libSemphella Project
#
#
OUTDIR?=$(CURDIR)/out
INSTALL_DIR=/usr
MAKE?=make
MAKEFLAGS?=
CXXFLAGS?=-D_LIB_SEMPHELLA
CFLAGS?=-D_LIB_SEMPHELLA -L$(OUTDIR)
MAKEFLAGS:=OUTDIR=$(OUTDIR) INSTALL_DIR=$(INSTALL_DIR) CFLAGS=$(CFLAGS) CXXFLAGS=$(CXXFLAGS)

SUBDIR=libSemphella libDragonEDGE bios mpshell PT time_lize 

WORKD_FOR

define for_each
	for dir in $(SUBDIR); do \
		echo "sub call for each $$dir" ; \
		$(MAKE) -C $$dir $(WORK_FOR) $(MAKEFLAGS);  \
	done 
endef

all:
	WORK_FOR=all
	$(call for_each)

install:
	WORK_FOR=install
	$(call for_each)

clean:
	WORK_FOR=clean
	$(call for_each)
