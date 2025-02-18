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
#	14 November, 2024 - E M Thornber
#	Corrected package build logfile directory
#
#	10 February, 2025 - E M Thornber
#	Added package name EV
#
#	EVs required
#	BFILE - path to canpi executable
#	ODIR  - where to place the .deb file
#	PKGNAME - package name
#	SDIR  - Current source directory
#	VERS  - package version
#
#	Working directory is assumed to be ${CMAKE_CURRENT_SOURCE_DIR}
################################################################################

ARCH := $(shell dpkg-architecture --query DEB_HOST_ARCH)
EPM := $(shell which epm)
PKGDEFN := $(SDIR)/package/$(PKGNAME).epm
PYTHON := /usr/bin/python3
RM=/bin/rm -f
TS=`date "+%y%m%d-%H%M"`

all: pkgs

pkgs: portable deb

deb portable:
	sudo $(RM) $(ODIR)/$@-*.log
	sudo $(EPM) -a ${ARCH} -f $@ -vv --output-dir $(ODIR) binary=$(BFILE) srcdir=$(SDIR) version=$(VERS) $(PKGNAME) $(PKGDEFN) 2>&1 | tee $(ODIR)/$@-$(TS).log

