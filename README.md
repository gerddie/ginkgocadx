[![Build Status](https://travis-ci.org/gerddie/ginkgocadx.svg?branch=master)](https://travis-ci.org/gerddie/ginkgocadx)
[![Coverity Status](https://scan.coverity.com/projects/8214/badge.svg)](https://scan.coverity.com/projects/ginkgocadx)

# Ginkgo CADx #

## Preface ##

* Currently the project is maintained on a voluntary basis. 
* There is no PRO version.

## Introduction ##

Ginkgo CADx is an advanced DICOM viewer and dicomizer that can
also be used to convert png, jpeg, bmp, pdf, tiff to DICOM files.

The version developed here is a continuation of the now abandoned
free version developed by MetaEmotion:

    http://ginkgo-cadx.com/en/


## License ##

Ginkgo CADx is licensed under the terms of the Lesser GNU Public
Licence 3.

## Build ##

To build Ginkgo CADx the following software packages are required:

* ITK (>= 4.8)
* VTK (>= 6.2)
* wxWidgets (>= 3.0.1)
* DCMTK (>= 3.6.1-20150924)
* A C++ compiler that supports the *C++11* standard
* CMake

With these prerequisites available run *cmake* on the Ginkgo CADx
source tree and then use the selected build system to build the software.
After the software was sucessfully compiled run the install target.


The original Ginkgo CADx implementation was done with cross-platform
compatibility in mind, i.e. the software should compile on MS Windows,
Mac OS X, and flavours of Linux. However, currently the main development
platforms are Debian GNU/Linux and Gentoo Linux using the GNU gcc compiler
tool chain. Hence incompatibilities may creep in and bug reports with fixes
for other platforms are very welcome. 







  
