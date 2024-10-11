################################################################################
#
#       18 July, 2022 - E M Thornber
#       Created
#
#	21 December, 2023 - E M Thornber
#	Create canpi.cfg from definition file
#
#	29 December, 2023 - E M Thornber
#	Moved creation of canpi.cfg to CMake
#
#	11 October, 2024 - E M Thornber
#	Streamlined targets
#
#	EVs required
#	BFILE - path to canpi executable
#	ODIR  - where to place the .deb file
#	SDIR  -  Current source directory
#	VERS  - package version
#
#	Working directory is assumed to be ${CMAKE_CURRENT_SOURCE_DIR}
################################################################################

EPM := $(shell which epm)
PKGNAME := $(shell basename $(SDIR) )
PKGDEFN := $(SDIR)/package/$(PKGNAME).epm
PYTHON := /usr/bin/python3
RM=/bin/rm -f
TS=`date "+%y%m%d-%H%M"`

all: pkgs

pkgs: portable deb

deb portable:
	sudo $(RM) $(ODIR)/$@-*.log
	sudo $(EPM) -a armhf -f $@ -vv --output-dir $(ODIR) binary=$(BFILE) srcdir=$(SDIR) version=$(VERS) $(PKGNAME) $(PKGDEFN) 2>&1 | tee $@-$(TS).log

