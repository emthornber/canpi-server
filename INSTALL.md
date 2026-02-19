# CANPi Gateway to CBus

This functionality is distributed as a Debian package, as a compressed source tarball,
and contained in an Apt Repository.

- The package and the sources are available as part of a release on github
(`emthornber/canpi-server`)

- The Apt repository is available at [MERG-DEV Apt
Repository](https://emthornber.github.io/RPi13Repo).  Visiting the site displays instructions
on how to setup the apt configuration to access the MERG-DEV repository along
with a downloadable script to carry out those instructions.

## Compiling

The executable needs the [log4cpp](https://log4cpp.sourceforge.net) 
and [libgpiod](https://github.com/brgl/libgpiod/) shared libraries and interface
development libraries.
```
sudo apt install liblog4cpp-doc liblog4cpp5-dev liblog5v5
sudo apt install libgpiod-doc libgpiod-dev libgpiod3
```

The .deb package is built using Easy Package Manager (EPM) (`emthornber/epm` tag
`v5.0.1`) which is built from source using the usual autotools incantation
```
./configure
make
sudo make install
```


The build system used is [cmake](https://cmake.org) 
```
cmake -S . -B build
cmake --build build
```

## Installation

After a successful build there is a Debian package (and a portable tarball) in `./package` e.g.
```
canpi-server-2.0.0-trixie-armhf.deb
canpi-server-2.0.0-trixie-armhf.tar.gz
```
which can be installed using `apt`
```
sudo apt install ./package/canpi-server-2.0.0-trixie-armhf.deb
```
