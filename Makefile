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
MAKEFLAGS:=OUTDIR=$(OUTDIR) INSTALL_DIR=$(INSTALL_DIR)

SUBDIR=libSemphella libDragonEDGE bios mpshell PT time_lize 

define for_each
	for dir in $(SUBDIR); do \
		echo "sub call for each $$dir" ; \
		$(MAKE) -C $$dir $(1) $(MAKEFLAGS) $(2);  \
	done 
endef

all:
	$(call for_each,all)

install:
	$(call for_each,install)

clean:
	$(call for_each,clean)
