#
#
#	ELONE inside - libSemphella Project
#
#
OUTDIR?=$(CURDIR)/out
MAKE?=make
MAKEFLAGS?=
all:
	mkdir -p $(OUTDIR)
	for dir in $(ls $(CURDIR)); do if [ -e $$dir/Makefile ]; then \
		$(MAKE) -C $$dir all OUTDIR=$(OUTDIR) CFLAGS=$(CFLAGS) CXXFLAGS=$(CXXFLAGS) \
	fi; done;
clean:
	rmdir -rfv $(OUTDIR)


	
