#
#
#	ELONE inside - libSemphella Project
#
#
OUTDIR?=$(CURDIR)/out
MAKE?=make
MAKEFLAGS?=
CXXFLAGS?=-D_LIB_SEMPHELLA
CFLAGS?=-D_LIB_SEMPHELLA -L$(OUTDIR)
MAKEFLAGS:=OUTDIR=$(OUTDIR)

SUBDIR=libSemphella libDragonEDGE bios mpshell PT time_lize 

all:
	for dir in $(SUBDIR); do \
		$(MAKE) -C $$dir all $(MAKEFLAGS); \
	done \

install:
	for dir in $(SUBDIR); do \
		$(MAKE) -C $$dir install $(MAKEFLAGS); \
	done \

clean:
	for dir in $(SUBDIR); do \
		$(MAKE) -C $$dir clean $(MAKEFLAGS); \
	done \
