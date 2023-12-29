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
#	EVs required
#	BFILE - path to canpi executable
#	ODIR - where to place the .deb file
#	SDIR -  Current source directory
#
#	Working directory is assumed to be ${CMAKE_CURRENT_SOURCE_DIR}
################################################################################

EPM=/usr/local/bin/epm
PKGNAME := $(shell basename $(SDIR) )
PKGDEFN := $(PKGNAME).epm
PYTHON := /usr/bin/python3
RM=/bin/rm -f
TS=`date "+%y%m%d-%H%M"`

all: clean pkgs

clean:
	sudo $(RM) *.log

pkgs: portable deb

portable:
	sudo $(EPM) -a armhf -f portable -vv --output-dir $(ODIR) binary=$(BFILE) srcdir=$(SDIR) $(PKGNAME) $(SDIR)/package/$(PKGDEFN) 2>&1 | tee portable-$(TS).log

deb:
	sudo $(EPM) -a armhf -f deb      -vv --output-dir $(ODIR) binary=$(BFILE) srcdir=$(SDIR) $(PKGNAME) $(SDIR)/package/$(PKGDEFN) 2>&1 | tee deb-$(TS).log

