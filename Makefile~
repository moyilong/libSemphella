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


all:
	for dir in $(SUBDIR); do \
		echo "sub call for each $$dir" ; \
		$(MAKE) -C $$dir all  $(MAKEFLAGS);  \
	done 

install:
	for dir in $(SUBDIR); do \
		echo "sub call for each $$dir" ; \
		$(MAKE) -C $$dir install  $(MAKEFLAGS);  \
	done 

clean:
	for dir in $(SUBDIR); do \
		echo "sub call for each $$dir" ; \
		$(MAKE) -C $$dir clean  $(MAKEFLAGS);  \
	done 
