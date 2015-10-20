#
#
#	ELONE inside - libSemphella Project
#
#
OUTDIR?=$(CURDIR)/out
INSTALL_DIR=/usr
MAKE?=make
CXXFLAGS?=-D_LIB_SEMPHELLA
CFLAGS?=-D_LIB_SEMPHELLA -L$(OUTDIR)
MAKEFLAGS:=

SUBDIR=libSemphella TAHITI bios mpshell PT time_lize  ULTRON


all:
	for dir in $(SUBDIR); do \
		echo "sub call for each $$dir" ; \
		$(MAKE) -C $$dir all  OUTDIR="$(OUTDIR)" INSTALL_DIR="$(INSTALL_DIR)" CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)";  \
	done 

install:
	for dir in $(SUBDIR); do \
		echo "sub call for each $$dir" ; \
		$(MAKE) -C $$dir install   OUTDIR="$(OUTDIR)" INSTALL_DIR="$(INSTALL_DIR)" CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)";  \
	done 

clean:
	for dir in $(SUBDIR); do \
		echo "sub call for each $$dir" ; \
		$(MAKE) -C $$dir clean   OUTDIR="$(OUTDIR)" INSTALL_DIR="$(INSTALL_DIR)" CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)";  \
	done 
